# firmware (pdk-continuity-tester)

### Toolchain:
- The open-source [Small Device C Compiler (SDCC)](http://sdcc.sourceforge.net/)
  - Requires version 3.9.0 or newer - version 4.0.0+ preferred
- The open-source [Easy PDK Programmer](https://github.com/free-pdk/easy-pdk-programmer-software)
  - Requires version 1.3 or newer
- The open-source [pdk-includes](https://github.com/free-pdk/pdk-includes) repository
  - Integrated via local copy into the pdk/ directory
- The open-source [easy-pdk-includes](https://github.com/free-pdk/easy-pdk-includes) repository
  - Integrated via local copy into the easy-pdk/ directory
- Also requires 'make' and other common build tools

### Build Commands:
```
make clean
make build
make size (also calls build)
make program (also calls size which calls build)
```
> Note: These commands can be chained as well.  i.e. `make clean program`

### Customization:
Edit the variables at the top of the Makefile to:
- **DEVICE**: Use a different Padauk MCU (defaults to PFS154)
- **TARGET_VDD_MV**: Use a different voltage while calibrating the internal oscillator (IHRC) (defaults to 3000mV)

> Note: Both of these variables can be changed on the commandline as well.  i.e. `make DEVICE=PFS173 clean program` 

### Compatibility
This project is currently intended to be run (without modifications) on the PFS154, PFS173, PMS150C, PMS15A, PMS152, and PMS154C Padauk microcontrollers,
but it should be able to be modified to run on just about every currently known Padauk microcontroller that is supported by SDCC and the Easy PDK Programmer.
Some microcontrollers will require additional definitions in the [hal.h](hal.h) file,
and less common devices may require additional device specific include files (pdk/device/*.h) to be supplied.

### Build Stats
- Code Size: ~90-100 words (depending on the microcontroller)
- RAM usage: 2 bytes + stack
