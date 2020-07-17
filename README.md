# pdk-continuity-tester
A simple standalone continuity tester, based on the ultra-inexpensive (~$0.03) Padauk microcontroller and a piezo buzzer.
- Turns on a Piezo Buzzer when there is continuity detected between the Probe pin and GND.
- Uses the Comparator to test for continuity (i.e. Reference(+) > Probe(-)).
- Goes to sleep after a short duration of inactivity to save battery power (< 0.5 uA).
- Optional LED indicates device is powered on and not sleeping.

This is a port of, and based on an original project for the ATtiny85(V) by David Johnson-Davies:
- http://www.technoblogy.com/show?1YON
- https://github.com/technoblogy/continuity-tester

### Hardware Circuit

##### Schematic for 8-pin PFS154, PFS173, PMS152, PMS154C
(Also works for 10-pin, 14-pin, and 16-pin)
> ![Schematic for PFS154-S08](https://github.com/serisman/pdk-continuity-tester/blob/master/img/Schematic-S08.png?raw=true)

##### Schematic for 6-pin PMS150C/PMS15A
> ![Schematic for PMS150C-U06](https://github.com/serisman/pdk-continuity-tester/blob/master/img/PMS150C-U06/Schematic.png?raw=true)

**Probe/Reference**:

One lead of the Probe is connected to one of the Comparator (-) pins, and the other lead is connected to GND. 
- By default, the Probe lead is placed on the PA3 pin* (Port A, Bit 3).
- For some devices (i.e. PMS150C/PMS15A), the Probe lead is placed on the PA6 pin* (Port A, Bit 6).

The Reference uses the Comparator (+) pin, with a ~50 ohm resistor to GND.
- On all devices, the Reference is placed on the PA4 pin* (Port A, Bit 4). 
 
>_*Note: Please consult the pinout for the specific microcontroller package used to identify the correct physical pin._

**Buzzer**:

For devices with an 11-bit PWM (specifically PWMG2) (i.e. PFS154/PFS173/PMS152/PMS154C), the Buzzer is placed on the PA5 pin* (Port A, Bit 5), which is also one of the PG2PWM pins.

For other devices (i.e. PMS150C/PMS15A), the 8-bit PWM output of timer 2 (TM2) is used, and the Buzzer is placed on the PA3 pin* (Port A, Bit 3), which is also the TM2PWM pin.

In either case the Buzzer is installed with a current sink configuration.

This means that one side of the Buzzer is connected to the digital pin of the IC, and the other is connected to VDD.
- To enable the Buzzer, it is driven with an inverted 50% duty cycle at approx 2kHz.
- To disable the Buzzer, it is driven with an inverted 0% duty cycle (i.e. HIGH, meaning no current will flow).

>_*Note: Please consult the pinout for the specific microcontroller package used to identify the correct physical pin._

**LED**:

By default, the LED is placed on the PA6 pin* (Port A, Bit 6) with a current sink configuration.

For some devices (i.e. PMS150C/PMS15A), the LED is placed on the PA5 pin* (Port A, Bit 5) with a current sink configuration.

This means the negative leg (or cathode) of the LED is connected to the digital pin of the IC, and the positive leg (or anode) of the LED is connected through a current limiting resistor to VDD.
- When the digital pin is LOW, current will flow through the LED and it will light up.
- When the digital pin is HIGH, no current will flow and the LED will turn off.

>_*Note: Please consult the pinout for the specific microcontroller package used to identify the correct physical pin._


### Compatibility
This project is currently intended to be run (without modifications) on the PFS154, PFS173, PMS150C, PMS15A, PMS152, and PMS154C Padauk microcontrollers,
but it should be able to be modified to run on just about every currently known Padauk microcontroller that is supported by SDCC and the Easy PDK Programmer.
Some microcontrollers will require additional definitions in the [firmware/hal.h](firmware/hal.h) file,
and less common devices may require additional device specific include files (firmware/pdk/device/*.h) to be supplied.


### PCB

A PCB design for the 6-pin PMS150C is available in the [pcb-pms150c-u06](pcb-pms150c-u06/) folder (created with KiCad) and has been [shared on OSHPARK](https://oshpark.com/shared_projects/XcWQiX7z).
This design is meant to be attached to the bottom of a ~12mm Piezo Buzzer, where the SMD parts are first soldered to the board, and are underneath the buzzer.
There are through-holes for the two probe connections, and pads on the back for the battery connections.
2x LR44 battery cells in series would pair nicely with this PCB, as they are a good voltage match, and are approximately the same diameter (meaning the whole thing can be heat shrunk together for a miniature continuity tester).

![3D Render for PMS150C-U06](https://github.com/serisman/pdk-continuity-tester/blob/master/img/PMS150C-U06/3D%20Profile%20-%20Top.png?raw=true)
![PCB Top for PMS150C-U06](https://github.com/serisman/pdk-continuity-tester/blob/master/img/PMS150C-U06/PCB%20-%20Top.png?raw=true)
![PCB Bottom for PMS150C-U06](https://github.com/serisman/pdk-continuity-tester/blob/master/img/PMS150C-U06/PCB%20-%20Bottom.png?raw=true)
