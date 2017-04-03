EESchema Schematic File Version 2
LIBS:tube-board-rescue
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
LIBS:radio-clock
LIBS:tube-board-cache
EELAYER 25 0
EELAYER END
$Descr B 17000 11000
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
L HV5522PG U2
U 1 1 58E07B28
P 9850 5450
F 0 "U2" H 9450 7150 60  0000 C CNN
F 1 "HV5522PG" H 10100 3750 60  0000 C CNN
F 2 "Housings_QFP:LQFP-44_10x10mm_Pitch0.8mm" H 9850 5450 60  0001 C CNN
F 3 "" H 9850 5450 60  0001 C CNN
F 4 "DigiKey" H 9850 5450 60  0001 C CNN "Source"
	1    9850 5450
	1    0    0    -1  
$EndComp
$Comp
L HV5522PG U1
U 1 1 58E07B01
P 4650 5450
F 0 "U1" H 4250 7150 60  0000 C CNN
F 1 "HV5522PG" H 4900 3750 60  0000 C CNN
F 2 "Housings_QFP:LQFP-44_10x10mm_Pitch0.8mm" H 4650 5450 60  0001 C CNN
F 3 "" H 4650 5450 60  0001 C CNN
F 4 "DigiKey" H 4650 5450 60  0001 C CNN "Source"
	1    4650 5450
	1    0    0    -1  
$EndComp
$Comp
L IN-14 AN1
U 1 1 58E081B1
P 2300 1950
F 0 "AN1" H 1800 2150 60  0000 C CNN
F 1 "IN-14" H 3050 1700 60  0000 C CNN
F 2 "radio-clock:IN-14" H 2300 1950 60  0001 C CNN
F 3 "" H 2300 1950 60  0001 C CNN
F 4 "DigiKey" H 2300 1950 60  0001 C CNN "Source"
	1    2300 1950
	1    0    0    -1  
$EndComp
$Comp
L IN-14 AN2
U 1 1 58E081F0
P 4200 1950
F 0 "AN2" H 3700 2150 60  0000 C CNN
F 1 "IN-14" H 4950 1700 60  0000 C CNN
F 2 "radio-clock:IN-14" H 4200 1950 60  0001 C CNN
F 3 "" H 4200 1950 60  0001 C CNN
F 4 "DigiKey" H 4200 1950 60  0001 C CNN "Source"
	1    4200 1950
	1    0    0    -1  
$EndComp
$Comp
L IN-14 AN3
U 1 1 58E08243
P 7500 1950
F 0 "AN3" H 7000 2150 60  0000 C CNN
F 1 "IN-14" H 8250 1700 60  0000 C CNN
F 2 "radio-clock:IN-14" H 7500 1950 60  0001 C CNN
F 3 "" H 7500 1950 60  0001 C CNN
F 4 "DigiKey" H 7500 1950 60  0001 C CNN "Source"
	1    7500 1950
	1    0    0    -1  
$EndComp
$Comp
L IN-14 AN4
U 1 1 58E08282
P 9400 1950
F 0 "AN4" H 8900 2150 60  0000 C CNN
F 1 "IN-14" H 10150 1700 60  0000 C CNN
F 2 "radio-clock:IN-14" H 9400 1950 60  0001 C CNN
F 3 "" H 9400 1950 60  0001 C CNN
F 4 "DigiKey" H 9400 1950 60  0001 C CNN "Source"
	1    9400 1950
	1    0    0    -1  
$EndComp
$Comp
L IN-14 AN5
U 1 1 58E082B6
P 12700 1950
F 0 "AN5" H 12200 2150 60  0000 C CNN
F 1 "IN-14" H 13450 1700 60  0000 C CNN
F 2 "radio-clock:IN-14" H 12700 1950 60  0001 C CNN
F 3 "" H 12700 1950 60  0001 C CNN
F 4 "DigiKey" H 12700 1950 60  0001 C CNN "Source"
	1    12700 1950
	1    0    0    -1  
$EndComp
$Comp
L IN-14 AN6
U 1 1 58E082F3
P 14600 1950
F 0 "AN6" H 14100 2150 60  0000 C CNN
F 1 "IN-14" H 15350 1700 60  0000 C CNN
F 2 "radio-clock:IN-14" H 14600 1950 60  0001 C CNN
F 3 "" H 14600 1950 60  0001 C CNN
F 4 "DigiKey" H 14600 1950 60  0001 C CNN "Source"
	1    14600 1950
	1    0    0    -1  
$EndComp
Text Label 5450 3900 0    60   ~ 0
HV1
Text Label 5450 4000 0    60   ~ 0
HV2
Text Label 5450 4100 0    60   ~ 0
HV3
Text Label 5450 4200 0    60   ~ 0
HV4
Text Label 5450 4300 0    60   ~ 0
HV5
Text Label 5450 4400 0    60   ~ 0
HV6
Text Label 5450 4500 0    60   ~ 0
HV7
Text Label 5450 4600 0    60   ~ 0
HV8
Text Label 5450 4700 0    60   ~ 0
HV9
Text Label 5450 4800 0    60   ~ 0
HV10
Text Label 5450 4900 0    60   ~ 0
HV11
Text Label 5450 5000 0    60   ~ 0
HV12
Text Label 5450 5100 0    60   ~ 0
HV13
Text Label 5450 5200 0    60   ~ 0
HV14
Text Label 5450 5300 0    60   ~ 0
HV15
Text Label 5450 5400 0    60   ~ 0
HV16
Text Label 5450 5500 0    60   ~ 0
HV17
Text Label 5450 5600 0    60   ~ 0
HV18
Text Label 5450 5700 0    60   ~ 0
HV19
Text Label 5450 5800 0    60   ~ 0
HV20
Text Label 5450 5900 0    60   ~ 0
HV21
Text Label 5450 6000 0    60   ~ 0
HV22
Text Label 5450 6100 0    60   ~ 0
HV23
Text Label 5450 6200 0    60   ~ 0
HV24
Text Label 5450 6300 0    60   ~ 0
HV25
Text Label 5450 6400 0    60   ~ 0
HV26
Text Label 5450 6500 0    60   ~ 0
HV27
Text Label 5450 6600 0    60   ~ 0
HV28
Text Label 5450 6700 0    60   ~ 0
HV29
Text Label 5450 6800 0    60   ~ 0
HV30
Text Label 10650 3900 0    60   ~ 0
HV33
Text Label 10650 4000 0    60   ~ 0
HV34
Text Label 10650 4100 0    60   ~ 0
HV35
Text Label 10650 4200 0    60   ~ 0
HV36
Text Label 10650 4300 0    60   ~ 0
HV37
Text Label 10650 4400 0    60   ~ 0
HV38
Text Label 10650 4500 0    60   ~ 0
HV39
Text Label 10650 4600 0    60   ~ 0
HV40
Text Label 10650 4700 0    60   ~ 0
HV41
Text Label 10650 4800 0    60   ~ 0
HV42
Text Label 10650 4900 0    60   ~ 0
HV43
Text Label 10650 5000 0    60   ~ 0
HV44
Text Label 10650 5100 0    60   ~ 0
HV45
Text Label 10650 5200 0    60   ~ 0
HV46
Text Label 10650 5300 0    60   ~ 0
HV47
Text Label 10650 5400 0    60   ~ 0
HV48
Text Label 10650 5500 0    60   ~ 0
HV49
Text Label 10650 5600 0    60   ~ 0
HV50
Text Label 10650 5700 0    60   ~ 0
HV51
Text Label 10650 5800 0    60   ~ 0
HV52
Text Label 10650 5900 0    60   ~ 0
HV53
Text Label 10650 6000 0    60   ~ 0
HV54
Text Label 10650 6100 0    60   ~ 0
HV55
Text Label 10650 6200 0    60   ~ 0
HV56
Text Label 10650 6300 0    60   ~ 0
HV57
Text Label 10650 6400 0    60   ~ 0
HV58
Text Label 10650 6500 0    60   ~ 0
HV59
Text Label 10650 6600 0    60   ~ 0
HV60
Text Label 10650 6700 0    60   ~ 0
HV61
Text Label 10650 6800 0    60   ~ 0
HV62
Entry Wire Line
	5750 3900 5850 3800
Entry Wire Line
	5750 4000 5850 3900
Entry Wire Line
	5750 4100 5850 4000
Entry Wire Line
	5750 4200 5850 4100
Entry Wire Line
	5750 4300 5850 4200
Entry Wire Line
	5750 4400 5850 4300
Entry Wire Line
	5750 4500 5850 4400
Entry Wire Line
	5750 4600 5850 4500
Entry Wire Line
	5750 4700 5850 4600
Entry Wire Line
	5750 4800 5850 4700
Entry Wire Line
	5750 4900 5850 4800
Entry Wire Line
	5750 5000 5850 4900
Entry Wire Line
	5750 5100 5850 5000
Entry Wire Line
	5750 5200 5850 5100
Entry Wire Line
	5750 5300 5850 5200
Entry Wire Line
	5750 5400 5850 5300
Entry Wire Line
	5750 5500 5850 5400
Entry Wire Line
	5750 5600 5850 5500
Entry Wire Line
	5750 5700 5850 5600
Entry Wire Line
	5750 5800 5850 5700
Entry Wire Line
	5750 5900 5850 5800
Entry Wire Line
	5750 6000 5850 5900
Entry Wire Line
	5750 6100 5850 6000
Entry Wire Line
	5750 6200 5850 6100
Entry Wire Line
	5750 6300 5850 6200
Entry Wire Line
	5750 6400 5850 6300
Entry Wire Line
	5750 6500 5850 6400
Entry Wire Line
	5750 6600 5850 6500
Entry Wire Line
	5750 6700 5850 6600
Entry Wire Line
	5750 6800 5850 6700
Entry Wire Line
	10950 3900 11050 3800
Entry Wire Line
	10950 4000 11050 3900
Entry Wire Line
	10950 4100 11050 4000
Entry Wire Line
	10950 4200 11050 4100
Entry Wire Line
	10950 4300 11050 4200
Entry Wire Line
	10950 4400 11050 4300
Entry Wire Line
	10950 4500 11050 4400
Entry Wire Line
	10950 4600 11050 4500
Entry Wire Line
	10950 4700 11050 4600
Entry Wire Line
	10950 4800 11050 4700
Entry Wire Line
	10950 4900 11050 4800
Entry Wire Line
	10950 5000 11050 4900
Entry Wire Line
	10950 5100 11050 5000
Entry Wire Line
	10950 5200 11050 5100
Entry Wire Line
	10950 5300 11050 5200
Entry Wire Line
	10950 5400 11050 5300
Entry Wire Line
	10950 5500 11050 5400
Entry Wire Line
	10950 5600 11050 5500
Entry Wire Line
	10950 5700 11050 5600
Entry Wire Line
	10950 5800 11050 5700
Entry Wire Line
	10950 5900 11050 5800
Entry Wire Line
	10950 6000 11050 5900
Entry Wire Line
	10950 6100 11050 6000
Entry Wire Line
	10950 6200 11050 6100
Entry Wire Line
	10950 6300 11050 6200
Entry Wire Line
	10950 6400 11050 6300
Entry Wire Line
	10950 6500 11050 6400
Entry Wire Line
	10950 6600 11050 6500
Entry Wire Line
	10950 6700 11050 6600
Entry Wire Line
	10950 6800 11050 6700
Text Label 7250 2550 1    60   ~ 0
HV1
Text Label 7350 2550 1    60   ~ 0
HV2
Text Label 7450 2550 1    60   ~ 0
HV3
Text Label 7550 2550 1    60   ~ 0
HV4
Text Label 7650 2550 1    60   ~ 0
HV5
Text Label 2150 2550 1    60   ~ 0
HV6
Text Label 2250 2550 1    60   ~ 0
HV7
Text Label 2350 2550 1    60   ~ 0
HV8
Text Label 2450 2550 1    60   ~ 0
HV9
Text Label 2550 2550 1    60   ~ 0
HV10
Text Label 4050 2550 1    60   ~ 0
HV11
Text Label 4150 2550 1    60   ~ 0
HV12
Text Label 4250 2550 1    60   ~ 0
HV13
Text Label 4350 2550 1    60   ~ 0
HV14
Text Label 4450 2550 1    60   ~ 0
HV15
Text Label 3750 2550 1    60   ~ 0
HV18
Text Label 3850 2550 1    60   ~ 0
HV19
Text Label 3950 2550 1    60   ~ 0
HV20
Text Label 2650 2550 1    60   ~ 0
HV21
Text Label 2750 2550 1    60   ~ 0
HV22
Text Label 1850 2550 1    60   ~ 0
HV23
Text Label 1950 2550 1    60   ~ 0
HV24
Text Label 2050 2550 1    60   ~ 0
HV25
Text Label 7750 2550 1    60   ~ 0
HV28
Text Label 7850 2550 1    60   ~ 0
HV29
Text Label 7950 2550 1    60   ~ 0
HV30
$Comp
L +12V #PWR01
U 1 1 58E09D26
P 3900 3800
F 0 "#PWR01" H 3900 3650 50  0001 C CNN
F 1 "+12V" H 3900 3940 50  0000 C CNN
F 2 "" H 3900 3800 50  0001 C CNN
F 3 "" H 3900 3800 50  0001 C CNN
	1    3900 3800
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR02
U 1 1 58E09E0A
P 9100 3800
F 0 "#PWR02" H 9100 3650 50  0001 C CNN
F 1 "+12V" H 9100 3940 50  0000 C CNN
F 2 "" H 9100 3800 50  0001 C CNN
F 3 "" H 9100 3800 50  0001 C CNN
	1    9100 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 58E09F5C
P 3900 7100
F 0 "#PWR03" H 3900 6850 50  0001 C CNN
F 1 "GND" H 3900 6950 50  0000 C CNN
F 2 "" H 3900 7100 50  0001 C CNN
F 3 "" H 3900 7100 50  0001 C CNN
	1    3900 7100
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 58E110CE
P 2300 1400
F 0 "R1" V 2380 1400 50  0000 C CNN
F 1 "8.2k" V 2300 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 2230 1400 50  0001 C CNN
F 3 "" H 2300 1400 50  0001 C CNN
F 4 "DigiKey" H 2300 1400 60  0001 C CNN "Source"
F 5 "311-8.20KFRCT-ND" H 2300 1400 60  0001 C CNN "Part"
	1    2300 1400
	1    0    0    -1  
$EndComp
$Comp
L HT #PWR04
U 1 1 58E1115E
P 2300 1200
F 0 "#PWR04" H 2300 1320 50  0001 C CNN
F 1 "HT" H 2300 1290 50  0000 C CNN
F 2 "" H 2300 1200 50  0001 C CNN
F 3 "" H 2300 1200 50  0001 C CNN
	1    2300 1200
	1    0    0    -1  
$EndComp
$Comp
L HT #PWR05
U 1 1 58E11584
P 4200 1200
F 0 "#PWR05" H 4200 1320 50  0001 C CNN
F 1 "HT" H 4200 1290 50  0000 C CNN
F 2 "" H 4200 1200 50  0001 C CNN
F 3 "" H 4200 1200 50  0001 C CNN
	1    4200 1200
	1    0    0    -1  
$EndComp
$Comp
L HT #PWR06
U 1 1 58E1160F
P 7500 1200
F 0 "#PWR06" H 7500 1320 50  0001 C CNN
F 1 "HT" H 7500 1290 50  0000 C CNN
F 2 "" H 7500 1200 50  0001 C CNN
F 3 "" H 7500 1200 50  0001 C CNN
	1    7500 1200
	1    0    0    -1  
$EndComp
$Comp
L HT #PWR07
U 1 1 58E1183B
P 9400 1200
F 0 "#PWR07" H 9400 1320 50  0001 C CNN
F 1 "HT" H 9400 1290 50  0000 C CNN
F 2 "" H 9400 1200 50  0001 C CNN
F 3 "" H 9400 1200 50  0001 C CNN
	1    9400 1200
	1    0    0    -1  
$EndComp
$Comp
L HT #PWR08
U 1 1 58E11876
P 12700 1200
F 0 "#PWR08" H 12700 1320 50  0001 C CNN
F 1 "HT" H 12700 1290 50  0000 C CNN
F 2 "" H 12700 1200 50  0001 C CNN
F 3 "" H 12700 1200 50  0001 C CNN
	1    12700 1200
	1    0    0    -1  
$EndComp
$Comp
L HT #PWR09
U 1 1 58E11B11
P 14600 1200
F 0 "#PWR09" H 14600 1320 50  0001 C CNN
F 1 "HT" H 14600 1290 50  0000 C CNN
F 2 "" H 14600 1200 50  0001 C CNN
F 3 "" H 14600 1200 50  0001 C CNN
	1    14600 1200
	1    0    0    -1  
$EndComp
Entry Wire Line
	1850 2650 1950 2750
Entry Wire Line
	1950 2650 2050 2750
Entry Wire Line
	2050 2650 2150 2750
Entry Wire Line
	2150 2650 2250 2750
Entry Wire Line
	2250 2650 2350 2750
Entry Wire Line
	2350 2650 2450 2750
Entry Wire Line
	2450 2650 2550 2750
Entry Wire Line
	2550 2650 2650 2750
Entry Wire Line
	2650 2650 2750 2750
Entry Wire Line
	2750 2650 2850 2750
Entry Wire Line
	3750 2650 3850 2750
Entry Wire Line
	3850 2650 3950 2750
Entry Wire Line
	3950 2650 4050 2750
Entry Wire Line
	4050 2650 4150 2750
Entry Wire Line
	4150 2650 4250 2750
Entry Wire Line
	4250 2650 4350 2750
Entry Wire Line
	4350 2650 4450 2750
Entry Wire Line
	4450 2650 4550 2750
Entry Wire Line
	4550 2650 4650 2750
Entry Wire Line
	4650 2650 4750 2750
Entry Wire Line
	8950 2650 9050 2750
Entry Wire Line
	9050 2650 9150 2750
Entry Wire Line
	9150 2650 9250 2750
Entry Wire Line
	9250 2650 9350 2750
Entry Wire Line
	9350 2650 9450 2750
Entry Wire Line
	9450 2650 9550 2750
Entry Wire Line
	9550 2650 9650 2750
Entry Wire Line
	9650 2650 9750 2750
Entry Wire Line
	9750 2650 9850 2750
Entry Wire Line
	9850 2650 9950 2750
Entry Wire Line
	6950 2750 7050 2650
Entry Wire Line
	7050 2750 7150 2650
Entry Wire Line
	7150 2750 7250 2650
Entry Wire Line
	7250 2750 7350 2650
Entry Wire Line
	7350 2750 7450 2650
Entry Wire Line
	7450 2750 7550 2650
Entry Wire Line
	7550 2750 7650 2650
Entry Wire Line
	7650 2750 7750 2650
Entry Wire Line
	7750 2750 7850 2650
Entry Wire Line
	7850 2750 7950 2650
Entry Wire Line
	12150 2750 12250 2650
Entry Wire Line
	12250 2750 12350 2650
Entry Wire Line
	12350 2750 12450 2650
Entry Wire Line
	12450 2750 12550 2650
Entry Wire Line
	12550 2750 12650 2650
Entry Wire Line
	12650 2750 12750 2650
Entry Wire Line
	12750 2750 12850 2650
Entry Wire Line
	12850 2750 12950 2650
Entry Wire Line
	12950 2750 13050 2650
Entry Wire Line
	13050 2750 13150 2650
Entry Wire Line
	14050 2750 14150 2650
Entry Wire Line
	14150 2750 14250 2650
Entry Wire Line
	14250 2750 14350 2650
Entry Wire Line
	14350 2750 14450 2650
Entry Wire Line
	14450 2750 14550 2650
Entry Wire Line
	14550 2750 14650 2650
Entry Wire Line
	14650 2750 14750 2650
Entry Wire Line
	14750 2750 14850 2650
Entry Wire Line
	14850 2750 14950 2650
Entry Wire Line
	14950 2750 15050 2650
Text Label 5450 6900 0    60   ~ 0
HV31
Text Label 5450 7000 0    60   ~ 0
HV32
Entry Wire Line
	5750 6900 5850 6800
Entry Wire Line
	5750 7000 5850 6900
Text Label 10650 6900 0    60   ~ 0
HV63
Text Label 10650 7000 0    60   ~ 0
HV64
Entry Wire Line
	10950 6900 11050 6800
Entry Wire Line
	10950 7000 11050 6900
$Comp
L GND #PWR010
U 1 1 58E16AA0
P 9100 7100
F 0 "#PWR010" H 9100 6850 50  0001 C CNN
F 1 "GND" H 9100 6950 50  0000 C CNN
F 2 "" H 9100 7100 50  0001 C CNN
F 3 "" H 9100 7100 50  0001 C CNN
	1    9100 7100
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 58E17309
P 2900 4000
F 0 "C1" H 2925 4100 50  0000 L CNN
F 1 "0.1u" H 2925 3900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2938 3850 50  0001 C CNN
F 3 "" H 2900 4000 50  0001 C CNN
F 4 "DigiKey" H 2900 4000 60  0001 C CNN "Source"
	1    2900 4000
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR011
U 1 1 58E1736D
P 2900 3800
F 0 "#PWR011" H 2900 3650 50  0001 C CNN
F 1 "+12V" H 2900 3940 50  0000 C CNN
F 2 "" H 2900 3800 50  0001 C CNN
F 3 "" H 2900 3800 50  0001 C CNN
	1    2900 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 58E173FF
P 2900 4200
F 0 "#PWR012" H 2900 3950 50  0001 C CNN
F 1 "GND" H 2900 4050 50  0000 C CNN
F 2 "" H 2900 4200 50  0001 C CNN
F 3 "" H 2900 4200 50  0001 C CNN
	1    2900 4200
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 58E177C0
P 8000 4050
F 0 "C2" H 8025 4150 50  0000 L CNN
F 1 "0.1u" H 8025 3950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8038 3900 50  0001 C CNN
F 3 "" H 8000 4050 50  0001 C CNN
F 4 "DigiKey" H 8000 4050 60  0001 C CNN "Source"
	1    8000 4050
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR013
U 1 1 58E177C6
P 8000 3850
F 0 "#PWR013" H 8000 3700 50  0001 C CNN
F 1 "+12V" H 8000 3990 50  0000 C CNN
F 2 "" H 8000 3850 50  0001 C CNN
F 3 "" H 8000 3850 50  0001 C CNN
	1    8000 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 58E177CC
P 8000 4250
F 0 "#PWR014" H 8000 4000 50  0001 C CNN
F 1 "GND" H 8000 4100 50  0000 C CNN
F 2 "" H 8000 4250 50  0001 C CNN
F 3 "" H 8000 4250 50  0001 C CNN
	1    8000 4250
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X10 J2
U 1 1 58E17DAE
P 12400 7750
F 0 "J2" H 12400 8300 50  0000 C CNN
F 1 "CONN_01X10" V 12500 7750 50  0000 C CNN
F 2 "radio-clock:1-338728-0" H 12400 7750 50  0001 C CNN
F 3 "" H 12400 7750 50  0001 C CNN
F 4 "DigiKey" H 12400 7750 60  0001 C CNN "Source"
F 5 "A116183CT-ND" H 12400 7750 60  0001 C CNN "Part"
	1    12400 7750
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X10 J1
U 1 1 58E17E3D
P 2350 7750
F 0 "J1" H 2350 8300 50  0000 C CNN
F 1 "CONN_01X10" V 2450 7750 50  0000 C CNN
F 2 "radio-clock:1-338728-0" H 2350 7750 50  0001 C CNN
F 3 "" H 2350 7750 50  0001 C CNN
F 4 "DigiKey" H 2350 7750 60  0001 C CNN "Source"
F 5 "A116183CT-ND" H 2350 7750 60  0001 C CNN "Part"
	1    2350 7750
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 58E18543
P 12100 8300
F 0 "#PWR015" H 12100 8050 50  0001 C CNN
F 1 "GND" H 12100 8150 50  0000 C CNN
F 2 "" H 12100 8300 50  0001 C CNN
F 3 "" H 12100 8300 50  0001 C CNN
	1    12100 8300
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR016
U 1 1 58E1AAE7
P 12000 7200
F 0 "#PWR016" H 12000 7050 50  0001 C CNN
F 1 "+12V" H 12000 7340 50  0000 C CNN
F 2 "" H 12000 7200 50  0001 C CNN
F 3 "" H 12000 7200 50  0001 C CNN
	1    12000 7200
	1    0    0    -1  
$EndComp
Text Label 11650 8200 0    60   ~ 0
DATA_IN
Text Label 11650 8000 0    60   ~ 0
CLK
Text Label 11650 7600 0    60   ~ 0
BL
Text Label 11650 7800 0    60   ~ 0
LE
NoConn ~ 3950 4700
$Comp
L HT #PWR017
U 1 1 58E1EF64
P 2750 7200
F 0 "#PWR017" H 2750 7320 50  0001 C CNN
F 1 "HT" H 2750 7290 50  0000 C CNN
F 2 "" H 2750 7200 50  0001 C CNN
F 3 "" H 2750 7200 50  0001 C CNN
	1    2750 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3900 5350 3900
Wire Wire Line
	5750 4000 5350 4000
Wire Wire Line
	5750 4100 5350 4100
Wire Wire Line
	5750 4200 5350 4200
Wire Wire Line
	5750 4300 5350 4300
Wire Wire Line
	5750 4400 5350 4400
Wire Wire Line
	5750 4500 5350 4500
Wire Wire Line
	5750 4600 5350 4600
Wire Wire Line
	5750 4700 5350 4700
Wire Wire Line
	5750 4800 5350 4800
Wire Wire Line
	5750 4900 5350 4900
Wire Wire Line
	5750 5000 5350 5000
Wire Wire Line
	5750 5100 5350 5100
Wire Wire Line
	5750 5200 5350 5200
Wire Wire Line
	5750 5300 5350 5300
Wire Wire Line
	5750 5400 5350 5400
Wire Wire Line
	5750 5500 5350 5500
Wire Wire Line
	5750 5600 5350 5600
Wire Wire Line
	5750 5700 5350 5700
Wire Wire Line
	5750 5800 5350 5800
Wire Wire Line
	5750 5900 5350 5900
Wire Wire Line
	5750 6000 5350 6000
Wire Wire Line
	5750 6100 5350 6100
Wire Wire Line
	5750 6200 5350 6200
Wire Wire Line
	5750 6300 5350 6300
Wire Wire Line
	5750 6400 5350 6400
Wire Wire Line
	5750 6500 5350 6500
Wire Wire Line
	5750 6600 5350 6600
Wire Wire Line
	5750 6700 5350 6700
Wire Wire Line
	5750 6800 5350 6800
Wire Wire Line
	10950 3900 10550 3900
Wire Wire Line
	10950 4000 10550 4000
Wire Wire Line
	10950 4100 10550 4100
Wire Wire Line
	10950 4200 10550 4200
Wire Wire Line
	10950 4300 10550 4300
Wire Wire Line
	10950 4400 10550 4400
Wire Wire Line
	10950 4500 10550 4500
Wire Wire Line
	10950 4600 10550 4600
Wire Wire Line
	10950 4700 10550 4700
Wire Wire Line
	10950 4800 10550 4800
Wire Wire Line
	10950 4900 10550 4900
Wire Wire Line
	10950 5000 10550 5000
Wire Wire Line
	10950 5100 10550 5100
Wire Wire Line
	10950 5200 10550 5200
Wire Wire Line
	10950 5300 10550 5300
Wire Wire Line
	10950 5400 10550 5400
Wire Wire Line
	10950 5500 10550 5500
Wire Wire Line
	10950 5600 10550 5600
Wire Wire Line
	10950 5700 10550 5700
Wire Wire Line
	10950 5800 10550 5800
Wire Wire Line
	10950 5900 10550 5900
Wire Wire Line
	10950 6000 10550 6000
Wire Wire Line
	10950 6100 10550 6100
Wire Wire Line
	10950 6200 10550 6200
Wire Wire Line
	10950 6300 10550 6300
Wire Wire Line
	10950 6400 10550 6400
Wire Wire Line
	10950 6500 10550 6500
Wire Wire Line
	10950 6600 10550 6600
Wire Wire Line
	10950 6700 10550 6700
Wire Wire Line
	10950 6800 10550 6800
Wire Wire Line
	1850 2650 1850 2300
Wire Wire Line
	1950 2650 1950 2300
Wire Wire Line
	2050 2650 2050 2300
Wire Wire Line
	2150 2650 2150 2300
Wire Wire Line
	2250 2650 2250 2300
Wire Wire Line
	2350 2650 2350 2300
Wire Wire Line
	2450 2650 2450 2300
Wire Wire Line
	2550 2650 2550 2300
Wire Wire Line
	2650 2650 2650 2300
Wire Wire Line
	2750 2650 2750 2300
Wire Wire Line
	7050 2650 7050 2300
Wire Wire Line
	7150 2650 7150 2300
Wire Wire Line
	7250 2650 7250 2300
Wire Wire Line
	7350 2650 7350 2300
Wire Wire Line
	7450 2650 7450 2300
Wire Wire Line
	7550 2650 7550 2300
Wire Wire Line
	7650 2650 7650 2300
Wire Wire Line
	7750 2650 7750 2300
Wire Wire Line
	7850 2650 7850 2300
Wire Wire Line
	7950 2650 7950 2300
Wire Wire Line
	3750 2650 3750 2300
Wire Wire Line
	3850 2650 3850 2300
Wire Wire Line
	3950 2650 3950 2300
Wire Wire Line
	4050 2650 4050 2300
Wire Wire Line
	4150 2650 4150 2300
Wire Wire Line
	4250 2650 4250 2300
Wire Wire Line
	4350 2650 4350 2300
Wire Wire Line
	4450 2650 4450 2300
Wire Wire Line
	4550 2650 4550 2300
Wire Wire Line
	4650 2650 4650 2300
Wire Wire Line
	8950 2650 8950 2300
Wire Wire Line
	9050 2650 9050 2300
Wire Wire Line
	9150 2650 9150 2300
Wire Wire Line
	9250 2650 9250 2300
Wire Wire Line
	9350 2650 9350 2300
Wire Wire Line
	9450 2650 9450 2300
Wire Wire Line
	9550 2650 9550 2300
Wire Wire Line
	9650 2650 9650 2300
Wire Wire Line
	9750 2650 9750 2300
Wire Wire Line
	9850 2650 9850 2300
Wire Wire Line
	12250 2650 12250 2300
Wire Wire Line
	12350 2650 12350 2300
Wire Wire Line
	12450 2650 12450 2300
Wire Wire Line
	12550 2650 12550 2300
Wire Wire Line
	12650 2650 12650 2300
Wire Wire Line
	12750 2650 12750 2300
Wire Wire Line
	12850 2650 12850 2300
Wire Wire Line
	12950 2650 12950 2300
Wire Wire Line
	13050 2650 13050 2300
Wire Wire Line
	13150 2650 13150 2300
Wire Wire Line
	14150 2650 14150 2300
Wire Wire Line
	14250 2650 14250 2300
Wire Wire Line
	14350 2650 14350 2300
Wire Wire Line
	14450 2650 14450 2300
Wire Wire Line
	14550 2650 14550 2300
Wire Wire Line
	14650 2650 14650 2300
Wire Wire Line
	14750 2650 14750 2300
Wire Wire Line
	14850 2650 14850 2300
Wire Wire Line
	14950 2650 14950 2300
Wire Wire Line
	15050 2650 15050 2300
Wire Wire Line
	2300 1200 2300 1250
Wire Wire Line
	2300 1600 2300 1550
Wire Wire Line
	4200 1200 4200 1250
Wire Wire Line
	4200 1550 4200 1600
Wire Wire Line
	7500 1200 7500 1250
Wire Wire Line
	7500 1550 7500 1600
Wire Wire Line
	9400 1250 9400 1200
Wire Wire Line
	9400 1550 9400 1600
Wire Wire Line
	12700 1200 12700 1250
Wire Wire Line
	12700 1550 12700 1600
Wire Wire Line
	14600 1600 14600 1550
Wire Wire Line
	14600 1250 14600 1200
Wire Bus Line
	8950 2750 15050 2750
Wire Bus Line
	11050 2750 11100 2750
Wire Bus Line
	1850 2750 7950 2750
Wire Bus Line
	5850 2750 5850 7000
Wire Bus Line
	11050 2750 11050 7000
Wire Wire Line
	3900 3800 3900 4100
Wire Wire Line
	3900 3900 3950 3900
Wire Wire Line
	5750 6900 5350 6900
Wire Wire Line
	5350 7000 5750 7000
Wire Wire Line
	10550 6900 10950 6900
Wire Wire Line
	10950 7000 10550 7000
Wire Wire Line
	3950 7000 3900 7000
Wire Wire Line
	3900 7000 3900 7100
Wire Wire Line
	3900 4100 3950 4100
Connection ~ 3900 3900
Wire Wire Line
	9150 7000 9100 7000
Wire Wire Line
	9100 7000 9100 7100
Wire Wire Line
	9100 3800 9100 4100
Wire Wire Line
	9100 3900 9150 3900
Wire Wire Line
	9100 4100 9150 4100
Connection ~ 9100 3900
Wire Wire Line
	2900 4200 2900 4150
Wire Wire Line
	2900 3850 2900 3800
Wire Wire Line
	8000 4250 8000 4200
Wire Wire Line
	8000 3900 8000 3850
Wire Wire Line
	9150 4700 8400 4700
Wire Wire Line
	8400 4700 8400 8200
Wire Wire Line
	8400 8200 3700 8200
Wire Wire Line
	3700 8200 3700 4600
Wire Wire Line
	3700 4600 3950 4600
Wire Wire Line
	3400 4200 3400 7600
Wire Wire Line
	3950 4200 3400 4200
Wire Wire Line
	3950 4300 3500 4300
Wire Wire Line
	3500 4300 3500 7800
Wire Wire Line
	3600 4500 3600 8000
Wire Wire Line
	3600 4500 3950 4500
Wire Wire Line
	3400 7600 12200 7600
Wire Wire Line
	3500 7800 12200 7800
Wire Wire Line
	3600 8000 12200 8000
Wire Wire Line
	8800 8200 8800 4600
Wire Wire Line
	8800 4600 9150 4600
Wire Wire Line
	8500 7600 8500 4200
Wire Wire Line
	8500 4200 9150 4200
Wire Wire Line
	9150 4300 8600 4300
Wire Wire Line
	8600 4300 8600 7800
Wire Wire Line
	9150 4500 8700 4500
Wire Wire Line
	8700 4500 8700 8000
$Comp
L GND #PWR018
U 1 1 58E1F35E
P 2650 8300
F 0 "#PWR018" H 2650 8050 50  0001 C CNN
F 1 "GND" H 2650 8150 50  0000 C CNN
F 2 "" H 2650 8300 50  0001 C CNN
F 3 "" H 2650 8300 50  0001 C CNN
	1    2650 8300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 7300 2750 7300
Wire Wire Line
	2750 7200 2750 8100
Wire Wire Line
	2750 7500 2550 7500
Connection ~ 2750 7300
Wire Wire Line
	2750 7700 2550 7700
Connection ~ 2750 7500
Wire Wire Line
	2750 7900 2550 7900
Connection ~ 2750 7700
Wire Wire Line
	2750 8100 2550 8100
Connection ~ 2750 7900
Wire Wire Line
	2550 7400 2650 7400
Wire Wire Line
	2650 7400 2650 8300
Wire Wire Line
	2550 8200 2650 8200
Connection ~ 2650 8200
Wire Wire Line
	2550 8000 2650 8000
Connection ~ 2650 8000
Wire Wire Line
	2550 7800 2650 7800
Connection ~ 2650 7800
Wire Wire Line
	2550 7600 2650 7600
Connection ~ 2650 7600
Text Label 9750 2550 1    60   ~ 0
HV33
Text Label 9850 2550 1    60   ~ 0
HV34
Text Label 8950 2550 1    60   ~ 0
HV35
Text Label 9050 2550 1    60   ~ 0
HV36
Text Label 9150 2550 1    60   ~ 0
HV37
Text Label 14850 2550 1    60   ~ 0
HV38
Text Label 14950 2550 1    60   ~ 0
HV39
Text Label 15050 2550 1    60   ~ 0
HV40
Text Label 14150 2550 1    60   ~ 0
HV41
Text Label 14250 2550 1    60   ~ 0
HV42
Text Label 12950 2550 1    60   ~ 0
HV43
Text Label 13050 2550 1    60   ~ 0
HV44
Text Label 13150 2550 1    60   ~ 0
HV45
Text Label 12250 2550 1    60   ~ 0
HV46
Text Label 12350 2550 1    60   ~ 0
HV47
Text Label 12450 2550 1    60   ~ 0
HV48
Text Label 12550 2550 1    60   ~ 0
HV49
Text Label 12650 2550 1    60   ~ 0
HV50
Text Label 12750 2550 1    60   ~ 0
HV51
Text Label 12850 2550 1    60   ~ 0
HV52
Text Label 14350 2550 1    60   ~ 0
HV53
Text Label 14450 2550 1    60   ~ 0
HV54
Text Label 14550 2550 1    60   ~ 0
HV55
Text Label 14650 2550 1    60   ~ 0
HV56
Text Label 14750 2550 1    60   ~ 0
HV57
Text Label 9250 2550 1    60   ~ 0
HV60
Text Label 9350 2550 1    60   ~ 0
HV61
Text Label 9450 2550 1    60   ~ 0
HV62
$Comp
L R R2
U 1 1 58E213AC
P 4200 1400
F 0 "R2" V 4280 1400 50  0000 C CNN
F 1 "8.2k" V 4200 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 4130 1400 50  0001 C CNN
F 3 "" H 4200 1400 50  0001 C CNN
F 4 "DigiKey" H 4200 1400 60  0001 C CNN "Source"
F 5 "311-8.20KFRCT-ND" H 4200 1400 60  0001 C CNN "Part"
	1    4200 1400
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 58E213F9
P 7500 1400
F 0 "R3" V 7580 1400 50  0000 C CNN
F 1 "8.2k" V 7500 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 7430 1400 50  0001 C CNN
F 3 "" H 7500 1400 50  0001 C CNN
F 4 "DigiKey" H 7500 1400 60  0001 C CNN "Source"
F 5 "311-8.20KFRCT-ND" H 7500 1400 60  0001 C CNN "Part"
	1    7500 1400
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 58E216F6
P 9400 1400
F 0 "R4" V 9480 1400 50  0000 C CNN
F 1 "8.2k" V 9400 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 9330 1400 50  0001 C CNN
F 3 "" H 9400 1400 50  0001 C CNN
F 4 "DigiKey" H 9400 1400 60  0001 C CNN "Source"
F 5 "311-8.20KFRCT-ND" H 9400 1400 60  0001 C CNN "Part"
	1    9400 1400
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 58E21743
P 12700 1400
F 0 "R5" V 12780 1400 50  0000 C CNN
F 1 "8.2k" V 12700 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 12630 1400 50  0001 C CNN
F 3 "" H 12700 1400 50  0001 C CNN
F 4 "DigiKey" H 12700 1400 60  0001 C CNN "Source"
F 5 "311-8.20KFRCT-ND" H 12700 1400 60  0001 C CNN "Part"
	1    12700 1400
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 58E21980
P 14600 1400
F 0 "R6" V 14680 1400 50  0000 C CNN
F 1 "8.2k" V 14600 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 14530 1400 50  0001 C CNN
F 3 "" H 14600 1400 50  0001 C CNN
F 4 "DigiKey" H 14600 1400 60  0001 C CNN "Source"
F 5 "311-8.20KFRCT-ND" H 14600 1400 60  0001 C CNN "Part"
	1    14600 1400
	1    0    0    -1  
$EndComp
Connection ~ 8500 7600
Wire Wire Line
	12200 7300 12100 7300
Wire Wire Line
	12100 7300 12100 8300
Wire Wire Line
	12200 7500 12100 7500
Connection ~ 12100 7500
Wire Wire Line
	12200 7700 12100 7700
Connection ~ 12100 7700
Wire Wire Line
	12200 7900 12100 7900
Connection ~ 12100 7900
Wire Wire Line
	12200 8100 12100 8100
Connection ~ 12100 8100
Wire Wire Line
	12200 7400 12000 7400
Wire Wire Line
	12000 7400 12000 7200
Connection ~ 8600 7800
Connection ~ 8700 8000
Wire Wire Line
	8800 8200 12200 8200
Text Label 7050 2550 1    60   ~ 0
HV31
Text Label 7150 2550 1    60   ~ 0
HV32
Text Label 4550 2550 1    60   ~ 0
HV16
Text Label 4650 2550 1    60   ~ 0
HV17
Text Label 9550 2550 1    60   ~ 0
HV63
Text Label 9650 2550 1    60   ~ 0
HV64
$EndSCHEMATC
