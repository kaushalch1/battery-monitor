# date:22/10
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
