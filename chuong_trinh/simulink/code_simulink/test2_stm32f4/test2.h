/*
 * File: test2.h
 *
 * Created with Waijung Blockset
 *
 * Real-Time Workshop code generated for Simulink model test2.
 *
 * Model version                        : 1.22
 * Real-Time Workshop file version      : 8.10 (R2016a) 10-Feb-2016
 * Real-Time Workshop file generated on : Thu Oct 30 16:00:35 2025
 * TLC version                          : 8.10 (Jan 14 2016)
 * C/C++ source code generated on       : Thu Oct 30 16:00:35 2025
 *
 * Target selection: stm32f4.tlc
 * Embedded hardware selection: ARM Compatible->ARM 10
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_test2_h_
#define RTW_HEADER_test2_h_
#include <string.h>
#ifndef test2_COMMON_INCLUDES_
# define test2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "waijung_hwdrvlib.h"
#endif                                 /* test2_COMMON_INCLUDES_ */

#include "test2_types.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T Saturation;                   /* '<Root>/Saturation' */
  real32_T RegularADC_o1;              /* '<Root>/Regular ADC' */
  real32_T RegularADC_o2;              /* '<Root>/Regular ADC' */
  real32_T RegularADC_o3;              /* '<Root>/Regular ADC' */
  real32_T Vin;                        /* '<Root>/Vin' */
  real32_T DiscreteTransferFcn;        /* '<S3>/Discrete Transfer Fcn' */
  real32_T vC;                         /* '<Root>/vC' */
  real32_T iL;                         /* '<Root>/iL' */
  real32_T DataTypeConversion;         /* '<Root>/Data Type Conversion' */
  real32_T Sum2;                       /* '<S3>/Sum2' */
  real32_T ByteUnpack_o3;              /* '<S2>/Byte Unpack' */
  real32_T ByteUnpack_o4;              /* '<S2>/Byte Unpack' */
  real32_T ByteUnpack_o5;              /* '<S2>/Byte Unpack' */
  real32_T ByteUnpack_o6;              /* '<S2>/Byte Unpack' */
  real32_T Switch;                     /* '<S2>/Switch' */
  real32_T Switch1;                    /* '<S2>/Switch1' */
  real32_T Switch2;                    /* '<S2>/Switch2' */
  real32_T Switch3;                    /* '<S2>/Switch3' */
  uint32_T DataTypeConversion1;        /* '<Root>/Data Type Conversion1' */
  uint8_T UARTRx_o1;                   /* '<Root>/UART Rx' */
  uint8_T UARTRx_o2[19];               /* '<Root>/UART Rx' */
  uint8_T ByteUnpack_o1;               /* '<S2>/Byte Unpack' */
  uint8_T ByteUnpack_o2;               /* '<S2>/Byte Unpack' */
  uint8_T ByteUnpack_o7;               /* '<S2>/Byte Unpack' */
  boolean_T Switch4;                   /* '<S2>/Switch4' */
  boolean_T LogicalOperator;           /* '<S10>/Logical Operator' */
  boolean_T LogicalOperator2;          /* '<S9>/Logical Operator2' */
} B_test2_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE;             /* '<S3>/Unit Delay' */
  real_T PrevY;                        /* '<Root>/Rate Limiter' */
  real32_T DiscreteTransferFcn_states; /* '<S3>/Discrete Transfer Fcn' */
  real32_T UnitDelay_DSTATE_m;         /* '<S2>/Unit Delay' */
  real32_T UnitDelay1_DSTATE;          /* '<S2>/Unit Delay1' */
  real32_T UnitDelay2_DSTATE;          /* '<S2>/Unit Delay2' */
  real32_T UnitDelay3_DSTATE;          /* '<S2>/Unit Delay3' */
  uint8_T Output_DSTATE;               /* '<S4>/Output' */
  boolean_T UnitDelay4_DSTATE;         /* '<S2>/Unit Delay4' */
  boolean_T ICic_DSTATE;               /* '<S10>/IC = ic' */
  boolean_T ICic_DSTATE_d;             /* '<S9>/IC = ic' */
} DW_test2_T;

/* Invariant block signals (auto storage) */
typedef struct {
  const real_T Gain;                   /* '<S3>/Gain' */
  const boolean_T DataTypeConversion1; /* '<S8>/Data Type Conversion1' */
  const boolean_T RelationalOperator1; /* '<S8>/Relational Operator1' */
  const boolean_T LogicalOperator2;    /* '<S8>/Logical Operator2' */
  const boolean_T LogicalOperator1;    /* '<S10>/Logical Operator1' */
  const boolean_T LogicalOperator1_j;  /* '<S9>/Logical Operator1' */
} ConstB_test2_T;

/* Real-time Model Data Structure */
struct tag_RTM_test2_T {
  const char_T *errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick1;
    struct {
      uint8_T TID[2];
    } TaskCounters;
  } Timing;
};

/* Block signals (auto storage) */
extern B_test2_T test2_B;

/* Block states (auto storage) */
extern DW_test2_T test2_DW;
extern const ConstB_test2_T test2_ConstB;/* constant block i/o */

/* Model entry point functions */
extern void test2_initialize(void);
extern void test2_step(void);
extern void test2_terminate(void);

/* Real-time Model object */
extern RT_MODEL_test2_T *const test2_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S8>/Logical Operator3' : Unused code path elimination
 * Block '<S9>/Logical Operator3' : Unused code path elimination
 * Block '<S10>/Logical Operator3' : Unused code path elimination
 * Block '<S15>/Data Type Duplicate' : Unused code path elimination
 * Block '<S15>/Data Type Propagation' : Unused code path elimination
 * Block '<Root>/To Workspace' : Unused code path elimination
 * Block '<Root>/To Workspace1' : Unused code path elimination
 * Block '<Root>/To Workspace2' : Unused code path elimination
 * Block '<Root>/To Workspace3' : Unused code path elimination
 * Block '<S4>/Data Type Propagation' : Unused code path elimination
 * Block '<S16>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S17>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S8>/Data Type Conversion2' : Eliminate redundant data type conversion
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
 * '<Root>' : 'test2'
 * '<S1>'   : 'test2/Bistable'
 * '<S2>'   : 'test2/PC_RX'
 * '<S3>'   : 'test2/Subsystem'
 * '<S4>'   : 'test2/cnt_100Hz'
 * '<S5>'   : 'test2/ocp'
 * '<S6>'   : 'test2/ov'
 * '<S7>'   : 'test2/uv'
 * '<S8>'   : 'test2/Bistable/Model'
 * '<S9>'   : 'test2/Bistable/Model/RESET Priority'
 * '<S10>'  : 'test2/Bistable/Model/SET  Priority'
 * '<S11>'  : 'test2/PC_RX/Compare To Constant'
 * '<S12>'  : 'test2/PC_RX/Compare To Constant1'
 * '<S13>'  : 'test2/PC_RX/Compare To Constant2'
 * '<S14>'  : 'test2/Subsystem/MATLAB Function'
 * '<S15>'  : 'test2/Subsystem/Saturation Dynamic'
 * '<S16>'  : 'test2/cnt_100Hz/Increment Real World'
 * '<S17>'  : 'test2/cnt_100Hz/Wrap To Zero'
 */
#endif                                 /* RTW_HEADER_test2_h_ */

/* [EOF] */
