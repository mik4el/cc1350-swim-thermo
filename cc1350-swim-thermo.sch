EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:cc1350
LIBS:switches
LIBS:cc1350-swim-thermo-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CC1350F128RSM U3
U 1 1 58BC6632
P 4150 2350
F 0 "U3" H 4150 2450 60  0000 C CNN
F 1 "CC1350F128RSM" H 4200 2300 60  0000 C CNN
F 2 "cc1350:CC1350F128RSM" H 4400 2050 60  0001 C CNN
F 3 "" H 4400 2050 60  0001 C CNN
	1    4150 2350
	1    0    0    -1  
$EndComp
$Comp
L XMSSJE3G0PA-003 U2
U 1 1 58BEE092
P 3900 5100
F 0 "U2" H 3900 5650 60  0000 C CNN
F 1 "XMSSJE3G0PA-003" H 3900 4500 60  0000 C CNN
F 2 "cc1350:XMSSJE3G0PA-003" H 3750 4700 60  0001 C CNN
F 3 "" H 3750 4700 60  0001 C CNN
	1    3900 5100
	0    1    1    0   
$EndComp
$Comp
L TLC5973 U5
U 1 1 58C057CB
P 9400 3300
F 0 "U5" H 9400 3600 60  0000 C CNN
F 1 "TLC5973" H 9400 3000 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 9400 3300 60  0001 C CNN
F 3 "" H 9400 3300 60  0001 C CNN
	1    9400 3300
	1    0    0    -1  
$EndComp
$Comp
L MCP16251 U6
U 1 1 58C05985
P 9650 5450
F 0 "U6" H 9650 5700 60  0000 C CNN
F 1 "MCP16251" H 9650 5200 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6" H 9650 5450 60  0001 C CNN
F 3 "" H 9650 5450 60  0001 C CNN
	1    9650 5450
	1    0    0    -1  
$EndComp
$Comp
L TLV713 U4
U 1 1 58C05C42
P 8750 1900
F 0 "U4" H 8750 2150 60  0000 C CNN
F 1 "TLV713" H 8750 1650 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 8750 1700 60  0001 C CNN
F 3 "" H 8750 1700 60  0001 C CNN
	1    8750 1900
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 58C3E755
P 4400 4100
F 0 "C9" H 4425 4200 50  0000 L CNN
F 1 "12pF" H 4425 4000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4438 3950 50  0001 C CNN
F 3 "" H 4400 4100 50  0000 C CNN
	1    4400 4100
	0    -1   -1   0   
$EndComp
$Comp
L C C8
U 1 1 58C3E78D
P 4400 3750
F 0 "C8" H 4425 3850 50  0000 L CNN
F 1 "12pF" H 4425 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4438 3600 50  0001 C CNN
F 3 "" H 4400 3750 50  0000 C CNN
	1    4400 3750
	0    -1   -1   0   
$EndComp
$Comp
L C C4
U 1 1 58C3E7BB
P 3750 4250
F 0 "C4" H 3775 4350 50  0000 L CNN
F 1 "12pF" H 3775 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3788 4100 50  0001 C CNN
F 3 "" H 3750 4250 50  0000 C CNN
	1    3750 4250
	-1   0    0    1   
$EndComp
$Comp
L C C5
U 1 1 58C3E7ED
P 4050 4250
F 0 "C5" H 4075 4350 50  0000 L CNN
F 1 "12pF" H 4075 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4088 4100 50  0001 C CNN
F 3 "" H 4050 4250 50  0000 C CNN
	1    4050 4250
	-1   0    0    1   
$EndComp
$Comp
L L L2
U 1 1 58C3E84C
P 4100 3900
F 0 "L2" V 4050 3900 50  0000 C CNN
F 1 "10nH" V 4175 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" H 4100 3900 50  0001 C CNN
F 3 "" H 4100 3900 50  0000 C CNN
	1    4100 3900
	0    -1   -1   0   
$EndComp
$Comp
L L L1
U 1 1 58C3E8ED
P 4000 3650
F 0 "L1" V 3950 3650 50  0000 C CNN
F 1 "10nH" V 4075 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" H 4000 3650 50  0001 C CNN
F 3 "" H 4000 3650 50  0000 C CNN
	1    4000 3650
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR01
U 1 1 58C3EC2D
P 4650 3750
F 0 "#PWR01" H 4650 3500 50  0001 C CNN
F 1 "GND" H 4650 3600 50  0000 C CNN
F 2 "" H 4650 3750 50  0000 C CNN
F 3 "" H 4650 3750 50  0000 C CNN
	1    4650 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 58C3EC65
P 4650 4100
F 0 "#PWR02" H 4650 3850 50  0001 C CNN
F 1 "GND" H 4650 3950 50  0000 C CNN
F 2 "" H 4650 4100 50  0000 C CNN
F 3 "" H 4650 4100 50  0000 C CNN
	1    4650 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 58C3F435
P 4050 3200
F 0 "#PWR03" H 4050 2950 50  0001 C CNN
F 1 "GND" H 4050 3050 50  0000 C CNN
F 2 "" H 4050 3200 50  0000 C CNN
F 3 "" H 4050 3200 50  0000 C CNN
	1    4050 3200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 58C3F69D
P 3150 4500
F 0 "#PWR04" H 3150 4250 50  0001 C CNN
F 1 "GND" H 3150 4350 50  0000 C CNN
F 2 "" H 3150 4500 50  0000 C CNN
F 3 "" H 3150 4500 50  0000 C CNN
	1    3150 4500
	1    0    0    -1  
$EndComp
Text Label 5300 3150 1    60   ~ 0
DIO1_RF_SW
Text Label 4500 4500 0    60   ~ 0
DIO1_RF_SW
$Comp
L Crystal Y2
U 1 1 58C3FC84
P 5250 3500
F 0 "Y2" H 5250 3650 50  0000 C CNN
F 1 "32.768kHz" H 5250 3350 50  0000 C CNN
F 2 "cc1350:FC-135" H 5250 3500 50  0001 C CNN
F 3 "" H 5250 3500 50  0000 C CNN
	1    5250 3500
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 58C3FE11
P 5000 3750
F 0 "C13" H 4800 3850 50  0000 L CNN
F 1 "12pF" H 5025 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5038 3600 50  0001 C CNN
F 3 "" H 5000 3750 50  0000 C CNN
	1    5000 3750
	1    0    0    -1  
$EndComp
$Comp
L C C15
U 1 1 58C3FE5B
P 5500 3750
F 0 "C15" H 5525 3850 50  0000 L CNN
F 1 "12pF" H 5525 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5538 3600 50  0001 C CNN
F 3 "" H 5500 3750 50  0000 C CNN
	1    5500 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 58C3FEDB
P 5000 4000
F 0 "#PWR05" H 5000 3750 50  0001 C CNN
F 1 "GND" H 5000 3850 50  0000 C CNN
F 2 "" H 5000 4000 50  0000 C CNN
F 3 "" H 5000 4000 50  0000 C CNN
	1    5000 4000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 58C3FF1C
P 5500 4000
F 0 "#PWR06" H 5500 3750 50  0001 C CNN
F 1 "GND" H 5500 3850 50  0000 C CNN
F 2 "" H 5500 4000 50  0000 C CNN
F 3 "" H 5500 4000 50  0000 C CNN
	1    5500 4000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 58C400ED
P 4450 3200
F 0 "#PWR07" H 4450 2950 50  0001 C CNN
F 1 "GND" H 4450 3050 50  0000 C CNN
F 2 "" H 4450 3200 50  0000 C CNN
F 3 "" H 4450 3200 50  0000 C CNN
	1    4450 3200
	1    0    0    -1  
$EndComp
$Comp
L 2450BM14G0011T U1
U 1 1 58C40E34
P 3350 6650
F 0 "U1" H 3350 6650 60  0000 C CNN
F 1 "2450BM14G0011T" H 3350 5800 60  0000 C CNN
F 2 "cc1350:2450BM14G011T" H 3150 6650 60  0001 C CNN
F 3 "" H 3150 6650 60  0001 C CNN
	1    3350 6650
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR08
U 1 1 58C41246
P 2950 5900
F 0 "#PWR08" H 2950 5650 50  0001 C CNN
F 1 "GND" H 2950 5750 50  0000 C CNN
F 2 "" H 2950 5900 50  0000 C CNN
F 3 "" H 2950 5900 50  0000 C CNN
	1    2950 5900
	1    0    0    -1  
$EndComp
$Comp
L C C20
U 1 1 58C4193B
P 5150 6700
F 0 "C20" H 5175 6800 50  0000 L CNN
F 1 "100pF" V 5000 6600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 5188 6550 50  0001 C CNN
F 3 "" H 5150 6700 50  0000 C CNN
	1    5150 6700
	0    1    1    0   
$EndComp
$Comp
L BNC P3
U 1 1 58C41999
P 5550 6700
F 0 "P3" H 5560 6820 50  0000 C CNN
F 1 "SMA FEMALE" V 5660 6640 50  0000 C CNN
F 2 "cc1350:SMA_EdgeMount_Horizontal" H 5550 6700 50  0001 C CNN
F 3 "" H 5550 6700 50  0000 C CNN
	1    5550 6700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 58C41B8A
P 5550 7000
F 0 "#PWR09" H 5550 6750 50  0001 C CNN
F 1 "GND" H 5550 6850 50  0000 C CNN
F 2 "" H 5550 7000 50  0000 C CNN
F 3 "" H 5550 7000 50  0000 C CNN
	1    5550 7000
	1    0    0    -1  
$EndComp
NoConn ~ 5300 12700
$Comp
L C C1
U 1 1 58C5AF5D
P 2600 2150
F 0 "C1" H 2650 2050 50  0000 L CNN
F 1 "100nF" H 2650 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2638 2000 50  0001 C CNN
F 3 "" H 2600 2150 50  0000 C CNN
	1    2600 2150
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR010
U 1 1 58C5B08E
P 2350 2150
F 0 "#PWR010" H 2350 1900 50  0001 C CNN
F 1 "GND" H 2350 2000 50  0000 C CNN
F 2 "" H 2350 2150 50  0000 C CNN
F 3 "" H 2350 2150 50  0000 C CNN
	1    2350 2150
	0    1    1    0   
$EndComp
Text Label 2850 2050 1    60   ~ 0
VDDS
$Comp
L GND #PWR011
U 1 1 58C5B802
P 3150 2350
F 0 "#PWR011" H 3150 2100 50  0001 C CNN
F 1 "GND" H 3150 2200 50  0000 C CNN
F 2 "" H 3150 2350 50  0000 C CNN
F 3 "" H 3150 2350 50  0000 C CNN
	1    3150 2350
	0    1    1    0   
$EndComp
$Comp
L C C17
U 1 1 58C5BB31
P 5850 2550
F 0 "C17" H 5875 2650 50  0000 L CNN
F 1 "100nF" H 5875 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5888 2400 50  0001 C CNN
F 3 "" H 5850 2550 50  0000 C CNN
	1    5850 2550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR012
U 1 1 58C5BC65
P 6100 2550
F 0 "#PWR012" H 6100 2300 50  0001 C CNN
F 1 "GND" H 6100 2400 50  0000 C CNN
F 2 "" H 6100 2550 50  0000 C CNN
F 3 "" H 6100 2550 50  0000 C CNN
	1    6100 2550
	0    -1   -1   0   
$EndComp
Text Label 5600 2850 1    60   ~ 0
VDDS
$Comp
L C C11
U 1 1 58C5C292
P 4600 850
F 0 "C11" V 4650 650 50  0000 L CNN
F 1 "100nF" V 4650 900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4638 700 50  0001 C CNN
F 3 "" H 4600 850 50  0000 C CNN
	1    4600 850 
	0    1    1    0   
$EndComp
$Comp
L C C6
U 1 1 58C5C309
P 4100 850
F 0 "C6" V 4150 900 50  0000 L CNN
F 1 "22uF" V 4150 600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4138 700 50  0001 C CNN
F 3 "" H 4100 850 50  0000 C CNN
	1    4100 850 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR013
U 1 1 58C5C583
P 3850 850
F 0 "#PWR013" H 3850 600 50  0001 C CNN
F 1 "GND" H 3850 700 50  0000 C CNN
F 2 "" H 3850 850 50  0000 C CNN
F 3 "" H 3850 850 50  0000 C CNN
	1    3850 850 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR014
U 1 1 58C5C70D
P 4850 850
F 0 "#PWR014" H 4850 600 50  0001 C CNN
F 1 "GND" H 4850 700 50  0000 C CNN
F 2 "" H 4850 850 50  0000 C CNN
F 3 "" H 4850 850 50  0000 C CNN
	1    4850 850 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR015
U 1 1 58C5C991
P 4250 1350
F 0 "#PWR015" H 4250 1100 50  0001 C CNN
F 1 "GND" H 4250 1200 50  0000 C CNN
F 2 "" H 4250 1350 50  0000 C CNN
F 3 "" H 4250 1350 50  0000 C CNN
	1    4250 1350
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR016
U 1 1 58C5CA91
P 4550 1350
F 0 "#PWR016" H 4550 1100 50  0001 C CNN
F 1 "GND" H 4550 1200 50  0000 C CNN
F 2 "" H 4550 1350 50  0000 C CNN
F 3 "" H 4550 1350 50  0000 C CNN
	1    4550 1350
	-1   0    0    1   
$EndComp
Text Label 4450 1350 1    60   ~ 0
DCDC_SW
Text Label 4250 1100 2    60   ~ 0
VDDS
$Comp
L Ferrite_Bead L6
U 1 1 58C5CF90
P 1550 5550
F 0 "L6" V 1400 5575 50  0000 C CNN
F 1 "BLM18HE152SN1" V 1700 5550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1480 5550 50  0001 C CNN
F 3 "" H 1550 5550 50  0000 C CNN
	1    1550 5550
	0    1    1    0   
$EndComp
Text Label 1350 4500 3    60   ~ 0
DCDC_SW
$Comp
L L L7
U 1 1 58C5D2BB
P 1650 4500
F 0 "L7" V 1600 4500 50  0000 C CNN
F 1 "6.8uH" V 1725 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" H 1650 4500 50  0001 C CNN
F 3 "" H 1650 4500 50  0000 C CNN
	1    1650 4500
	0    1    1    0   
$EndComp
$Comp
L C C18
U 1 1 58C5D340
P 1900 4750
F 0 "C18" H 1925 4850 50  0000 L CNN
F 1 "22uF" H 1925 4650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1938 4600 50  0001 C CNN
F 3 "" H 1900 4750 50  0000 C CNN
	1    1900 4750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 58C5D5A7
P 1900 5000
F 0 "#PWR017" H 1900 4750 50  0001 C CNN
F 1 "GND" H 1900 4850 50  0000 C CNN
F 2 "" H 1900 5000 50  0000 C CNN
F 3 "" H 1900 5000 50  0000 C CNN
	1    1900 5000
	1    0    0    -1  
$EndComp
Text Label 2200 4500 0    60   ~ 0
VDDR
$Comp
L C C2
U 1 1 58C5DEE4
P 2600 2350
F 0 "C2" H 2625 2450 50  0000 L CNN
F 1 "100nF" H 2625 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2638 2200 50  0001 C CNN
F 3 "" H 2600 2350 50  0000 C CNN
	1    2600 2350
	0    1    1    0   
$EndComp
Text Label 2850 2450 3    60   ~ 0
VDDR
$Comp
L GND #PWR018
U 1 1 58C5DFED
P 2350 2350
F 0 "#PWR018" H 2350 2100 50  0001 C CNN
F 1 "GND" H 2350 2200 50  0000 C CNN
F 2 "" H 2350 2350 50  0000 C CNN
F 3 "" H 2350 2350 50  0000 C CNN
	1    2350 2350
	0    1    1    0   
$EndComp
$Comp
L C C3
U 1 1 58C5E3FE
P 3200 2900
F 0 "C3" H 3225 3000 50  0000 L CNN
F 1 "100nF" H 3225 2800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3238 2750 50  0001 C CNN
F 3 "" H 3200 2900 50  0000 C CNN
	1    3200 2900
	1    0    0    -1  
$EndComp
Text Label 3100 2650 3    60   ~ 0
VDDR
$Comp
L GND #PWR019
U 1 1 58C5E4F4
P 3200 3150
F 0 "#PWR019" H 3200 2900 50  0001 C CNN
F 1 "GND" H 3200 3000 50  0000 C CNN
F 2 "" H 3200 3150 50  0000 C CNN
F 3 "" H 3200 3150 50  0000 C CNN
	1    3200 3150
	1    0    0    -1  
$EndComp
$Comp
L Crystal_GND24 Y1
U 1 1 58C5E72A
P 2550 3450
F 0 "Y1" H 2300 3350 50  0000 L CNN
F 1 "24MHz" H 2150 3250 50  0000 L CNN
F 2 "cc1350:TSX-3225" H 2550 3450 50  0001 C CNN
F 3 "" H 2550 3450 50  0000 C CNN
	1    2550 3450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 58C5EAE5
P 2550 3150
F 0 "#PWR020" H 2550 2900 50  0001 C CNN
F 1 "GND" H 2550 3000 50  0000 C CNN
F 2 "" H 2550 3150 50  0000 C CNN
F 3 "" H 2550 3150 50  0000 C CNN
	1    2550 3150
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR021
U 1 1 58C5EB6B
P 2550 3750
F 0 "#PWR021" H 2550 3500 50  0001 C CNN
F 1 "GND" H 2550 3600 50  0000 C CNN
F 2 "" H 2550 3750 50  0000 C CNN
F 3 "" H 2550 3750 50  0000 C CNN
	1    2550 3750
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR022
U 1 1 58C5F515
P 1300 5550
F 0 "#PWR022" H 1300 5400 50  0001 C CNN
F 1 "VCC" H 1300 5700 50  0000 C CNN
F 2 "" H 1300 5550 50  0000 C CNN
F 3 "" H 1300 5550 50  0000 C CNN
	1    1300 5550
	1    0    0    -1  
$EndComp
$Comp
L C C16
U 1 1 58C6316D
P 5850 2350
F 0 "C16" H 5875 2450 50  0000 L CNN
F 1 "1uF" H 5875 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5888 2200 50  0001 C CNN
F 3 "" H 5850 2350 50  0000 C CNN
	1    5850 2350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR023
U 1 1 58C63218
P 6100 2350
F 0 "#PWR023" H 6100 2100 50  0001 C CNN
F 1 "GND" H 6100 2200 50  0000 C CNN
F 2 "" H 6100 2350 50  0000 C CNN
F 3 "" H 6100 2350 50  0000 C CNN
	1    6100 2350
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR024
U 1 1 58C6EB93
P 10900 4800
F 0 "#PWR024" H 10900 4650 50  0001 C CNN
F 1 "VCC" H 10900 4950 50  0000 C CNN
F 2 "" H 10900 4800 50  0000 C CNN
F 3 "" H 10900 4800 50  0000 C CNN
	1    10900 4800
	1    0    0    -1  
$EndComp
$Comp
L C C25
U 1 1 58C6EE65
P 10600 4950
F 0 "C25" H 10625 5050 50  0000 L CNN
F 1 "4.7uF" H 10625 4850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 10638 4800 50  0001 C CNN
F 3 "" H 10600 4950 50  0000 C CNN
	1    10600 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 58C6EFDA
P 10600 5200
F 0 "#PWR025" H 10600 4950 50  0001 C CNN
F 1 "GND" H 10600 5050 50  0000 C CNN
F 2 "" H 10600 5200 50  0000 C CNN
F 3 "" H 10600 5200 50  0000 C CNN
	1    10600 5200
	1    0    0    -1  
$EndComp
$Comp
L C C26
U 1 1 58C6F066
P 10600 5600
F 0 "C26" H 10625 5700 50  0000 L CNN
F 1 "10uF" H 10625 5500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 10638 5450 50  0001 C CNN
F 3 "" H 10600 5600 50  0000 C CNN
	1    10600 5600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 58C6F117
P 10600 5850
F 0 "#PWR026" H 10600 5600 50  0001 C CNN
F 1 "GND" H 10600 5700 50  0000 C CNN
F 2 "" H 10600 5850 50  0000 C CNN
F 3 "" H 10600 5850 50  0000 C CNN
	1    10600 5850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 58C6F1A6
P 9000 5450
F 0 "#PWR027" H 9000 5200 50  0001 C CNN
F 1 "GND" H 9000 5300 50  0000 C CNN
F 2 "" H 9000 5450 50  0000 C CNN
F 3 "" H 9000 5450 50  0000 C CNN
	1    9000 5450
	1    0    0    -1  
$EndComp
Text Label 9200 5900 0    60   ~ 0
PSU_ENABLE
$Comp
L L L10
U 1 1 58C6F44B
P 9700 4800
F 0 "L10" V 9650 4800 50  0000 C CNN
F 1 "4.7uH" V 9775 4800 50  0000 C CNN
F 2 "cc1350:VLS3015E" H 9700 4800 50  0001 C CNN
F 3 "" H 9700 4800 50  0000 C CNN
	1    9700 4800
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 58C6FFDB
P 10300 6000
F 0 "R6" V 10380 6000 50  0000 C CNN
F 1 "1M" V 10300 6000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 10230 6000 50  0001 C CNN
F 3 "" H 10300 6000 50  0000 C CNN
	1    10300 6000
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 58C7007F
P 10300 5600
F 0 "R5" V 10380 5600 50  0000 C CNN
F 1 "3.09M" V 10200 5600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 10230 5600 50  0001 C CNN
F 3 "" H 10300 5600 50  0000 C CNN
	1    10300 5600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 58C7062D
P 10300 6250
F 0 "#PWR028" H 10300 6000 50  0001 C CNN
F 1 "GND" H 10300 6100 50  0000 C CNN
F 2 "" H 10300 6250 50  0000 C CNN
F 3 "" H 10300 6250 50  0000 C CNN
	1    10300 6250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR029
U 1 1 58C70805
P 10900 5450
F 0 "#PWR029" H 10900 5300 50  0001 C CNN
F 1 "+5V" H 10900 5590 50  0000 C CNN
F 2 "" H 10900 5450 50  0000 C CNN
F 3 "" H 10900 5450 50  0000 C CNN
	1    10900 5450
	1    0    0    -1  
$EndComp
Text Label 6100 2950 0    60   ~ 0
PSU_ENABLE
$Comp
L R R2
U 1 1 58C711BC
P 5950 3100
F 0 "R2" V 6030 3100 50  0000 C CNN
F 1 "10K" V 5950 3100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5880 3100 50  0001 C CNN
F 3 "" H 5950 3100 50  0000 C CNN
	1    5950 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 58C71264
P 5950 3350
F 0 "#PWR030" H 5950 3100 50  0001 C CNN
F 1 "GND" H 5950 3200 50  0000 C CNN
F 2 "" H 5950 3350 50  0000 C CNN
F 3 "" H 5950 3350 50  0000 C CNN
	1    5950 3350
	1    0    0    -1  
$EndComp
Text Label 4750 3250 0    60   ~ 0
DIO0_RF_SW_PWR
NoConn ~ 9850 3450
$Comp
L +5V #PWR031
U 1 1 58C74803
P 8200 3250
F 0 "#PWR031" H 8200 3100 50  0001 C CNN
F 1 "+5V" H 8200 3390 50  0000 C CNN
F 2 "" H 8200 3250 50  0000 C CNN
F 3 "" H 8200 3250 50  0000 C CNN
	1    8200 3250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR032
U 1 1 58C7489E
P 10250 2600
F 0 "#PWR032" H 10250 2450 50  0001 C CNN
F 1 "+5V" H 10250 2740 50  0000 C CNN
F 2 "" H 10250 2600 50  0000 C CNN
F 3 "" H 10250 2600 50  0000 C CNN
	1    10250 2600
	1    0    0    -1  
$EndComp
$Comp
L C C24
U 1 1 58C74939
P 10050 2750
F 0 "C24" H 10075 2850 50  0000 L CNN
F 1 "100nF" H 10075 2650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 10088 2600 50  0001 C CNN
F 3 "" H 10050 2750 50  0000 C CNN
	1    10050 2750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 58C74B9F
P 10050 3000
F 0 "#PWR033" H 10050 2750 50  0001 C CNN
F 1 "GND" H 10050 2850 50  0000 C CNN
F 2 "" H 10050 3000 50  0000 C CNN
F 3 "" H 10050 3000 50  0000 C CNN
	1    10050 3000
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 58C75297
P 10100 3250
F 0 "R4" V 10180 3250 50  0000 C CNN
F 1 "1.5K" V 10000 3450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 10030 3250 50  0001 C CNN
F 3 "" H 10100 3250 50  0000 C CNN
	1    10100 3250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR034
U 1 1 58C75CE8
P 8950 3550
F 0 "#PWR034" H 8950 3300 50  0001 C CNN
F 1 "GND" H 8950 3400 50  0000 C CNN
F 2 "" H 8950 3550 50  0000 C CNN
F 3 "" H 8950 3550 50  0000 C CNN
	1    8950 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR035
U 1 1 58C768B2
P 10350 3250
F 0 "#PWR035" H 10350 3000 50  0001 C CNN
F 1 "GND" H 10350 3100 50  0000 C CNN
F 2 "" H 10350 3250 50  0000 C CNN
F 3 "" H 10350 3250 50  0000 C CNN
	1    10350 3250
	1    0    0    -1  
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 58C76C63
P 10600 3850
F 0 "Q1" H 10800 3900 50  0000 L CNN
F 1 "2N7002" H 10800 3800 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 10800 3950 50  0001 C CNN
F 3 "" H 10600 3850 50  0000 C CNN
	1    10600 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR036
U 1 1 58C76EB4
P 10700 4150
F 0 "#PWR036" H 10700 3900 50  0001 C CNN
F 1 "GND" H 10700 4000 50  0000 C CNN
F 2 "" H 10700 4150 50  0000 C CNN
F 3 "" H 10700 4150 50  0000 C CNN
	1    10700 4150
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR037
U 1 1 58C76F58
P 10700 3050
F 0 "#PWR037" H 10700 2900 50  0001 C CNN
F 1 "+5V" H 10700 3190 50  0000 C CNN
F 2 "" H 10700 3050 50  0000 C CNN
F 3 "" H 10700 3050 50  0000 C CNN
	1    10700 3050
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 58C77034
P 10700 3300
F 0 "R7" V 10780 3300 50  0000 C CNN
F 1 "10K" V 10700 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 10630 3300 50  0001 C CNN
F 3 "" H 10700 3300 50  0000 C CNN
	1    10700 3300
	1    0    0    -1  
$EndComp
Text Label 9700 3850 0    60   ~ 0
LED_EASYSET
Text Label 3000 2050 1    60   ~ 0
LED_EASYSET
Text Notes 10050 3550 0    60   ~ 0
For I=25mA
$Comp
L C C21
U 1 1 58C79871
P 8250 1650
F 0 "C21" H 8275 1750 50  0000 L CNN
F 1 "1uF" H 8100 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8288 1500 50  0001 C CNN
F 3 "" H 8250 1650 50  0000 C CNN
	1    8250 1650
	1    0    0    -1  
$EndComp
$Comp
L C C23
U 1 1 58C7992F
P 9250 1650
F 0 "C23" H 9275 1750 50  0000 L CNN
F 1 "100nF" H 9000 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9288 1500 50  0001 C CNN
F 3 "" H 9250 1650 50  0000 C CNN
	1    9250 1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR038
U 1 1 58C799E1
P 8250 1900
F 0 "#PWR038" H 8250 1650 50  0001 C CNN
F 1 "GND" H 8250 1750 50  0000 C CNN
F 2 "" H 8250 1900 50  0000 C CNN
F 3 "" H 8250 1900 50  0000 C CNN
	1    8250 1900
	1    0    0    -1  
$EndComp
NoConn ~ 9150 2000
$Comp
L GND #PWR039
U 1 1 58C79AC5
P 8250 1400
F 0 "#PWR039" H 8250 1150 50  0001 C CNN
F 1 "GND" H 8250 1250 50  0000 C CNN
F 2 "" H 8250 1400 50  0000 C CNN
F 3 "" H 8250 1400 50  0000 C CNN
	1    8250 1400
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR040
U 1 1 58C79B72
P 9250 1400
F 0 "#PWR040" H 9250 1150 50  0001 C CNN
F 1 "GND" H 9250 1250 50  0000 C CNN
F 2 "" H 9250 1400 50  0000 C CNN
F 3 "" H 9250 1400 50  0000 C CNN
	1    9250 1400
	-1   0    0    1   
$EndComp
$Comp
L USB_OTG P4
U 1 1 58C79CC7
P 6700 1600
F 0 "P4" H 7025 1475 50  0000 C CNN
F 1 "USB_OTG" H 6700 1800 50  0000 C CNN
F 2 "Connectors:USB_Mini-B" V 6650 1500 50  0001 C CNN
F 3 "" V 6650 1500 50  0000 C CNN
	1    6700 1600
	0    -1   -1   0   
$EndComp
$Comp
L Ferrite_Bead L9
U 1 1 58C79DB4
P 7400 1800
F 0 "L9" V 7250 1825 50  0000 C CNN
F 1 "BLM18HE152SN1" V 7550 1800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7330 1800 50  0001 C CNN
F 3 "" H 7400 1800 50  0000 C CNN
	1    7400 1800
	0    1    1    0   
$EndComp
NoConn ~ 7000 1700
NoConn ~ 7000 1600
NoConn ~ 7000 1500
$Comp
L GND #PWR041
U 1 1 58C7BBB5
P 7200 1400
F 0 "#PWR041" H 7200 1150 50  0001 C CNN
F 1 "GND" H 7200 1250 50  0000 C CNN
F 2 "" H 7200 1400 50  0000 C CNN
F 3 "" H 7200 1400 50  0000 C CNN
	1    7200 1400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR042
U 1 1 58C7C936
P 9550 1800
F 0 "#PWR042" H 9550 1650 50  0001 C CNN
F 1 "VCC" H 9550 1950 50  0000 C CNN
F 2 "" H 9550 1800 50  0000 C CNN
F 3 "" H 9550 1800 50  0000 C CNN
	1    9550 1800
	1    0    0    -1  
$EndComp
$Comp
L Battery BT1
U 1 1 58C7D930
P 10500 1700
F 0 "BT1" H 10600 1800 50  0000 L CNN
F 1 "Battery" H 10600 1700 50  0000 L CNN
F 2 "cc1350:2_AAA_batteryholder_keystone" V 10500 1760 50  0001 C CNN
F 3 "" V 10500 1760 50  0000 C CNN
	1    10500 1700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR043
U 1 1 58C7E08F
P 10500 1000
F 0 "#PWR043" H 10500 850 50  0001 C CNN
F 1 "VCC" H 10500 1150 50  0000 C CNN
F 2 "" H 10500 1000 50  0000 C CNN
F 3 "" H 10500 1000 50  0000 C CNN
	1    10500 1000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR044
U 1 1 58C7E1F1
P 10500 2000
F 0 "#PWR044" H 10500 1750 50  0001 C CNN
F 1 "GND" H 10500 1850 50  0000 C CNN
F 2 "" H 10500 2000 50  0000 C CNN
F 3 "" H 10500 2000 50  0000 C CNN
	1    10500 2000
	1    0    0    -1  
$EndComp
Text Notes 8000 1100 0    60   ~ 0
Device uses either USB or battery power. \nIf battery power no USB or LDO is mounted.
Text Label 5250 2050 0    60   ~ 0
JTAG_TDO
Text Label 5250 1950 0    60   ~ 0
JTAG_TDI
Text Label 5250 2150 0    60   ~ 0
JTAG_TCKC
Text Label 5250 2250 0    60   ~ 0
JTAG_TMSC
Text Notes 7100 4700 0    60   ~ 0
JTAG - Matching CC1350 Launchpad
Text Label 7950 5050 0    60   ~ 0
JTAG_TCKC
Text Label 7950 5150 0    60   ~ 0
JTAG_TDO
Text Label 7950 5250 0    60   ~ 0
JTAG_TDI
Text Label 7950 4950 0    60   ~ 0
JTAG_TMSC
$Comp
L CONN_02X05 P5
U 1 1 58CA0045
P 7600 5150
F 0 "P5" H 7600 5450 50  0000 C CNN
F 1 "CONN_02X05" H 7600 4850 50  0000 C CNN
F 2 "cc1350:Pin_Header_Straight_SMT_02x05_1_27mm" H 7600 3950 50  0001 C CNN
F 3 "" H 7600 3950 50  0000 C CNN
	1    7600 5150
	1    0    0    -1  
$EndComp
NoConn ~ 7350 5250
$Comp
L GND #PWR045
U 1 1 58CA0220
P 7250 5450
F 0 "#PWR045" H 7250 5200 50  0001 C CNN
F 1 "GND" H 7250 5300 50  0000 C CNN
F 2 "" H 7250 5450 50  0000 C CNN
F 3 "" H 7250 5450 50  0000 C CNN
	1    7250 5450
	1    0    0    -1  
$EndComp
Text Label 4150 1200 2    60   ~ 0
RESET_N
Text Label 8550 4900 0    60   ~ 0
VDDS
$Comp
L R R3
U 1 1 58CA3762
P 8550 5150
F 0 "R3" V 8630 5150 50  0000 C CNN
F 1 "100K" V 8550 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8480 5150 50  0001 C CNN
F 3 "" H 8550 5150 50  0000 C CNN
	1    8550 5150
	1    0    0    -1  
$EndComp
$Comp
L C C22
U 1 1 58CA3820
P 8550 5750
F 0 "C22" H 8575 5850 50  0000 L CNN
F 1 "100nF" H 8575 5650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8588 5600 50  0001 C CNN
F 3 "" H 8550 5750 50  0000 C CNN
	1    8550 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR046
U 1 1 58CA38E4
P 8550 6000
F 0 "#PWR046" H 8550 5750 50  0001 C CNN
F 1 "GND" H 8550 5850 50  0000 C CNN
F 2 "" H 8550 6000 50  0000 C CNN
F 3 "" H 8550 6000 50  0000 C CNN
	1    8550 6000
	1    0    0    -1  
$EndComp
Text Label 8400 5500 2    60   ~ 0
RESET_N
Text Notes 9400 4600 0    60   ~ 0
5V PSU (for LEDs etc)
Text Notes 9200 2850 0    60   ~ 0
LED Driver
$Comp
L Jumper_NC_Small JP1
U 1 1 58CA7320
P 10500 1300
F 0 "JP1" H 10500 1380 50  0000 C CNN
F 1 "Jumper_NC" H 10510 1240 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 10500 1300 50  0001 C CNN
F 3 "" H 10500 1300 50  0000 C CNN
	1    10500 1300
	0    1    1    0   
$EndComp
Text Notes 10350 1600 1    60   ~ 0
Remove jumper \nfor programming
Text Notes 5100 6400 0    60   ~ 0
868MHz RF section
Text Notes 2250 6300 0    60   ~ 0
BLE RF section
$Comp
L SW_Push SW1
U 1 1 58CC3CE2
P 1250 1750
F 0 "SW1" H 1300 1850 50  0000 L CNN
F 1 "SW_Push" H 1250 1690 50  0000 C CNN
F 2 "cc1350:SKSLLAE010" H 1250 1950 50  0001 C CNN
F 3 "" H 1250 1950 50  0000 C CNN
	1    1250 1750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR047
U 1 1 58CC4391
P 950 1750
F 0 "#PWR047" H 950 1500 50  0001 C CNN
F 1 "GND" H 950 1600 50  0000 C CNN
F 2 "" H 950 1750 50  0000 C CNN
F 3 "" H 950 1750 50  0000 C CNN
	1    950  1750
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 58CC4456
P 1700 1750
F 0 "R1" V 1780 1750 50  0000 C CNN
F 1 "100" V 1700 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1630 1750 50  0001 C CNN
F 3 "" H 1700 1750 50  0000 C CNN
	1    1700 1750
	0    1    1    0   
$EndComp
Text Label 1950 1750 0    60   ~ 0
SWITCH
Text Label 3150 1950 1    60   ~ 0
SWITCH
$Comp
L CONN_01X04 P1
U 1 1 58CC5410
P 1100 900
F 0 "P1" H 1100 1150 50  0000 C CNN
F 1 "LMT_70_1" V 1200 900 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-04_04x2.54mm_Straight" H 1100 900 50  0001 C CNN
F 3 "" H 1100 900 50  0000 C CNN
	1    1100 900 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR048
U 1 1 58CC7C22
P 1050 1300
F 0 "#PWR048" H 1050 1050 50  0001 C CNN
F 1 "GND" H 1050 1150 50  0000 C CNN
F 2 "" H 1050 1300 50  0000 C CNN
F 3 "" H 1050 1300 50  0000 C CNN
	1    1050 1300
	1    0    0    -1  
$EndComp
Text Label 1150 1250 0    60   ~ 0
TAO
Text Label 1350 1100 0    60   ~ 0
T_ON_1
$Comp
L CONN_01X04 P2
U 1 1 58CC91C1
P 2200 900
F 0 "P2" H 2200 1150 50  0000 C CNN
F 1 "LMT_70_2" V 2300 900 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-04_04x2.54mm_Straight" H 2200 900 50  0001 C CNN
F 3 "" H 2200 900 50  0000 C CNN
	1    2200 900 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR049
U 1 1 58CC91C7
P 2150 1300
F 0 "#PWR049" H 2150 1050 50  0001 C CNN
F 1 "GND" H 2150 1150 50  0000 C CNN
F 2 "" H 2150 1300 50  0000 C CNN
F 3 "" H 2150 1300 50  0000 C CNN
	1    2150 1300
	1    0    0    -1  
$EndComp
Text Label 2250 1250 0    60   ~ 0
TAO
Text Label 2450 1100 0    60   ~ 0
T_ON_2
Text Label 3700 1350 2    60   ~ 0
T_ON_2
Text Label 3700 1450 2    60   ~ 0
TAO
Text Label 3700 1250 2    60   ~ 0
T_ON_1
$Comp
L PWR_FLAG #FLG050
U 1 1 58CCF366
P 7700 1700
F 0 "#FLG050" H 7700 1795 50  0001 C CNN
F 1 "PWR_FLAG" H 7700 1880 50  0000 C CNN
F 2 "" H 7700 1700 50  0000 C CNN
F 3 "" H 7700 1700 50  0000 C CNN
	1    7700 1700
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG051
U 1 1 58CD2351
P 1900 5550
F 0 "#FLG051" H 1900 5645 50  0001 C CNN
F 1 "PWR_FLAG" H 1900 5730 50  0000 C CNN
F 2 "" H 1900 5550 50  0000 C CNN
F 3 "" H 1900 5550 50  0000 C CNN
	1    1900 5550
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG052
U 1 1 58CD3D84
P 10500 1950
F 0 "#FLG052" H 10500 2045 50  0001 C CNN
F 1 "PWR_FLAG" H 10500 2130 50  0000 C CNN
F 2 "" H 10500 1950 50  0000 C CNN
F 3 "" H 10500 1950 50  0000 C CNN
	1    10500 1950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3850 3150 3850 4100
Wire Wire Line
	4250 3600 4250 4100
Wire Wire Line
	4550 3750 4650 3750
Wire Wire Line
	4650 4100 4550 4100
Wire Wire Line
	3850 4100 3750 4100
Connection ~ 3850 3650
Wire Wire Line
	3750 4400 3850 4400
Wire Wire Line
	3850 4400 3850 4500
Wire Wire Line
	4050 4400 3950 4400
Wire Wire Line
	3950 4400 3950 4500
Wire Wire Line
	4050 3150 4050 3200
Connection ~ 4250 3900
Wire Wire Line
	3150 4500 3550 4500
Connection ~ 3450 4500
Wire Wire Line
	4300 4500 4500 4500
Wire Wire Line
	5500 3900 5500 4000
Wire Wire Line
	5000 3900 5000 4000
Wire Wire Line
	5500 3300 5500 3600
Wire Wire Line
	5500 3500 5400 3500
Wire Wire Line
	5000 3500 5000 3600
Connection ~ 5500 3500
Wire Wire Line
	4450 3150 4450 3200
Wire Wire Line
	4350 3150 4350 3400
Wire Wire Line
	4350 3400 5000 3400
Wire Wire Line
	4250 3150 4250 3500
Wire Wire Line
	4250 3500 5100 3500
Wire Wire Line
	3950 3150 3950 4100
Wire Wire Line
	3950 4100 4050 4100
Connection ~ 3950 3900
Wire Wire Line
	4150 3150 4150 3600
Wire Wire Line
	4150 3600 4250 3600
Connection ~ 4250 3750
Wire Wire Line
	4150 3650 4250 3650
Connection ~ 4250 3650
Connection ~ 5000 3500
Wire Wire Line
	5000 3400 5000 3300
Wire Wire Line
	5000 3300 5500 3300
Wire Wire Line
	3700 5900 3700 5700
Wire Wire Line
	2950 5900 3350 5900
Wire Wire Line
	3500 5900 3700 5900
Wire Wire Line
	3800 5700 3800 7400
Wire Wire Line
	3800 7400 3500 7400
Connection ~ 3200 5900
Wire Wire Line
	2850 7400 3200 7400
Wire Wire Line
	2850 6850 2850 7400
Wire Wire Line
	5550 6900 5550 7000
Wire Wire Line
	5400 6700 5300 6700
Wire Wire Line
	4000 5700 4000 7400
Wire Wire Line
	2750 2150 3250 2150
Wire Wire Line
	2350 2150 2450 2150
Wire Wire Line
	3150 2350 3250 2350
Wire Wire Line
	5500 2550 5700 2550
Wire Wire Line
	6000 2550 6100 2550
Wire Wire Line
	2850 2050 2850 2150
Connection ~ 2850 2150
Wire Wire Line
	4350 850  4350 1450
Wire Wire Line
	4250 850  4450 850 
Connection ~ 4350 850 
Wire Wire Line
	3950 850  3850 850 
Wire Wire Line
	4750 850  4850 850 
Wire Wire Line
	4250 1350 4250 1450
Wire Wire Line
	4550 1350 4550 1450
Wire Wire Line
	4450 1350 4450 1450
Wire Wire Line
	4250 1100 4350 1100
Connection ~ 4350 1100
Wire Wire Line
	1700 5550 2050 5550
Wire Wire Line
	1400 5550 1300 5550
Wire Wire Line
	1350 4500 1500 4500
Wire Wire Line
	1800 4500 2200 4500
Connection ~ 1900 4500
Wire Wire Line
	1900 4500 1900 4600
Wire Wire Line
	3250 2250 2850 2250
Wire Wire Line
	2850 2250 2850 2450
Wire Wire Line
	2850 2350 2750 2350
Wire Wire Line
	2450 2350 2350 2350
Connection ~ 2850 2350
Wire Wire Line
	3100 2650 3250 2650
Wire Wire Line
	3250 2450 2950 2450
Wire Wire Line
	2950 2450 2950 2850
Wire Wire Line
	2950 2850 2300 2850
Wire Wire Line
	2300 2850 2300 3450
Wire Wire Line
	2300 3450 2400 3450
Wire Wire Line
	2550 3150 2550 3250
Wire Wire Line
	2550 3650 2550 3750
Wire Wire Line
	3250 2550 3000 2550
Wire Wire Line
	3000 2550 3000 2900
Wire Wire Line
	3000 2900 2800 2900
Wire Wire Line
	2800 2900 2800 3450
Wire Wire Line
	2800 3450 2700 3450
Wire Wire Line
	5150 2450 5500 2450
Wire Wire Line
	5500 2450 5500 2550
Wire Wire Line
	5700 2350 5150 2350
Wire Wire Line
	6100 2350 6000 2350
Wire Wire Line
	9850 4800 10900 4800
Wire Wire Line
	10600 5100 10600 5200
Connection ~ 10600 4800
Wire Wire Line
	10300 4800 10300 5350
Wire Wire Line
	10300 5350 10150 5350
Wire Wire Line
	9200 5350 9200 4800
Wire Wire Line
	9200 4800 9550 4800
Connection ~ 10300 4800
Wire Wire Line
	9200 5550 9200 5900
Wire Wire Line
	9200 5450 9000 5450
Wire Wire Line
	10150 5450 10900 5450
Wire Wire Line
	10150 5550 10150 5800
Wire Wire Line
	10150 5800 10300 5800
Wire Wire Line
	10300 5750 10300 5850
Connection ~ 10300 5800
Connection ~ 10300 5450
Connection ~ 10600 5450
Wire Wire Line
	10600 5750 10600 5850
Wire Wire Line
	10300 6150 10300 6250
Wire Wire Line
	5150 2550 5450 2550
Wire Wire Line
	5450 2550 5450 2950
Wire Wire Line
	5450 2950 6100 2950
Connection ~ 5950 2950
Wire Wire Line
	5950 3250 5950 3350
Wire Wire Line
	4550 3150 4550 3250
Wire Wire Line
	4550 3250 4750 3250
Wire Wire Line
	9850 2600 10250 2600
Wire Wire Line
	10050 2900 10050 3000
Wire Wire Line
	9850 3150 9850 2600
Connection ~ 10050 2600
Wire Wire Line
	9950 3250 9850 3250
Wire Wire Line
	8700 3250 8950 3250
Wire Wire Line
	8950 3350 8700 3350
Wire Wire Line
	8700 3350 8700 3450
Wire Wire Line
	8950 3150 8700 3150
Wire Wire Line
	8700 3150 8700 3050
Wire Wire Line
	8950 3450 8950 3550
Wire Wire Line
	8300 3250 8200 3250
Wire Wire Line
	10250 3250 10350 3250
Wire Wire Line
	10700 3050 10700 3150
Wire Wire Line
	10700 3450 10700 3650
Wire Wire Line
	10700 4050 10700 4150
Wire Wire Line
	9700 3850 10400 3850
Wire Wire Line
	9950 3550 10700 3550
Wire Wire Line
	9950 3350 9950 3550
Wire Wire Line
	9950 3350 9850 3350
Connection ~ 10700 3550
Wire Wire Line
	7000 1400 7200 1400
Wire Wire Line
	6600 1200 7100 1200
Wire Wire Line
	7100 1200 7100 1400
Wire Wire Line
	7000 1800 7250 1800
Wire Wire Line
	7550 1800 8350 1800
Connection ~ 8250 1800
Wire Wire Line
	8250 1400 8250 1500
Wire Wire Line
	9250 1400 9250 1500
Wire Wire Line
	9150 1800 9550 1800
Connection ~ 9250 1800
Wire Wire Line
	8350 1900 8250 1900
Wire Wire Line
	8350 2000 8350 2200
Wire Wire Line
	8350 2200 8100 2200
Wire Wire Line
	8100 2200 8100 1800
Connection ~ 8100 1800
Connection ~ 7100 1400
Wire Wire Line
	5150 3150 5300 3150
Wire Wire Line
	5150 2650 5150 3150
Wire Wire Line
	3250 2050 3000 2050
Wire Wire Line
	5150 2150 5250 2150
Wire Wire Line
	5150 2050 5250 2050
Wire Wire Line
	5150 1950 5250 1950
Wire Wire Line
	5250 2250 5150 2250
Connection ~ 5600 2550
Wire Wire Line
	5600 2850 5600 2550
Wire Wire Line
	7350 4950 7250 4950
Wire Wire Line
	7350 5050 7250 5050
Wire Wire Line
	7250 5050 7250 5450
Wire Wire Line
	7250 5350 7350 5350
Wire Wire Line
	7250 5150 7350 5150
Connection ~ 7250 5150
Connection ~ 7250 5350
Wire Wire Line
	7850 5350 8550 5350
Wire Wire Line
	7850 5250 7950 5250
Wire Wire Line
	7950 5150 7850 5150
Wire Wire Line
	7950 5050 7850 5050
Wire Wire Line
	7850 4950 7950 4950
Wire Wire Line
	4150 1200 4150 1450
Wire Wire Line
	8550 4900 8550 5000
Wire Wire Line
	8550 5300 8550 5600
Wire Wire Line
	8550 5900 8550 6000
Connection ~ 8550 5350
Wire Wire Line
	8400 5500 8550 5500
Connection ~ 8550 5500
Wire Wire Line
	10500 1400 10500 1500
Wire Wire Line
	950  1750 1050 1750
Wire Wire Line
	1450 1750 1550 1750
Wire Wire Line
	1950 1750 1850 1750
Wire Wire Line
	3250 1950 3150 1950
Wire Wire Line
	950  1100 950  1300
Wire Wire Line
	950  1300 850  1300
Wire Wire Line
	1050 1300 1050 1100
Wire Wire Line
	1150 1100 1150 1250
Wire Wire Line
	1250 1100 1350 1100
Wire Wire Line
	2050 1100 2050 1300
Wire Wire Line
	2050 1300 1950 1300
Wire Wire Line
	2150 1300 2150 1100
Wire Wire Line
	2250 1100 2250 1250
Wire Wire Line
	2350 1100 2450 1100
Wire Wire Line
	3950 1350 3950 1450
Wire Wire Line
	4050 1250 4050 1450
Wire Wire Line
	3700 1450 3850 1450
Wire Wire Line
	3950 1350 3700 1350
Wire Wire Line
	3700 1250 4050 1250
Wire Wire Line
	7700 1700 7700 1800
Connection ~ 7700 1800
Connection ~ 1900 5550
Wire Wire Line
	1900 4900 1900 5000
Wire Wire Line
	10500 1900 10500 2000
Connection ~ 10500 1950
Wire Wire Line
	10500 1000 10500 1200
Wire Wire Line
	3200 2750 3200 2650
Connection ~ 3200 2650
Wire Wire Line
	3200 3050 3200 3150
$Comp
L PWR_FLAG #FLG053
U 1 1 58CD8AB4
P 2050 4500
F 0 "#FLG053" H 2050 4595 50  0001 C CNN
F 1 "PWR_FLAG" H 2050 4680 50  0000 C CNN
F 2 "" H 2050 4500 50  0000 C CNN
F 3 "" H 2050 4500 50  0000 C CNN
	1    2050 4500
	1    0    0    -1  
$EndComp
Connection ~ 2050 4500
Text Label 4500 5700 0    60   ~ 0
DIO0_RF_SW_PWR
Wire Wire Line
	4300 5700 4500 5700
$Comp
L Antenna AE1
U 1 1 58CED65E
P 2850 6650
F 0 "AE1" H 2775 6725 50  0000 R CNN
F 1 "Antenna" H 2775 6650 50  0000 R CNN
F 2 "cc1350:2450AT45A100" H 2850 6650 50  0001 C CNN
F 3 "" H 2850 6650 50  0001 C CNN
	1    2850 6650
	1    0    0    -1  
$EndComp
$Comp
L LFB18868MBG9E212 U7
U 1 1 58D42D77
P 4450 6650
F 0 "U7" H 4450 6650 60  0000 C CNN
F 1 "LFB18868MBG9E212" H 4450 5800 60  0000 C CNN
F 2 "cc1350:LFB18868MBG9E212" H 4250 6650 60  0001 C CNN
F 3 "" H 4250 6650 60  0001 C CNN
	1    4450 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 5900 4100 5900
Wire Wire Line
	4100 5900 4100 5700
Wire Wire Line
	4000 7400 4300 7400
Wire Wire Line
	4450 7400 5000 7400
$Comp
L GND #PWR054
U 1 1 58D43397
P 5000 7400
F 0 "#PWR054" H 5000 7150 50  0001 C CNN
F 1 "GND" H 5000 7250 50  0000 C CNN
F 2 "" H 5000 7400 50  0000 C CNN
F 3 "" H 5000 7400 50  0000 C CNN
	1    5000 7400
	1    0    0    -1  
$EndComp
Connection ~ 4600 7400
$Comp
L GND #PWR055
U 1 1 58D43708
P 4950 5800
F 0 "#PWR055" H 4950 5550 50  0001 C CNN
F 1 "GND" H 4950 5650 50  0000 C CNN
F 2 "" H 4950 5800 50  0000 C CNN
F 3 "" H 4950 5800 50  0000 C CNN
	1    4950 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 5800 4450 5800
Wire Wire Line
	4450 5800 4450 5900
Wire Wire Line
	4600 5900 4800 5900
Wire Wire Line
	4800 5900 4800 6700
Wire Wire Line
	4800 6700 5000 6700
$Comp
L GND #PWR056
U 1 1 58D96477
P 3350 7550
F 0 "#PWR056" H 3350 7300 50  0001 C CNN
F 1 "GND" H 3350 7400 50  0000 C CNN
F 2 "" H 3350 7550 50  0000 C CNN
F 3 "" H 3350 7550 50  0000 C CNN
	1    3350 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 7400 3350 7550
Text Label 2050 5550 0    60   ~ 0
VDDS
Text Label 7250 4950 2    60   ~ 0
VDDS
Text Label 850  1300 2    60   ~ 0
VDDS
Text Label 1950 1300 2    60   ~ 0
VDDS
$Comp
L LED_ARGB D1
U 1 1 58C766EA
P 8500 3250
F 0 "D1" H 8500 3620 50  0000 C CNN
F 1 "ASMB-MTB0-0A3A2" H 8500 2900 50  0000 C CNN
F 2 "cc1350:ASMB-MTB0-0A3A2" H 8500 3200 50  0001 C CNN
F 3 "" H 8500 3200 50  0000 C CNN
	1    8500 3250
	-1   0    0    1   
$EndComp
$EndSCHEMATC
