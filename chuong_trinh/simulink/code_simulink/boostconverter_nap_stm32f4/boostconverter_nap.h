/*
 * File: boostconverter_nap.h
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

#ifndef RTW_HEADER_boostconverter_nap_h_
#define RTW_HEADER_boostconverter_nap_h_
#include <stddef.h>
#include <string.h>
#ifndef boostconverter_nap_COMMON_INCLUDES_
# define boostconverter_nap_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "waijung_hwdrvlib.h"
#endif                                 /* boostconverter_nap_COMMON_INCLUDES_ */

#include "boostconverter_nap_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T RegularADC_o1;                /* '<S1>/Regular ADC' */
  real_T RegularADC_o2;                /* '<S1>/Regular ADC' */
  real_T RegularADC_o3;                /* '<S1>/Regular ADC' */
  real_T Gain;                         /* '<S5>/Gain' */
  real32_T UARTRx_o2;                  /* '<S3>/UART Rx' */
  real32_T UARTRx_o3;                  /* '<S3>/UART Rx' */
  real32_T UARTRx_o4;                  /* '<S3>/UART Rx' */
  real32_T UARTRx_o5;                  /* '<S3>/UART Rx' */
  real32_T DataTypeConversion;         /* '<S4>/Data Type Conversion' */
  real32_T DataTypeConversion1;        /* '<S4>/Data Type Conversion1' */
  real32_T DataTypeConversion2;        /* '<S4>/Data Type Conversion2' */
  real32_T DataTypeConversion3;        /* '<S4>/Data Type Conversion3' */
  real32_T DataTypeConversion4;        /* '<S4>/Data Type Conversion4' */
  real32_T DataTypeConversion5;        /* '<S4>/Data Type Conversion5' */
  real32_T DataTypeConversion6;        /* '<S4>/Data Type Conversion6' */
  uint8_T UARTRx_o1;                   /* '<S3>/UART Rx' */
} B_boostconverter_nap_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Vd_cmd;                       /* '<Root>/Data Store Memory' */
  real_T Vin;                          /* '<Root>/Vin' */
  real_T Vout;                         /* '<Root>/Vout' */
  real_T a1;                           /* '<Root>/a1' */
  real_T a2;                           /* '<Root>/a2' */
  real_T a3;                           /* '<Root>/a3' */
  real_T duty;                         /* '<Root>/duty' */
  real_T iL;                           /* '<Root>/iL' */
} DW_boostconverter_nap_T;

/* Real-time Model Data Structure */
struct tag_RTM_boostconverter_nap_T {
  const char_T *errorStatus;
};

/* Block signals (auto storage) */
extern B_boostconverter_nap_T boostconverter_nap_B;

/* Block states (auto storage) */
extern DW_boostconverter_nap_T boostconverter_nap_DW;

/* Model entry point functions */
extern void boostconverter_nap_initialize(void);
extern void boostconverter_nap_step(void);
extern void boostconverter_nap_terminate(void);

/* Real-time Model object */
extern RT_MODEL_boostconverter_nap_T *const boostconverter_nap_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S6>/Scope' : Unused code path elimination
 * Block '<S6>/Scope1' : Unused code path elimination
 * Block '<S6>/Scope2' : Unused code path elimination
 * Block '<S6>/Scope3' : Unused code path elimination
 * Block '<S7>/Display' : Unused code path elimination
 * Block '<S7>/Display1' : Unused code path elimination
 * Block '<S7>/Display2' : Unused code path elimination
 * Block '<S7>/Display3' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'boostconverter_nap'
 * '<S1>'   : 'boostconverter_nap/DOC_ADC'
 * '<S2>'   : 'boostconverter_nap/Function-Call Subsystem'
 * '<S3>'   : 'boostconverter_nap/NHAN_DU_LIEU'
 * '<S4>'   : 'boostconverter_nap/TRUYEN_DU_LIEU'
 * '<S5>'   : 'boostconverter_nap/Function-Call Subsystem/PBC'
 * '<S6>'   : 'boostconverter_nap/Function-Call Subsystem/PBC/Control'
 * '<S7>'   : 'boostconverter_nap/NHAN_DU_LIEU/Enabled Subsystem'
 */
#endif                                 /* RTW_HEADER_boostconverter_nap_h_ */

/* [EOF] */
