/*
  pdk-continuity-tester

  Copyright (C) 2020  serisman  <github@serisman.com>
  Based on an original project by David Johnson-Davies (http://www.technoblogy.com/show?1YON)

  Turns on a Piezo Buzzer when there is continuity detected between the Probe pin and GND.
  Uses the Comparator to test for continuity (i.e. Reference(+) > Probe(-)).
  Goes to sleep after a short duration of inactivity to save battery power.
  Optional LED indicates device is powered on and not sleeping.

  ----------------------------------------------------------------------
  CC BY 4.0
  Licensed under a Creative Commons Attribution 4.0 International license:
  http://creativecommons.org/licenses/by/4.0/
*/

#include <stdint.h>
#include <pdk/device.h>
#include <easy-pdk/calibrate.h>
#include "hal.h"

uint16_t idle_counter;

// Main program
void main() {

  // Disable wake-up on un-used pins to save power
  PADIER = 0x00;
#if defined(PBDIER)
  PBDIER = 0x00;
#endif
#if defined(PCDIER)
  PCDIER = 0x00;
#endif

  // Initialize Buzzer, LED, Probe, and Reference Pins
  PAC = ((1<<LED_BIT) | (1<<BUZZER_BIT));         // Set Buzzer and LED Pins as output
  PAPH = ((1<<REFERENCE_BIT) | (1<<PROBE_BIT));   // Enable pull-ups on Probe and Reference Pins
  ledOn();
  buzzerInit();

  // Setup/enable the Comparator for continuity testing (i.e. Reference(+) > Probe(-))
  GPCC = (uint8_t)(GPCC_COMP_ENABLE | COMP_MINUS | GPCC_COMP_PLUS_PA4);

  // Main processing loop
  while (1) {

    if ((GPCC & GPCC_COMP_RESULT_POSITIV) == GPCC_COMP_RESULT_POSITIV) {
      // Continuity found, turn Buzzer on, reset idle counter
      buzzerOn();
      idle_counter = 0xFFFF;
    } else {
      // No continuity, turn Buzzer off, go to sleep after a short delay
      buzzerOff();
      idle_counter--;
      if (idle_counter == 0) {
        ledOff();
        GPCC &= ~(GPCC_COMP_ENABLE);          // Disable Comparator to save power
        PAPH &= ~(1<<REFERENCE_BIT);          // Turn off pull-up on Reference Pin to save power
        PADIER = (1<<PROBE_BIT);              // Enable wake-up on Probe Pin
        MISC = (MISC_FAST_WAKEUP_ENABLE | MISC_LVR_DISABLE); // Disable LVR to save power

        __stopsys();                          // Go to sleep

        // Carry on here when we wake up
        MISC = 0x00;                          // Re-enable LVR
        PADIER = 0x00;
        PAPH |= (1<<REFERENCE_BIT);           // Re-enable pull-up on Reference Pin
        GPCC |= (GPCC_COMP_ENABLE);           // Re-enable Comparator
        ledOn();
      }
    }
  }
}

// Startup code - Setup/calibrate system clock
unsigned char _sdcc_external_startup(void) {

  // Set FUSE:
  // - 1.8v LVR (if available)
  // - Normal IO Drive (if available)
  // - Fast Bootup (if available)
  // - Security Off
#if defined(FUSE_LVR_1V8) && defined(FUSE_IO_DRV_NORMAL) && defined(FUSE_BOOTUP_FAST)
  PDK_SET_FUSE(FUSE_BOOTUP_FAST | FUSE_IO_DRV_NORMAL | FUSE_LVR_1V8 | FUSE_SECURITY_OFF);
#elif defined(FUSE_LVR_1V8)
  PDK_SET_FUSE(FUSE_LVR_1V8 | FUSE_SECURITY_OFF);
#elif defined(FUSE_IO_DRV_NORMAL) && defined(FUSE_BOOTUP_FAST)
  PDK_SET_FUSE(FUSE_BOOTUP_FAST | FUSE_IO_DRV_NORMAL | FUSE_SECURITY_OFF);
#elif defined(FUSE_BOOTUP_FAST)
  PDK_SET_FUSE(FUSE_BOOTUP_FAST | FUSE_SECURITY_OFF);
#else
  PDK_SET_FUSE(FUSE_SECURITY_OFF);
#endif

  // Set LVR:
  // - 1.8v (if available)
  // - otherwise 2.0v (if available)
#if defined(MISCLVR_1V8)
  MISCLVR = (uint8_t) MISCLVR_1V8;
#elif defined(MISCLVR_2V)
  MISCLVR = (uint8_t) MISCLVR_2V;
#endif

  // Initialize the system clock (CLKMD register) for 1MHz operation (IHRC/16)
  PDK_SET_SYSCLOCK(SYSCLOCK_IHRC_1MHZ);

  // Insert placeholder code to tell EasyPdkProg to calibrate the IHRC internal oscillator for 1MHz operation.
  EASY_PDK_CALIBRATE_IHRC(1000000, TARGET_VDD_MV);

  return 0;   // Return 0 to inform SDCC to continue with normal initialization.
}
