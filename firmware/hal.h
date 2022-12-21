/*
  hal.h - Device specific definitions and helper methods for the pdk-continuity-tester project.

  ----------------------------------------------------------------------
  Copyright (C) 2020  serisman  <github@serisman.com>

  CC BY 4.0
  Licensed under a Creative Commons Attribution 4.0 International license:
  http://creativecommons.org/licenses/by/4.0/
*/

#ifndef __HAL_H__
#define __HAL_H__

#if !defined(PFS154) && !defined(PFS173) && !defined(PMS150C) && !defined(PMS15A) && !defined(PMS152) && !defined(PMS154C) && !defined(PMS171B)
  #error "Device not currently supported!"
#endif

// Define which pins to use
#define PULL_REF_HI_BIT   0     // 47k Reference pull-up resistor is on PA0 (Port A, Bit 0)
#define REFERENCE_BIT     4     // Reference is on PA4 (Port A, Bit 4), which is also Comp+
#define PROBE_BIT         3     // Probe is on PA3 (Port A, Bit 3), which is also Comp-
#define BUZZER_BIT        5     // Buzzer is placed on the PA5 pin (Port A, Bit 5), which is also PWM2 (Note: PWM not available on PMS150C,PMS15A,PMS171B)
//#define BUZZER_BIT      6     // Buzzer is placed on the PA6 pin (Port A, Bit 6)
#define LED_BIT           7     // LED is placed on the PA7 pin (Port A, Bit 7)

#define COMP_MINUS        GPCC_COMP_MINUS_PA3

// Helpers to initialize Buzzer PWM and turn Buzzer on/off
#if defined(PMS150C) || defined(PMS15A) || defined(PMS171B)
  inline void buzzerInitPWM() { }
  #define buzzerOn()          PA &= ~(1 << BUZZER_BIT)
  #define buzzerOff()         PA |= (1 << BUZZER_BIT)
  #warning Remember to use an Active Buzzer with PMS150C,PMS15A,PMS171B
#else
  #if defined(PWMG2CUBL)            // Full 11-bit PWM:   PFS154,PMS154C
    inline void buzzerInitPWM() {
      PWMG2CUBL = 0xC0;             // Set the PWM upper bound (255+1)
      PWMG2CUBH = 0x3F;
      PWMG2DTL = 0xE0;              // Set the PWM duty value (127 + 0.5 + 0.5) = 50%
      PWMG2DTH = 0x1F;
      PWMG2S = (uint8_t)(PWMG2S_SCALE_DIV32); // Set the PWM frequency to ~1.953 kHz
    }
    #define buzzerOn()          PWMG2C = (uint8_t)(PWMG2C_ENABLE | PWMG2C_INVERT_OUT | PWMG2C_OUT_PA5 | PWMG2C_CLK_IHRC)
    #define buzzerOff()         PWMG2C = 0x00; PA |= (1 << BUZZER_BIT)
  #elif defined(PWMGCUBL)           // Basic 11-bit PWM:  PFS173,PMS152
    inline void buzzerInitPWM() {
      PWMGCUBL = 0xC0;              // Set the PWM upper bound (255+1)
      PWMGCUBH = 0x3F;
      PWMG2DTL = 0xE0;              // Set the PWM duty value (127 + 0.5 + 0.5) = 50%
      PWMG2DTH = 0x1F;
      PWMGCLK = (uint8_t)(PWMGCLK_PWMG_ENABLE | PWMGCLK_CLK_IHRC | PWMGCLK_PRESCALE_DIV32); // Set the PWM frequency to ~1.953 kHz
    }
    #define buzzerOn()          PWMG2C = (uint8_t)(PWMG2C_INVERT_OUT | PWMG2C_OUT_PWMG2 | PWMG2C_OUT_PA5)
    #define buzzerOff()         PWMG2C = 0x00; PA |= (1 << BUZZER_BIT)
  #endif
#endif

// LED is active low (current sink), so define helpers for better readability
#define ledOn()             PA &= ~(1 << LED_BIT)
#define ledOff()            PA |= (1 << LED_BIT)

#endif //__HAL_H__
