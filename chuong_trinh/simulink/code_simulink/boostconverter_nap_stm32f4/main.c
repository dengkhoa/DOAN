/*
 * File: main.c
 *
 * Created with Waijung Blockset
 *
 * Real-Time Workshop code generated for Simulink model boostconverter_nap.
 *
 * Model version                        : 1.222
 * Real-Time Workshop file version      : 8.10 (R2016a) 10-Feb-2016
 * Real-Time Workshop file generated on : Sat Jan 10 10:25:36 2026
 * TLC version                          : 8.10 (Jan 14 2016)
 * C/C++ source code generated on       : Sat Jan 10 10:25:36 2026
 *
 * Target selection: stm32f4.tlc
 * Embedded hardware selection: ARM Compatible->Cortex - M4
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "waijung_hwdrvlib.h"
#include "boostconverter_nap.h"
#include "rtwtypes.h"                  /* MathWorks types */

/*
 * rt_Onestep is called from a timer ISR at the base sample time of 5.0E-5 sec.
 * The ISR is void SysTick_Handler(void) and is defined in stm32f4xx_it.c
 */
void rt_OneStep(void);
volatile unsigned int systick_count = 1;
static volatile unsigned int systick_active = 0;

/* Main program */
int_T main(void)
{
  /* Initialize model */
  boostconverter_nap_initialize();

  /* Continue indefinitely */
  if (SysTick_Config(SYSTICKRELOADVALUE)) {
    /* Capture error */
    while (1) ;
  }

  while (1) {
    if (systick_active != systick_count) {
      systick_active ++;
      boostconverter_nap_step();

      /* Get model outputs here */

      /* Profiler update */
    }
  }
}

void rt_OneStep(void)
{
  /* Tick counter */
  systick_count++;
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
  rt_OneStep();
}

/* [EOF] */
