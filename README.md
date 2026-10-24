# CellCheck 

CellCheck is a battery discharge analyzer designed to measure battery voltage, current, power, temperature, estimated capacity, energy, and approximate internal resistance.

the planned hardware supports interchangeable holders for single 18650 Li ion cells and AA/AAA NiMH/NiCd cells. It uses an esp32-s3 controller, an oled dashboard, usb serial communication, and a protected programmable electronic load.

## Features

- battery voltage, current, power, and temperature monitoring
- SSD1306 OLED dashboard
- usb serial telemetry for PC monitoring and CSV logging
- start, stop, and batterymchemistry selection buttons
- capacity calculation in mAh
- energy calculation in Wh
- planned programmable discharge load with independent hardware shutdown
- planned support for interchangeable 18650 and AA/AAA battery holders

## PCB Setup

- collect all components according to the BOM.
- solder the components onto the pcb, starting with small SMD components and then larger parts and connectors.
- check solder joints and use a multimeter to check for short circuits.
- power the logic section using a current limited power supply and verify the 3.3v rail.
- upload the firmware to the esp32-s3.
- test the oled, buttons, LEDs, voltage sensing, and temperature sensing.
- calibrate voltage and current readings using known references.
- test the electronic load and hardware safety cutoffs with a suitable dummy load before connecting a battery.

# Battery Monitor — Bill of Materials

| Name | Purpose | Quantity | Total Cost (USD) | Link | Distributor |
|---|---|---:|---:|---|---|
| ESP32-S3-WROOM-1 (N16R8 assumed) | Main MCU (Wi-Fi/BLE) | 1 | 5.54 | [Link](https://www.lcsc.com/product-detail/C2913202.html) | LCSC |
| MCP9808T-E/MC (DFN-8) | Digital temperature sensor | 1 | 2.55 | [Link](https://www.lcsc.com/product-detail/C194710.html) | LCSC |
| 0.91 inch OLED (ER-OLEDM0.91-1W-I2C) | Display | 1 | 2.80 | [Link](https://www.buydisplay.com/i2c-white-0-91-inch-oled-display-module-128x32-arduino-raspberry-pi) | BuyDisplay |
| TPS63020DSJR | Buck-boost converter | 1 | 0.91 | [Link](https://www.lcsc.com/product-detail/C15483.html) | LCSC |
| INA226AIDGSR | Voltage/current/power monitor | 1 | 0.65 | [Link](https://www.lcsc.com/product-detail/C49851.html) | LCSC |
| 18650 holder (BH-18650-A1AJ006) | 18650 cell holder | 1 | 0.49 | [Link](https://www.lcsc.com/product-detail/C5290175.html) | LCSC |
| AA holder (BH-AA-A5BJ012) | AA cell holder | 1 | 0.46 | [Link](https://www.lcsc.com/product-detail/C5290180.html) | LCSC |
| NMOS TO-252 (Q2, Q3) | Load/discharge switching MOSFETs | 2 | 0.40 | [Link](https://www.lcsc.com/search?q=NMOS+TO-252) | LCSC |
| SMD tact switch (EVP-BF type, SW4-SW6) | User buttons | 3 | 0.45 | [Link](https://www.lcsc.com/search?q=EVPBF+tactile+switch) | LCSC |
| LM397 comparator (HGSEMI LM397M5/TR, SOT-23-5) | Comparator | 1 | 0.31 | [Link](https://www.lcsc.com/product-detail/C910849.html) | LCSC |
| DPDT slide switch (JS202011JCQN) | Mode/power switch SW3 | 1 | 0.25 | [Link](https://www.lcsc.com/search?q=JS202011JCQN) | LCSC |
| USB-C receptacle (TYPE-C-31-M-12) | USB-C port | 1 | 0.19 | [Link](https://www.lcsc.com/product-detail/C165948.html) | LCSC |
| USBLC6-2SC6 | USB ESD protection | 1 | 0.13 | [Link](https://www.lcsc.com/product-detail/C7519.html) | LCSC |
| 1.5uH inductor (Sunlord MWSA0402S) | Buck-boost inductor | 1 | 0.10 | [Link](https://www.lcsc.com/search?q=MWSA0402S-1R5) | LCSC |
| LM358 (SOIC-8) | Dual op-amp | 1 | 0.10 | [Link](https://www.lcsc.com/search?q=LM358+SOIC-8) | LCSC |
| JST XH 2-pin header (J2, J3) | Connectors | 2 | 0.08 | [Link](https://www.lcsc.com/search?q=B2B-XH-A) | LCSC |
| Tact switch 6mm THT (SW1, SW2) | User buttons | 2 | 0.07 | [Link](https://www.lcsc.com/search?q=tactile+switch+6x6+THT) | LCSC |
| PMOS SOT-23 (Q1) | Reverse-polarity/power switch | 1 | 0.05 | [Link](https://www.lcsc.com/search?q=PMOS+SOT-23) | LCSC |
| JST XH 4-pin header (B4B-XH-A) | Sensor/expansion connector | 1 | 0.05 | [Link](https://www.lcsc.com/product-detail/C144395.html) | LCSC |
| Fuse 1206 | Overcurrent protection | 1 | 0.05 | [Link](https://www.lcsc.com/search?q=1206+fuse) | LCSC |
| NTC 10k 0805 | Temperature sensing thermistor | 1 | 0.04 | [Link](https://www.lcsc.com/search?q=10k+NTC+0805) | LCSC |
| LED 0603 | Status LEDs | 2 | 0.04 | [Link](https://www.lcsc.com/search?q=0603+LED) | LCSC |
| 10uF capacitor 0603 | Bulk/decoupling capacitors | 3 | 0.04 | [Link](https://www.lcsc.com/search?q=10uF+0603+MLCC) | LCSC |
| 100nF capacitor 0603 | IC bypass capacitors | 3 | 0.01 | [Link](https://www.lcsc.com/search?q=100nF+0603+MLCC) | LCSC |
| Resistors 0603 (0.02, 0.22, 220, 1k, 2.4k, 4.7k, 5.1k, 10k, 100k) | Dividers, pull-ups, current sense | 21 | 0.14 | [Link](https://www.lcsc.com/search?q=0603+1%25+resistor) | LCSC |
| PCB (bare boards, JLCPCB minimum order of 5) | Main PCBs | 5 | 2.00 | [Link](https://jlcpcb.com/) | JLCPCB |
| PCB assembly (PCBA setup + stencil + fees, 1-board order) | SMT + through-hole assembly fees | 1 | 7.00 | [Link](https://jlcpcb.com/) | JLCPCB |

# schematic:
<img width="1172" height="806" alt="image" src="https://github.com/user-attachments/assets/3e010df0-9a82-450c-a60f-ccc15f0e6b72" />
# pcb:
<img width="1180" height="782" alt="image" src="https://github.com/user-attachments/assets/cb59863e-6611-4053-9102-40538c400a52" />
# 3d view:
<img width="1596" height="877" alt="image" src="https://github.com/user-attachments/assets/2c946a2e-aeb2-4864-bb9c-9006f2dbd55c" />
