# cc1350-swim-thermo
This project tries to create a swim thermometer broadcasting sensor data and handling interaction on dual band radio (BLE and long range sub 1 GHz) using the Texas Instrument CC1350 MCU, specifically the CC1350F128RSM. Temperature sensing is done using the high precision temperature sensor LMT70. This repo contains both software and hardware implementation.

Requires Code Composer Studio (CCS) v8 for software and Kicad for hardware. Software is programmed using the CC1350 Launchpad.

## How to get a cc1350 custom board "working"
1. Follow the launchpad design carefully. I sort of did that.
1. Start with soldering only the critical components for the CC1350 on the board, i.e. decoupling caps, crystals, power rail, jtag connector etc. I didn't do that but it worked anyway but I wish I did if I would have ran in to more problems.
1. Power it up with a current limited power supply and make sure there aren't any shorts. A working board should only consume a few mA.
1. Follow the steps in initial board bring up (http://processors.wiki.ti.com/index.php/CC26xx_HW_Troubleshooting) and make sure VDDR is at 1.68v, VDDS is at power supply voltage and DCOUPL is at 1.28V. It wasn't working for me initially and my VDDR was at a few mV. I got the error message (`Error connecting to the target:
(Error -242 @ 0x0)`). I found that it was due to a bad solder so adding more solder made it work. 
1. Setup the launchpad to program the board by removing all jumpers from the launchpad. Connect the 2x5-pin JTAG connector 1.27mm, I used https://www.mouser.se/ProductDetail/485-1675. Make sure the orientation is correct on the CC1350-swim-thermo (cable pointing in to the board). 
1. Power the board with 3.3v from your supply (since the Launchpad uses 3.3v).
1. Open Code Composer Studio and test the connection to the board from the target config. It should just work.
1. Program the cc1350 with the hello world project that includes a modified board file in this repo and make sure the program runs as expected.
1. Success!

![Working hello world](https://github.com/mik4el/cc1350-swim-thermo/blob/master/working_hello_world.JPG)

If you have problems, follow these debugging steps to make sure the CC1350 and board is setup as expected:

Always a good idea to cycle power and restart CCS if a previously working board stopped working.

From https://e2e.ti.com/support/wireless_connectivity/bluetooth_low_energy/f/538/p/432936/1817780): 
```
A few debugging hints (without applying power):
- Check impedance between DCDC_SW/VDDR/VDDR_RF (should be 0)
- Check impedance betwwen all 3 VDDS pins and board supply (should be 0)
- Check impedance between DCOUPL and VDDR/VDDS/GND (should be in the mega-ohm range)
- Check capacitance values from VDDR->GND (10uF) and DCOUPL->GND(1uF).
- Check current draw (when applying power). An empty device should go into boot loader mode and draw 6-7mA.
```
