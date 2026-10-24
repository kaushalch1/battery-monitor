
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define ADC_VBAT_PIN 1
#define LOAD_PIN 2
#define START_PIN 4
#define STOP_PIN 5
#define CHEM_PIN 6
#define SDA_PIN 8
#define SCL_PIN 9
#define ADC_TEMP_PIN 10
#define LED_RUN 11
#define LED_ERR 12

#define INA_ADDR 0x40
#define OLED_ADDR 0x3C
#define SCR_W 128
#define SCR_H 64
#define I2C_SPEED 400000
#define INA_REG_SHUNT 0x01
#define INA_REG_BUS 0x02

#define R_SHUNT 0.010f
#define DIV_HI 100000.0f
#define DIV_LO 100000.0f
#define R_FIX 10000.0f
#define R_NTC25 10000.0f
#define BETA 3950.0f
#define VREF 3.3f

#define V_STOP_LI 3.00f
#define V_STOP_NI 0.90f
#define I_TRIP 2.00f
#define T_TRIP 65.0f
#define DMIN 0.10f

#define SAMPLE_MS 250
#define SCREEN_MS 300
#define DEBOUNCE_MS 30

Adafruit_SSD1306 oled(SCR_W, SCR_H, &Wire, -1);

enum CellKind { CHEM_LI = 0, CHEM_NI = 1 };
CellKind cellType = CHEM_LI;

bool oledUp = false;
bool inaUp = false;
bool running = false;
bool tripped = false;
String faultWhy = "OK";

uint32_t runStartMs = 0;
uint32_t lastSampleAt = 0;
uint32_t lastScreenAt = 0;
uint32_t lastTickAt = 0;

float voltsNow = 0.0f;
float ampsNow = 0.0f;
float wattsNow = 0.0f;
float degNow = NAN;
float mahAcc = 0.0f;
float whAcc = 0.0f;
float rEst = NAN;

bool vadcOk = false;
bool tadcOk = false;
bool readOk = false;

float lastVolts = NAN;
float lastAmps = NAN;
uint32_t lastAgo = 0;

struct Key {
  uint8_t pin;
  bool oldRaw;
  bool held;
  uint32_t wiggleMs;
  bool tapped;
};

Key btnStart{START_PIN, true, true, 0, false};
Key btnStop{STOP_PIN, true, true, 0, false};
Key btnChem{CHEM_PIN, true, true, 0, false};

bool regRead16(uint8_t addr, uint8_t which, uint16_t &out) {
  Wire.beginTransmission(addr);
  Wire.write(which);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(static_cast<int>(addr), 2) != 2) return false;
  const uint8_t hi = Wire.read();
  const uint8_t lo = Wire.read();
  out = (static_cast<uint16_t>(hi) << 8) | lo;
  return true;
}

bool wakeIna() {
  uint16_t v1 = 0;
  uint16_t v2 = 0;
  return regRead16(INA_ADDR, INA_REG_BUS, v1) && regRead16(INA_ADDR, INA_REG_SHUNT, v2);
}

bool readIna(float &vbus, float &iout) {
  uint16_t rawBus = 0;
  uint16_t rawShunt = 0;
  if (!regRead16(INA_ADDR, INA_REG_BUS, rawBus)) return false;
  if (!regRead16(INA_ADDR, INA_REG_SHUNT, rawShunt)) return false;

  const int16_t signedShunt = static_cast<int16_t>(rawShunt);
  vbus = static_cast<float>(rawBus) * 0.00125f;
  const float vAcrossShunt = static_cast<float>(signedShunt) * 0.0000025f;
  iout = vAcrossShunt / R_SHUNT;
  return true;
}

float readVolts(bool &good) {
  uint32_t sum = 0;
  const uint8_t times = 12;
  for (uint8_t i = 0; i < times; ++i) {
    sum += analogReadMilliVolts(ADC_VBAT_PIN);
    delayMicroseconds(250);
  }
  const float atPin = (static_cast<float>(sum) / times) / 1000.0f;
  const float step = (DIV_HI + DIV_LO) / DIV_LO;
  const float vBat = atPin * step;
  good = isfinite(vBat) && atPin >= 0.005f && atPin < 3.25f;
  return vBat;
}

float readTemp(bool &good) {
  uint32_t sum = 0;
  const uint8_t times = 12;
  for (uint8_t i = 0; i < times; ++i) {
    sum += analogReadMilliVolts(ADC_TEMP_PIN);
    delayMicroseconds(250);
  }
  const float vmid = (static_cast<float>(sum) / times) / 1000.0f;
  if (!isfinite(vmid) || vmid <= 0.02f || vmid >= (VREF - 0.02f)) {
    good = false;
    return NAN;
  }

  const float ntcR = R_FIX * vmid / (VREF - vmid);
  const float roomK = 25.0f + 273.15f;
  const float invT = (1.0f / roomK) + (log(ntcR / R_NTC25) / BETA);
  if (ntcR <= 0.0f || invT <= 0.0f || !isfinite(invT)) {
    good = false;
    return NAN;
  }
  good = true;
  return (1.0f / invT) - 273.15f;
}

void checkKey(Key &k, uint32_t t) {
  k.tapped = false;
  const bool level = digitalRead(k.pin);
  if (level != k.oldRaw) {
    k.oldRaw = level;
    k.wiggleMs = t;
  }
  if ((t - k.wiggleMs) >= DEBOUNCE_MS && level != k.held) {
    const bool before = k.held;
    k.held = level;
    if (before && !k.held) k.tapped = true;
  }
}

const char *cellLabel() {
  return cellType == CHEM_LI ? "Li-ion" : "NiMH/NiCd";
}

float stopVoltage() {
  return cellType == CHEM_LI ? V_STOP_LI : V_STOP_NI;
}

void killLoad() {
  digitalWrite(LOAD_PIN, LOW);
}

void trip(const char *why) {
  tripped = true;
  faultWhy = why;
  running = false;
  killLoad();
}

void beginRun() {
  if (tripped) return;
  running = true;
  runStartMs = millis();
  lastTickAt = runStartMs;
  mahAcc = 0.0f;
  whAcc = 0.0f;
  rEst = NAN;
  lastVolts = NAN;
  lastAmps = NAN;
  lastAgo = 0;
  killLoad();
}

void endRun() {
  running = false;
  killLoad();
}

void serviceKeys(uint32_t t) {
  checkKey(btnStart, t);
  checkKey(btnStop, t);
  checkKey(btnChem, t);

  if (btnStop.tapped) {
    endRun();
  }
  if (btnChem.tapped && !running && !tripped) {
    cellType = (cellType == CHEM_LI) ? CHEM_NI : CHEM_LI;
  }
  if (btnStart.tapped && !running && !tripped) {
    beginRun();
  }
}

void takeSample(uint32_t t) {
  float busV = 0.0f;
  float inaI = 0.0f;
  inaUp = readIna(busV, inaI);

  const float adcV = readVolts(vadcOk);
  degNow = readTemp(tadcOk);

  if (inaUp && busV > 0.05f) {
    voltsNow = busV;
  } else if (vadcOk) {
    voltsNow = adcV;
  } else {
    voltsNow = 0.0f;
  }

  ampsNow = inaUp ? inaI : 0.0f;
  wattsNow = voltsNow * ampsNow;
  readOk = inaUp && (vadcOk || (busV > 0.05f));

  if (running && isfinite(lastVolts) && isfinite(lastAmps)) {
    const float dI = ampsNow - lastAmps;
    const float dV = voltsNow - lastVolts;
    if (fabsf(dI) >= DMIN && (dI * dV) < 0.0f) {
      const float guess = fabsf(dV / dI);
      if (isfinite(guess) && guess < 2.0f) rEst = guess;
    }
  }

  if (running) {
    const float dt = static_cast<float>(t - lastTickAt) / 1000.0f;
    if (dt > 0.0f && dt < 5.0f && ampsNow > 0.0f && voltsNow > 0.0f) {
      mahAcc += ampsNow * dt * (1000.0f / 3600.0f);
      whAcc += voltsNow * ampsNow * dt / 3600.0f;
    }
    lastTickAt = t;

    if (!inaUp) {
      trip("INA226_ERROR");
    } else if (!tadcOk) {
      trip("NTC_ERROR");
    } else if (voltsNow < stopVoltage()) {
      trip("LOW_VOLTAGE");
    } else if (fabsf(ampsNow) > I_TRIP) {
      trip("OVERCURRENT_SW");
    } else if (degNow >= T_TRIP) {
      trip("OVERTEMP_SW");
    } else if (ampsNow < -0.05f) {
      trip("CURRENT_POLARITY");
    }
  }

  lastVolts = voltsNow;
  lastAmps = ampsNow;
  lastAgo = t;
}

void paintScreen(uint32_t t) {
  if (!oledUp) return;
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println(F("CELLCHECK PRO"));
  oled.drawFastHLine(0, 9, SCR_W, SSD1306_WHITE);

  oled.setCursor(0, 12);
  oled.print(cellLabel());
  oled.print(F(" "));
  if (tripped) oled.print(F("FAULT"));
  else if (running) oled.print(F("RECORDING"));
  else oled.print(F("IDLE"));

  oled.setCursor(0, 22);
  oled.print(F("V: "));
  oled.print(voltsNow, 3);
  oled.print(F(" V"));

  oled.setCursor(0, 32);
  oled.print(F("I: "));
  oled.print(ampsNow, 3);
  oled.print(F(" A  P:"));
  oled.print(wattsNow, 2);
  oled.print(F("W"));

  oled.setCursor(0, 42);
  oled.print(F("Q:"));
  oled.print(mahAcc, 1);
  oled.print(F("mAh E:"));
  oled.print(whAcc, 3);
  oled.print(F("Wh"));

  oled.setCursor(0, 52);
  oled.print(F("T:"));
  if (tadcOk && isfinite(degNow)) oled.print(degNow, 1);
  else oled.print(F("--"));
  oled.print(F("C"));
  if (isfinite(rEst)) {
    oled.print(F(" R:"));
    oled.print(rEst * 1000.0f, 1);
    oled.print(F("m"));
  } else {
    oled.print(F(" R:--"));
  }

  if (tripped) {
    oled.setCursor(0, 62);
    oled.print(faultWhy.substring(0, 20));
  }
  oled.display();

  digitalWrite(LED_RUN, running ? HIGH : LOW);
  digitalWrite(LED_ERR, tripped ? HIGH : LOW);
}

void csvHeader() {
  Serial.println(F("ms,elapsed_s,chemistry,mode,voltage_V,current_A,power_W,capacity_mAh,energy_Wh,temperature_C,r_est_ohm,status"));
}

void csvLine(uint32_t t) {
  const float ranFor = running ? static_cast<float>(t - runStartMs) / 1000.0f : 0.0f;
  const char *state = tripped ? "FAULT" : (running ? "MONITOR_ONLY" : "IDLE");

  Serial.print(t);
  Serial.print(','); Serial.print(ranFor, 3);
  Serial.print(','); Serial.print(cellType == CHEM_LI ? "LIION" : "NIMH_NICD");
  Serial.print(','); Serial.print(state);
  Serial.print(','); Serial.print(voltsNow, 4);
  Serial.print(','); Serial.print(ampsNow, 5);
  Serial.print(','); Serial.print(wattsNow, 5);
  Serial.print(','); Serial.print(mahAcc, 4);
  Serial.print(','); Serial.print(whAcc, 5);
  Serial.print(',');
  if (tadcOk && isfinite(degNow)) Serial.print(degNow, 2);
  Serial.print(',');
  if (isfinite(rEst)) Serial.print(rEst, 6);
  Serial.print(',');
  if (tripped) Serial.println(faultWhy);
  else if (!inaUp) Serial.println(F("INA226_ERROR"));
  else if (!tadcOk) Serial.println(F("NTC_ERROR"));
  else Serial.println(F("OK"));
}

void setup() {
  pinMode(LOAD_PIN, OUTPUT);
  killLoad();

  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(STOP_PIN, INPUT_PULLUP);
  pinMode(CHEM_PIN, INPUT_PULLUP);
  pinMode(LED_RUN, OUTPUT);
  pinMode(LED_ERR, OUTPUT);
  digitalWrite(LED_RUN, LOW);
  digitalWrite(LED_ERR, LOW);

  analogReadResolution(12);
  analogSetPinAttenuation(ADC_VBAT_PIN, ADC_11db);
  analogSetPinAttenuation(ADC_TEMP_PIN, ADC_11db);

  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN, I2C_SPEED);

  oledUp = oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  if (oledUp) {
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.println(F("CELLCHECK PRO"));
    oled.println(F("Starting monitor..."));
    oled.println(F("Load command = OFF"));
    oled.display();
  }

  inaUp = wakeIna();
  if (!inaUp) {
    faultWhy = "INA226_NOT_FOUND";
  }

  csvHeader();
  lastSampleAt = millis();
  lastScreenAt = lastSampleAt;
  lastTickAt = lastSampleAt;
  btnStart.wiggleMs = lastSampleAt;
  btnStop.wiggleMs = lastSampleAt;
  btnChem.wiggleMs = lastSampleAt;
}

void loop() {
  const uint32_t t = millis();
  serviceKeys(t);

  if (t - lastSampleAt >= SAMPLE_MS) {
    lastSampleAt = t;
    takeSample(t);
    csvLine(t);
  }

  if (t - lastScreenAt >= SCREEN_MS) {
    lastScreenAt = t;
    paintScreen(t);
  }

  if (!running || tripped) killLoad();
}
