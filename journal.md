<img width="1063" height="795" alt="image" src="https://github.com/user-attachments/assets/ffb2c896-c963-409e-b297-91249ad1bdf2" /># date:22/10
# time spent:1.25hr
## description:
before starting the project, i went through different power models and components to make sure the design could handle the operating conditions. a main part of this is involved analyzing the discharge behavior of a single 18650 Li-ion cell: i recognized that a standard linear LDO regulator would suffer from voltage dropout as the battery go to under load, failing to maintain a stable 3.3V rail. this made me to check and select a robust 3.3v buck boost regulator model capable of stepping voltage both up and down smoothly across the cell  full discharge range.i also mapped out reliable peripheral models integrating dedicated esd protection for the USB-C interface.
## images:
<img width="1137" height="863" alt="image" src="https://github.com/user-attachments/assets/71240793-cbe7-40b4-82b6-5f2269681e93" />
<img width="1452" height="847" alt="image" src="https://github.com/user-attachments/assets/820ceee7-1d94-4a73-ac21-cb86071e8493" />



-------
# date:22/10
# time spent:1.25hr
## description:
i started by building out the core foundation by getting the esp32s3 module in place, and wiring up the usbc port complete with cc resistors and esd protection to keep everything safe from static shocks.then, i started the power section. after realizing a standard LDO wouldn't cut it across a full 18650 battery discharge cycle,i then switched over to the TPS63020 buck boost regulator.now, the entire controller and power block is wired up clean, correct, and ready to use the esp32 running smoothly and next is the battery section.
## images:
<img width="702" height="731" alt="image" src="https://github.com/user-attachments/assets/ea7c656d-c113-40f1-9933-6ff6c170230b" />
<img width="950" height="703" alt="image" src="https://github.com/user-attachments/assets/77d3bc96-e079-45e8-a140-650c1e281759" />
<img width="1100" height="761" alt="image" src="https://github.com/user-attachments/assets/ff637faa-827d-4428-81a5-6afac744d90a" />


-------
# date:22/10
# time spent:1hr
## description:
ie integrated inter changeable battery holders for your 18650 and aa/aaa cells,and  routing both their positive and negative terminals through a dpdt selector switch so only one battery type stays active at a time while completely isolating the unused holder. from there i added an input fuse and a p channel MOSFET paired with a gate resistor to safeguard your circuit against accidental reverse polarity,at last i set up a precision 100k voltage divider with a filter capacitor to safely scale down the battery voltage.
## images:
<img width="1352" height="816" alt="image" src="https://github.com/user-attachments/assets/2cb8b124-3fde-4a91-aba6-4c754a50e1d2" />
<img width="1138" height="796" alt="image" src="https://github.com/user-attachments/assets/41bd3b82-3aca-4ac4-97de-2b4f872db851" />
<img width="1258" height="852" alt="image" src="https://github.com/user-attachments/assets/c94dd7f3-2121-4251-9dd2-5f30dcab4b61" />


-------
# date:23/10
# time spent:1.25hr
## description:
in the next part i built the tester the programmable electronic load designed to safely handle up to a 2A discharge.i setuped the n-channel power MOSFET and the 0.22ohm shunt resistor, managed cleanly by the LM358 op amp control loop.i also locked in the gate protection resistors,and dropped in a handy test point, and then safely tied off the unused half of the op-amp as a buffer to keep things stable.
Then,i added the precision tracking brains with the INA226 power monitor. then connected it up to the i2c bus complete with pull up resistors, set up clean kelvin connections across a dedicated 0.02ohm sense resistor, added a decoupling cap, and then in a 4 pin jst test header so you can easily plug in external debuggers
## images:
<img width="1733" height="752" alt="image" src="https://github.com/user-attachments/assets/5efe719c-22e0-4abf-9925-8afa59459f8b" />
<img width="1807" height="827" alt="image" src="https://github.com/user-attachments/assets/5358bfb4-3b58-4a54-8c20-2b766075deba" />
<img width="991" height="836" alt="image" src="https://github.com/user-attachments/assets/830a6164-3fa5-407b-a2ef-b6ef61c39190" />

-------
# date:23/10
# time spent:1.5hr
## description:
i finalized the core safety systems and the user interface for the hardware design. i built out the thermal protection and hardware safety circuit and  heatsink ntc thermistor,and lm397 hardware comparator, reference,thermal cutoff switch, load disable transistor, and cooling fan header.in the next part, i set up the user controls and indicators,and 3 tactile push buttons with pull up resistors for starting tests, stopping tests,and switching battery chemistry.
## images:
<img width="1513" height="667" alt="image" src="https://github.com/user-attachments/assets/87ae3efc-85a9-4a82-b162-5a55ad1d9df8" />
<img width="872" height="470" alt="image" src="https://github.com/user-attachments/assets/3fdde867-a962-409b-8683-29f0f6eec20d" />
<img width="907" height="806" alt="image" src="https://github.com/user-attachments/assets/586a0cac-7e9e-44d5-92ed-8673ad5ac09d" />
<img width="552" height="272" alt="image" src="https://github.com/user-attachments/assets/b59c2d14-85af-4f8d-976d-6d412a48b315" />


-------
# date:23/10
# time spent:0.75hr
## description:
i had added an oled screen so that it can display how much amount of battery is left and then i also ran thedrc and fixed 36 errors
## images:
<img width="917" height="443" alt="image" src="https://github.com/user-attachments/assets/7cc25e98-d32b-44e4-be1b-d7f5d1412445" />
<img width="1822" height="846" alt="image" src="https://github.com/user-attachments/assets/55795ab9-35c7-4688-801b-18225ea56a77" />
<img width="1773" height="835" alt="image" src="https://github.com/user-attachments/assets/da2660dc-c85b-47fd-8a86-cd3da712ed74" />


-------
# date:23/10
# time spent:1hr
## description:
i assigned the footprints for each part and then i had updated the pcb thru schematics an then i had made a layout and then i had moved the parts according to the layout and drew the edge cuts.
## images:
<img width="1642" height="888" alt="image" src="https://github.com/user-attachments/assets/6307de7b-0b46-4798-83bb-0ae7d8890c5b" />
<img width="1646" height="882" alt="image" src="https://github.com/user-attachments/assets/8f3741ab-578d-43ce-8c5f-1812c022bdbb" />
<img width="622" height="672" alt="image" src="https://github.com/user-attachments/assets/ec79537a-b287-4e6c-b486-dccea0f2c3d3" />
<img width="1017" height="726" alt="image" src="https://github.com/user-attachments/assets/2912e69d-208d-47b5-ad74-d4a48206e6d2" />


-------
# date:24/10
# time spent:1hr
## description:
after arranging the parts i had started routing and then routed the half part of the pcb.
## images:
<img width="1237" height="790" alt="image" src="https://github.com/user-attachments/assets/6ff30d12-56d7-4657-8b20-16c76be7790c" />
<img width="1237" height="790" alt="image" src="https://github.com/user-attachments/assets/eb58dfd4-302b-4630-be67-e883289d4079" />
<img width="1238" height="797" alt="image" src="https://github.com/user-attachments/assets/0254b1b4-7b67-49dc-a109-d06e386b36c0" />

-------
# date:24/10
# time spent:1.25hr
## description:
again starting from the remaining half which was left for the pcb ,i had completed routing the other half of the pcb and then filling it  by the copper zone on the 2 faces and then i ran the erc and found 20 errors and fixed them too and then i haad checked the 3d viewer and exported the pcb
## images:
<img width="810" height="783" alt="image" src="https://github.com/user-attachments/assets/31319be4-8eb2-4b0d-9676-0982fba61085" />
<img width="1040" height="787" alt="image" src="https://github.com/user-attachments/assets/2c7e91ae-b65a-4b0d-aad3-9f3a31e96c89" />
<img width="1180" height="782" alt="image" src="https://github.com/user-attachments/assets/13dd5b8a-e82e-4f54-9d25-d3d4b56baa53" />
<img width="822" height="780" alt="image" src="https://github.com/user-attachments/assets/fbedf4b4-c379-4131-ab84-4f9c291fcbe9" />
<img width="1521" height="817" alt="image" src="https://github.com/user-attachments/assets/4fdda0eb-100b-4f0b-9b96-6c9b4f9676a6" />
<img width="1596" height="877" alt="image" src="https://github.com/user-attachments/assets/fa0719e0-f809-4134-8acf-4cd9a6cd0938" />


-------
# date:24/10
# time spent:0.75hr
## description:
in wokwi simulator i had took all the parts from the wokwi docs and then i had created the diagram.json file for it and then created the circuit diaagram for it
## images:
<img width="931" height="781" alt="image" src="https://github.com/user-attachments/assets/b5997727-7d83-43d2-8d93-dbeb623c70db" />
<img width="952" height="770" alt="image" src="https://github.com/user-attachments/assets/2433d218-d899-49a4-8a86-472f36961b6d" />
<img width="938" height="746" alt="image" src="https://github.com/user-attachments/assets/03911d98-47df-444f-a40d-63976ee26374" />


-------
# date:24/10
# time spent:1hr
## description:
i had learnt how to use wokwi simulator and then i completed the firmware by learning from yt videos and also with the help of ai
## images:
<img width="947" height="823" alt="image" src="https://github.com/user-attachments/assets/57291367-bcfb-49f7-bb7d-ab81c3074953" />
<img width="950" height="762" alt="image" src="https://github.com/user-attachments/assets/dc71613e-5d18-446f-bc27-37a753c8dd1d" />
<img width="467" height="377" alt="image" src="https://github.com/user-attachments/assets/76301196-2ff8-4834-81d6-8fa542578dd3" />


-------
# date:24/10
# time spent:0.5hr
## description:
i rendered all gerber files and added them in the repo and also made a bom and also added them in repo.I also added the readme for the repo
## images:
<img width="1202" height="763" alt="image" src="https://github.com/user-attachments/assets/64e1ec30-9085-478f-9632-1cce81bbf3de" />
<img width="1906" height="730" alt="image" src="https://github.com/user-attachments/assets/3ece9d6f-38a9-46b2-95c2-335fd55d2a69" />



-------
