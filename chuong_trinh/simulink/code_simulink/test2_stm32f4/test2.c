/*
 * File: test2.c
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

#include "test2.h"
#include "test2_private.h"

/* Block signals (auto storage) */
B_test2_T test2_B;

/* Block states (auto storage) */
DW_test2_T test2_DW;

/* Real-time model */
RT_MODEL_test2_T test2_M_;
RT_MODEL_test2_T *const test2_M = &test2_M_;
static void rate_scheduler(void);

/*
 *   This function updates active task flag for each subrate.
 * The function is called at model base rate, hence the
 * generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (test2_M->Timing.TaskCounters.TID[1])++;
  if ((test2_M->Timing.TaskCounters.TID[1]) > 9) {/* Sample time: [5.0E-5s, 0.0s] */
    test2_M->Timing.TaskCounters.TID[1] = 0;
  }
}

/* Model step function */
void test2_step(void)
{
  real_T rtb_Switch;
  real32_T rtb_DataTypeConversion2;
  real32_T rtb_mu;
  boolean_T rtb_Compare;
  uint8_T rtb_FixPtSum1;
  real_T rtb_Sum6;
  real_T rtb_RateLimiter;
  real_T rtb_Switch_0;
  if (test2_M->Timing.TaskCounters.TID[1] == 0) {
    /* S-Function (stm32f4_regular_adc): '<Root>/Regular ADC' */

    /* ADC Block - MODIFIED: Only read Vout (PA0), fake Vin and IL */
    RegularADCStartConversion();
    test2_B.RegularADC_o1 = (float)RegularADC_ConvertedValue[0];  /* Vout - READ FROM PCB */
    test2_B.RegularADC_o2 = (float)1354.0;  /* FAKE: 12V * (4096/3.3) / 0.00886447 */
    test2_B.RegularADC_o3 = (float)620.0;   /* FAKE: 0.5A * (4096/3.3) / 0.00080586 */

    /* Gain: '<Root>/Vin' */
    test2_B.Vin = 0.00886447F * test2_B.RegularADC_o2;  /* = 12.0V */

    /* UARTRx: '<Root>/UART Rx' */
    if (UARTRx_Receive(&UART2_Temp_Buffer[0], 19)) {/* Non-blocking */
      memcpy(&test2_B.UARTRx_o2[0], &UART2_Temp_Buffer[0], 19*sizeof(uint8_T));/* D0 [19] */
      test2_B.UARTRx_o1 = 1;           /* READY */
    } else {
      test2_B.UARTRx_o1 = 0;           /* Not READY */
    }

    /* Outputs for Enabled SubSystem: '<Root>/PC_RX' incorporates:
     *  EnablePort: '<S2>/Enable'
     */
    if (test2_B.UARTRx_o1 > 0) {
      /* Unpack: <S2>/Byte Unpack */
      (void) memcpy(&test2_B.ByteUnpack_o1, &test2_B.UARTRx_o2[0],
                    1);
      (void) memcpy(&test2_B.ByteUnpack_o2, &test2_B.UARTRx_o2[1],
                    1);
      (void) memcpy(&test2_B.ByteUnpack_o3, &test2_B.UARTRx_o2[2],
                    4);
      (void) memcpy(&test2_B.ByteUnpack_o4, &test2_B.UARTRx_o2[6],
                    4);
      (void) memcpy(&test2_B.ByteUnpack_o5, &test2_B.UARTRx_o2[10],
                    4);
      (void) memcpy(&test2_B.ByteUnpack_o6, &test2_B.UARTRx_o2[14],
                    4);
      (void) memcpy(&test2_B.ByteUnpack_o7, &test2_B.UARTRx_o2[18],
                    1);

      /* Switch: '<S2>/Switch' incorporates:
       *  Constant: '<S11>/Constant'
       *  Constant: '<S12>/Constant'
       *  Constant: '<S13>/Constant'
       *  Logic: '<S2>/Logical Operator'
       *  RelationalOperator: '<S11>/Compare'
       *  RelationalOperator: '<S12>/Compare'
       *  RelationalOperator: '<S13>/Compare'
       *  Switch: '<S2>/Switch1'
       *  Switch: '<S2>/Switch2'
       *  Switch: '<S2>/Switch3'
       *  Switch: '<S2>/Switch4'
       *  UnitDelay: '<S2>/Unit Delay'
       *  UnitDelay: '<S2>/Unit Delay1'
       *  UnitDelay: '<S2>/Unit Delay2'
       *  UnitDelay: '<S2>/Unit Delay3'
       *  UnitDelay: '<S2>/Unit Delay4'
       */
      if ((test2_B.ByteUnpack_o1 == 165) && (test2_B.ByteUnpack_o2 == 90)) {
        /* Saturate: '<S2>/Saturation' */
        if (test2_B.ByteUnpack_o3 > 25.0F) {
          test2_B.Switch = 25.0F;
        } else if (test2_B.ByteUnpack_o3 < 15.0F) {
          test2_B.Switch = 15.0F;
        } else {
          test2_B.Switch = test2_B.ByteUnpack_o3;
        }

        /* End of Saturate: '<S2>/Saturation' */

        /* Saturate: '<S2>/Saturation1' */
        if (test2_B.ByteUnpack_o4 > 1.0F) {
          test2_B.Switch1 = 1.0F;
        } else if (test2_B.ByteUnpack_o4 < 0.0F) {
          test2_B.Switch1 = 0.0F;
        } else {
          test2_B.Switch1 = test2_B.ByteUnpack_o4;
        }

        /* End of Saturate: '<S2>/Saturation1' */

        /* Saturate: '<S2>/Saturation2' */
        if (test2_B.ByteUnpack_o5 > 0.2F) {
          test2_B.Switch2 = 0.2F;
        } else if (test2_B.ByteUnpack_o5 < 0.0F) {
          test2_B.Switch2 = 0.0F;
        } else {
          test2_B.Switch2 = test2_B.ByteUnpack_o5;
        }

        /* End of Saturate: '<S2>/Saturation2' */

        /* Saturate: '<S2>/Saturation3' */
        if (test2_B.ByteUnpack_o6 > 0.1F) {
          test2_B.Switch3 = 0.1F;
        } else if (test2_B.ByteUnpack_o6 < 0.0F) {
          test2_B.Switch3 = 0.0F;
        } else {
          test2_B.Switch3 = test2_B.ByteUnpack_o6;
        }

        /* End of Saturate: '<S2>/Saturation3' */
        test2_B.Switch4 = (test2_B.ByteUnpack_o7 > 0);
      } else {
        test2_B.Switch = test2_DW.UnitDelay_DSTATE_m;
        test2_B.Switch1 = test2_DW.UnitDelay1_DSTATE;
        test2_B.Switch2 = test2_DW.UnitDelay2_DSTATE;
        test2_B.Switch3 = test2_DW.UnitDelay3_DSTATE;
        test2_B.Switch4 = test2_DW.UnitDelay4_DSTATE;
      }

      /* End of Switch: '<S2>/Switch' */

      /* Update for UnitDelay: '<S2>/Unit Delay' */
      test2_DW.UnitDelay_DSTATE_m = test2_B.Switch;

      /* Update for UnitDelay: '<S2>/Unit Delay1' */
      test2_DW.UnitDelay1_DSTATE = test2_B.Switch1;

      /* Update for UnitDelay: '<S2>/Unit Delay2' */
      test2_DW.UnitDelay2_DSTATE = test2_B.Switch2;

      /* Update for UnitDelay: '<S2>/Unit Delay3' */
      test2_DW.UnitDelay3_DSTATE = test2_B.Switch3;

      /* Update for UnitDelay: '<S2>/Unit Delay4' */
      test2_DW.UnitDelay4_DSTATE = test2_B.Switch4;
    }

    /* End of Outputs for SubSystem: '<Root>/PC_RX' */

    /* Sum: '<S3>/Sum6' incorporates:
     *  UnitDelay: '<S3>/Unit Delay'
     */
    rtb_Sum6 = test2_ConstB.Gain + test2_DW.UnitDelay_DSTATE;

    /* Switch: '<Root>/Vd_ref' */
    if (test2_B.Switch4) {
      /* DataTypeConversion: '<Root>/Data Type Conversion2' */
      rtb_DataTypeConversion2 = test2_B.Switch;
      rtb_Switch = test2_B.Switch;
    } else {
      /* DataTypeConversion: '<Root>/Data Type Conversion2' */
      rtb_DataTypeConversion2 = (real32_T)rtb_Sum6;
      rtb_Switch = rtb_Sum6;
    }

    /* End of Switch: '<Root>/Vd_ref' */

    /* Sum: '<S3>/Sum3' incorporates:
     *  Constant: '<S3>/1 '
     *  Constant: '<S3>/Vdrop'
     *  DataTypeConversion: '<Root>/Data Type Conversion2'
     *  Product: '<S3>/E//Vd_ref + Vdrop'
     *  Sum: '<S3>/Sum5'
     */
    rtb_Switch = 1.0 - test2_B.Vin / ((real32_T)rtb_Switch + 0.35F);

    /* MinMax: '<S3>/min_val' incorporates:
     *  Constant: '<S3>/one'
     *  Sum: '<S3>/one_minus_D'
     */
    if ((rtb_Switch <= 1.0 - rtb_Switch) || rtIsNaN(1.0 - rtb_Switch)) {
      rtb_Switch_0 = rtb_Switch;
    } else {
      rtb_Switch_0 = 1.0 - rtb_Switch;
    }

    /* Sum: '<S3>/Delta_raw' incorporates:
     *  Constant: '<S3>/epsilon'
     *  MinMax: '<S3>/min_val'
     */
    rtb_RateLimiter = rtb_Switch_0 - 1.0;

    /* MinMax: '<S3>/min_val' incorporates:
     *  Constant: '<S3>/one'
     *  Sum: '<S3>/one_minus_D'
     */
    if ((rtb_Switch <= 1.0 - rtb_Switch) || rtIsNaN(1.0 - rtb_Switch)) {
      rtb_Switch_0 = rtb_Switch;
    } else {
      rtb_Switch_0 = 1.0 - rtb_Switch;
    }

    /* Saturate: '<S3>/Delta_max' incorporates:
     *  Constant: '<S3>/epsilon'
     *  MinMax: '<S3>/min_val'
     *  Sum: '<S3>/Delta_raw'
     */
    if (rtb_Switch_0 - 1.0 <= 0.0) {
      rtb_RateLimiter = 0.0;
    }

    /* End of Saturate: '<S3>/Delta_max' */

    /* DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' */
    test2_B.DiscreteTransferFcn = 0.003333F *
      test2_DW.DiscreteTransferFcn_states;

    /* MATLAB Function: '<S3>/MATLAB Function' */
    /*  y,a1,a2,a3 &#x111;&#x1EC1;u single */
    /* MATLAB Function 'Subsystem/MATLAB Function': '<S14>:1' */
    /* '<S14>:1:4' mu = -(a1*y + a2*y*y*y + a3*y*y*y*y*y); */
    rtb_mu = -(test2_B.Switch3 * test2_B.DiscreteTransferFcn *
               test2_B.DiscreteTransferFcn * test2_B.DiscreteTransferFcn *
               test2_B.DiscreteTransferFcn * test2_B.DiscreteTransferFcn +
               (test2_B.Switch2 * test2_B.DiscreteTransferFcn *
                test2_B.DiscreteTransferFcn * test2_B.DiscreteTransferFcn +
                test2_B.Switch1 * test2_B.DiscreteTransferFcn));

    /* Switch: '<S15>/Switch2' incorporates:
     *  Gain: '<S3>/Gain2'
     *  RelationalOperator: '<S15>/LowerRelop1'
     *  RelationalOperator: '<S15>/UpperRelop'
     *  Switch: '<S15>/Switch'
     */
    if (rtb_mu > rtb_RateLimiter) {
      rtb_mu = (real32_T)rtb_RateLimiter;
    } else {
      if (rtb_mu < (real32_T)-rtb_RateLimiter) {
        /* Switch: '<S15>/Switch' incorporates:
         *  Gain: '<S3>/Gain2'
         */
        rtb_mu = (real32_T)-rtb_RateLimiter;
      }
    }

    /* End of Switch: '<S15>/Switch2' */

    /* Sum: '<S3>/Sum4' */
    rtb_RateLimiter = rtb_Switch + rtb_mu;

    /* RateLimiter: '<Root>/Rate Limiter' */
    rtb_Switch = rtb_RateLimiter - test2_DW.PrevY;
    if (rtb_Switch > 0.1) {
      rtb_RateLimiter = test2_DW.PrevY + 0.1;
    } else {
      if (rtb_Switch < -0.2) {
        rtb_RateLimiter = test2_DW.PrevY + -0.2;
      }
    }

    test2_DW.PrevY = rtb_RateLimiter;

    /* End of RateLimiter: '<Root>/Rate Limiter' */

    /* Gain: '<Root>/vC' */
    test2_B.vC = 0.00886447F * test2_B.RegularADC_o1;  /* READ REAL Vout FROM PCB */

    /* Gain: '<Root>/iL' */
    test2_B.iL = 0.00080586F * test2_B.RegularADC_o3;  /* FAKE IL = 0.5A */

    /* RelationalOperator: '<S5>/Compare' incorporates:
     *  Constant: '<S5>/Constant'
     */
    rtb_Compare = (test2_B.iL >= 3.5F);

    /* Outputs for Enabled SubSystem: '<S8>/SET  Priority' incorporates:
     *  EnablePort: '<S10>/Enable'
     */
    if (test2_ConstB.RelationalOperator1) {
      /* Logic: '<S10>/Logical Operator' incorporates:
       *  Logic: '<S10>/Logical Operator2'
       *  UnitDelay: '<S10>/IC = ic'
       */
      test2_B.LogicalOperator = (rtb_Compare || (test2_ConstB.LogicalOperator1 &&
        test2_DW.ICic_DSTATE));

      /* Update for UnitDelay: '<S10>/IC = ic' */
      test2_DW.ICic_DSTATE = test2_B.LogicalOperator;
    }

    /* End of Outputs for SubSystem: '<S8>/SET  Priority' */

    /* Outputs for Enabled SubSystem: '<S8>/RESET Priority' incorporates:
     *  EnablePort: '<S9>/Enable'
     */
    if (test2_ConstB.LogicalOperator2) {
      /* Logic: '<S9>/Logical Operator2' incorporates:
       *  Logic: '<S9>/Logical Operator'
       *  UnitDelay: '<S9>/IC = ic'
       */
      test2_B.LogicalOperator2 = ((rtb_Compare || test2_DW.ICic_DSTATE_d) &&
        test2_ConstB.LogicalOperator1_j);

      /* Update for UnitDelay: '<S9>/IC = ic' */
      test2_DW.ICic_DSTATE_d = test2_B.LogicalOperator2;
    }

    /* End of Outputs for SubSystem: '<S8>/RESET Priority' */

    /* Switch: '<Root>/Switch' */
    if (rtb_RateLimiter > 0.0) {
      /* Saturate: '<Root>/Saturation' incorporates:
       *  Constant: '<Root>/Constant1'
       */
      test2_B.Saturation = 0.0;
    } else {
      /* Saturate: '<Root>/Saturation' incorporates:
       *  Constant: '<S6>/Constant'
       *  Constant: '<S7>/Constant'
       *  Logic: '<Root>/Logical Operator'
       *  Logic: '<Root>/Logical Operator1'
       *  Logic: '<S8>/Logical Operator1'
       *  RelationalOperator: '<S6>/Compare'
       *  RelationalOperator: '<S7>/Compare'
       */
      test2_B.Saturation = ((test2_B.vC >= 22.0F) || (test2_B.Vin <= 10.0F) ||
                            (test2_B.LogicalOperator || test2_B.LogicalOperator2));
    }

    /* End of Switch: '<Root>/Switch' */

    /* S-Function Block: <Root>/Basic PWM (stm32f4_basicpwm) */
    TIM4->CCR1 = (uint32_t) (test2_B.Saturation * BasicPWM_SF);

    /* DataTypeConversion: '<Root>/Data Type Conversion' */
    test2_B.DataTypeConversion = (real32_T)test2_B.Saturation;

    /* Product: '<S3>/X10 = Vd^2//R.Vin' incorporates:
     *  Constant: '<S3>/R'
     *  Product: '<S3>/Vd^2'
     *  Product: '<S3>/Vd^2//R'
     */
    rtb_mu = rtb_DataTypeConversion2 * rtb_DataTypeConversion2 / 15.0F /
      test2_B.Vin;

    /* Sum: '<S3>/Sum2' incorporates:
     *  Product: '<S3>/term1 = x20 * x1_tilde'
     *  Product: '<S3>/term2 = x10 * x2_tilde'
     *  Sum: '<S3>/Sum'
     *  Sum: '<S3>/Sum1'
     */
    test2_B.Sum2 = (test2_B.iL - rtb_mu) * rtb_DataTypeConversion2 - (test2_B.vC
      - rtb_DataTypeConversion2) * rtb_mu;

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  UnitDelay: '<S4>/Output'
     */
    test2_B.DataTypeConversion1 = test2_DW.Output_DSTATE;

    /* UARTTx: '<Root>/UART Tx' */
    {
      /* Flush tx, so we can put data directly to DMA buffer */
      UART2_FlushTxBuffer();

      /* Put data into buffer */
      UART2_Tx_Buffer[0] = 170;        /* Header 0 */
      UART2_Tx_Buffer[1] = 85;         /* Header 1 */
      memcpy(&UART2_Tx_Buffer[2], &test2_B.DataTypeConversion, sizeof(real32_T));/* Data 0 */
      memcpy(&UART2_Tx_Buffer[6], &test2_B.vC, sizeof(real32_T));/* Data 1 */
      memcpy(&UART2_Tx_Buffer[10], &test2_B.Vin, sizeof(real32_T));/* Data 2 */
      memcpy(&UART2_Tx_Buffer[14], &test2_B.iL, sizeof(real32_T));/* Data 3 */
      memcpy(&UART2_Tx_Buffer[18], &test2_B.Sum2, sizeof(real32_T));/* Data 4 */
      memcpy(&UART2_Tx_Buffer[22], &test2_B.DataTypeConversion1, sizeof(uint32_T));/* Data 5 */

      /* Write to DMA, 26 bytes */
      UART2_TxUpdate(26);              /* Only update since data is ready on Tx buffer */
    }

    /* Sum: '<S16>/FixPt Sum1' incorporates:
     *  Constant: '<S16>/FixPt Constant'
     *  UnitDelay: '<S4>/Output'
     */
    rtb_FixPtSum1 = (uint8_T)(test2_DW.Output_DSTATE + 1U);

    /* Switch: '<S17>/FixPt Switch' incorporates:
     *  Constant: '<S17>/Constant'
     */
    if (rtb_FixPtSum1 > 199) {
      rtb_FixPtSum1 = 0U;
    }

    /* End of Switch: '<S17>/FixPt Switch' */

    /* Update for UnitDelay: '<S3>/Unit Delay' */
    test2_DW.UnitDelay_DSTATE = rtb_Sum6;

    /* Update for DiscreteTransferFcn: '<S3>/Discrete Transfer Fcn' */
    test2_DW.DiscreteTransferFcn_states = test2_B.Sum2 - -0.996667F *
      test2_DW.DiscreteTransferFcn_states;

    /* Update for UnitDelay: '<S4>/Output' */
    test2_DW.Output_DSTATE = rtb_FixPtSum1;
  }

  if (test2_M->Timing.TaskCounters.TID[1] == 0) {
    /* Update absolute timer for sample time: [5.0E-5s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 5.0E-5, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     */
    test2_M->Timing.clockTick1++;
  }

  rate_scheduler();
}

/* Model initialize function */
void test2_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)test2_M, 0,
                sizeof(RT_MODEL_test2_T));

  /* block I/O */
  (void) memset(((void *) &test2_B), 0,
                sizeof(B_test2_T));

  /* states (dwork) */
  (void) memset((void *)&test2_DW, 0,
                sizeof(DW_test2_T));

  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter' */
  test2_DW.PrevY = 0.0;

  /* SystemInitialize for Enabled SubSystem: '<Root>/PC_RX' */
  /* InitializeConditions for UnitDelay: '<S2>/Unit Delay' */
  test2_DW.UnitDelay_DSTATE_m = 20.0F;

  /* InitializeConditions for UnitDelay: '<S2>/Unit Delay1' */
  test2_DW.UnitDelay1_DSTATE = 0.2F;

  /* End of SystemInitialize for SubSystem: '<Root>/PC_RX' */
  /* Level2 S-Function Block: '<Root>/Regular ADC' (stm32f4_regular_adc) */
  enable_RegularADC();

  /* Level2 S-Function Block: '<Root>/UART Rx' (stm32f4_usart) */
  enable_UARTRx();

  /* Level2 S-Function Block: '<Root>/Basic PWM' (stm32f4_basicpwm) */
  enable_BasicPWM();

  /* Level2 S-Function Block: '<Root>/UART Tx' (stm32f4_usart) */
  enable_UARTTx();

  /* Level2 S-Function Block: '<Root>/UART Setup' (stm32f4_usart) */
  enable_UARTSetup();
}

/* Model terminate function */
void test2_terminate(void)
{
  /* Terminate for S-Function (stm32f4_regular_adc): '<Root>/Regular ADC' */

  /* terminate_RegularADC(); */

  /* Terminate for S-Function (stm32f4_basicpwm): '<Root>/Basic PWM' */

  /* terminate_BasicPWM(); */
}

/* [EOF] */
