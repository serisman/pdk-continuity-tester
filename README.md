# pdk-continuity-tester
A simple standalone continuity tester, based on the ultra-inexpensive (~$0.03) Padauk microcontroller and a piezo buzzer.
- Turns on a Piezo Buzzer when there is continuity detected between the Probe pin and GND.
- Uses the Comparator to test for continuity (i.e. Reference(+) > Probe(-)).
- Goes to sleep after a short duration of inactivity to save battery power (< 0.5 uA).
- Optional LED indicates device is powered on and not sleeping.

This is a port of, and based on an original project for the ATtiny85(V) by David Johnson-Davies:
- http://www.technoblogy.com/show?1YON
- https://github.com/technoblogy/continuity-tester

### Links
- [Firmware](firmware/)
- [PCB](pcb-s08/)

> ![Finished](https://github.com/serisman/pdk-continuity-tester/blob/master/img/Finished.jpg?raw=true)
> ![Top Side](https://github.com/serisman/pdk-continuity-tester/blob/master/img/Top-Side.jpg?raw=true)
> ![Bottom Side](https://github.com/serisman/pdk-continuity-tester/blob/master/img/Bottom-with-Battery.jpg?raw=true)

### Hardware Circuit

##### Schematic
(for 8-pin PFS154, PFS173, PMS150C/PMS15A, PMS152, PMS154C, PMS171B)
> ![Schematic](https://github.com/serisman/pdk-continuity-tester/blob/master/pcb-s08/output/Schematic.png?raw=true)

**Probe/Reference**:

One lead of the Probe is connected to one of the Comparator (-) pins, and the other lead is connected to GND. 
- The Probe lead is placed on the PA3 pin (Port A, Bit 3).

The Reference uses the Comparator (+) pin, with a 47 ohm resistor to GND.
- The Reference is placed on the PA4 pin (Port A, Bit 4). 

**Buzzer**:

The Buzzer is placed on the PA5 pin (Port A, Bit 5).  It is also connected to the PA6 pin, but the current firmware doesn't use this (yet).

For devices with an 11-bit PWM (i.e. PFS154, PFS173, PMS152, PMS154C), PA5 is also one of the PWM pins, enabling PWM control of the Buzzer's frequency using a Passive buzzer.

For other devices (i.e. PMS150C/PMS15A, PMS171B), the Buzzer can only be turned on/off and an Active buzzer is required.

In either case the Buzzer is used with a current sink configuration.

This means that one side of the Buzzer is connected to VDD, and the other is connected (through a current limiting resistor) to the digital pin of the IC.
- To enable the Buzzer (Passive / PWM), it is driven with an inverted 50% duty cycle at approx 2kHz.
- To enable the Buzzer (Active), it is driven LOW, meaning current will flow.
- To disable the Buzzer, it is floated HIGH, meaning no current will flow.

**LED**:

The LED is placed on the PA7 pin (Port A, Bit 7) with a current sink configuration.

This means the negative leg (or cathode) of the LED is connected to the digital pin of the IC, and the positive leg (or anode) of the LED is connected through a current limiting resistor to VDD.
- When the digital pin is LOW, current will flow through the LED and it will light up.
- When the digital pin is HIGH, no current will flow and the LED will turn off.

### Compatibility
This project is currently intended to be run (without modifications) on the PFS154, PFS173, PMS150C/PMS15A, PMS152, PMS154C, and PMS171B Padauk microcontrollers,
but it should be able to be modified to run on just about every currently known Padauk microcontroller that is supported by SDCC and the Easy PDK Programmer.
Some microcontrollers will require additional definitions in the [firmware/hal.h](firmware/hal.h) file,
and less common devices may require additional device specific include files (firmware/pdk/device/*.h) to be supplied.
