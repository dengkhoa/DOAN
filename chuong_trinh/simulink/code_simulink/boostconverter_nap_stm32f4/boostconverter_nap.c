/*
 * File: boostconverter_nap.c
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

#include "boostconverter_nap.h"
#include "boostconverter_nap_private.h"

/* Block signals (auto storage) */
B_boostconverter_nap_T boostconverter_nap_B;

/* Block states (auto storage) */
DW_boostconverter_nap_T boostconverter_nap_DW;

/* Real-time model */
RT_MODEL_boostconverter_nap_T boostconverter_nap_M_;
RT_MODEL_boostconverter_nap_T *const boostconverter_nap_M =
  &boostconverter_nap_M_;

/* ########################################################################
 * Block Name: <Root>/Timer (Time Base) IRQ
 * Block ID: TimerTimeBaseIRQ
 * ########################################################################
 * Function call: TimerTimeBaseIRQ IRQ
 */
void TimerTimeBaseIRQ_irq(void)
{
  /* Call the system: <Root>/Function-Call Subsystem */
  {
    /* Output and update for function-call system: '<Root>/Function-Call Subsystem' */
    {
      real_T rtb_Sum8;
      real_T rtb_Saturation;

      /* Product: '<S6>/Product4' incorporates:
       *  Constant: '<S6>/R30 '
       *  DataStoreRead: '<S6>/Data Store Read'
       *  DataStoreRead: '<S6>/Data Store Read2'
       *  Product: '<S6>/Product3'
       *  Product: '<S6>/Product5'
       */
      rtb_Sum8 = boostconverter_nap_DW.Vd_cmd * boostconverter_nap_DW.Vd_cmd /
        (30.0 * boostconverter_nap_DW.Vin);

      /* Sum: '<S6>/Sum8' incorporates:
       *  DataStoreRead: '<S5>/Vin'
       *  DataStoreRead: '<S5>/Vout'
       *  DataStoreRead: '<S6>/Data Store Read'
       *  Product: '<S6>/Product6'
       *  Product: '<S6>/Product7'
       *  Sum: '<S6>/Sum4'
       *  Sum: '<S6>/Sum7'
       */
      rtb_Sum8 = (boostconverter_nap_DW.Vout - rtb_Sum8) *
        boostconverter_nap_DW.Vd_cmd - (boostconverter_nap_DW.Vin -
        boostconverter_nap_DW.Vd_cmd) * rtb_Sum8;

      /* Product: '<S6>/Product' */
      rtb_Saturation = rtb_Sum8 * rtb_Sum8 * rtb_Sum8;

      /* Sum: '<S6>/Sum1' incorporates:
       *  Constant: '<S6>/Constant'
       *  DataStoreRead: '<S6>/Data Store Read'
       *  DataStoreRead: '<S6>/Data Store Read2'
       *  DataStoreRead: '<S6>/a1'
       *  DataStoreRead: '<S6>/a2'
       *  DataStoreRead: '<S6>/a3'
       *  Product: '<S6>/Product1'
       *  Product: '<S6>/Product10'
       *  Product: '<S6>/Product2'
       *  Product: '<S6>/Product8'
       *  Product: '<S6>/Product9'
       *  Sum: '<S6>/Sum'
       *  Sum: '<S6>/Sum2'
       */
      rtb_Saturation = (((0.0 - rtb_Sum8 * rtb_Sum8 * rtb_Saturation *
                          boostconverter_nap_DW.a2) - rtb_Saturation *
                         boostconverter_nap_DW.a3) - rtb_Sum8 *
                        boostconverter_nap_DW.a1) + (1.0 -
        boostconverter_nap_DW.Vin / boostconverter_nap_DW.Vd_cmd);

      /* Saturate: '<S5>/Saturation' */
      if (rtb_Saturation > 1.0) {
        rtb_Saturation = 1.0;
      } else {
        if (rtb_Saturation < 0.0) {
          rtb_Saturation = 0.0;
        }
      }

      /* End of Saturate: '<S5>/Saturation' */

      /* DataStoreWrite: '<S5>/duty ' */
      boostconverter_nap_DW.duty = rtb_Saturation;

      /* Gain: '<S5>/Gain' */
      boostconverter_nap_B.Gain = 18.0 * rtb_Saturation;

      /* S-Function Block: <S5>/Basic PWM (stm32f4_basicpwm) */
      TIM4->CCR1 = (uint32_t) (boostconverter_nap_B.Gain *
        FunctionCallSubsystemPBCBasicPWM_SF);
    }
  }
}

/* Model step function */
void boostconverter_nap_step(void)
{
  /* S-Function (stm32f4_regular_adc): '<S1>/Regular ADC' */

  /* ADC Block */
  RegularADCStartConversion();
  boostconverter_nap_B.RegularADC_o1 = (double)RegularADC_ConvertedValue[0];
  boostconverter_nap_B.RegularADC_o2 = (double)RegularADC_ConvertedValue[1];
  boostconverter_nap_B.RegularADC_o3 = (double)RegularADC_ConvertedValue[2];

  /* DataStoreWrite: '<S1>/Vin ' incorporates:
   *  Gain: '<S1>/Gain1'
   */
  boostconverter_nap_DW.Vin = 0.0048 * boostconverter_nap_B.RegularADC_o1;

  /* DataStoreWrite: '<S1>/Vout' incorporates:
   *  Gain: '<S1>/Gain'
   */
  boostconverter_nap_DW.Vout = 0.009 * boostconverter_nap_B.RegularADC_o2;

  /* DataStoreWrite: '<S1>/iL ' incorporates:
   *  Gain: '<S1>/Gain2'
   */
  boostconverter_nap_DW.iL = 0.00080586 * boostconverter_nap_B.RegularADC_o3;

  /* S-Function Block: <Root>/Timer (Time Base) IRQ */

  /* NHAN_DU_LIEUUARTRx: '<S3>/UART Rx' */
  if (NHAN_DU_LIEUUARTRx_Receive(&UART5_Temp_Buffer[0], URX5_BUFFER_SIZE)) {/* Non-Blocking */
    boostconverter_nap_B.UARTRx_o2 = NHAN_DU_LIEUUARTRx_data0;/* D0 */
    boostconverter_nap_B.UARTRx_o3 = NHAN_DU_LIEUUARTRx_data1;/* D1 */
    boostconverter_nap_B.UARTRx_o4 = NHAN_DU_LIEUUARTRx_data2;/* D2 */
    boostconverter_nap_B.UARTRx_o5 = NHAN_DU_LIEUUARTRx_data3;/* D3 */
    boostconverter_nap_B.UARTRx_o1 = 1;/* READY */
  } else {
    boostconverter_nap_B.UARTRx_o1 = 0;/* Not READY */
  }

  /* Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S7>/Enable'
   */
  if (boostconverter_nap_B.UARTRx_o1 > 0) {
    /* DataStoreWrite: '<S7>/a1' incorporates:
     *  DataTypeConversion: '<S7>/Data Type Conversion'
     */
    boostconverter_nap_DW.a1 = boostconverter_nap_B.UARTRx_o2;

    /* DataStoreWrite: '<S7>/a2' incorporates:
     *  DataTypeConversion: '<S7>/Data Type Conversion1'
     */
    boostconverter_nap_DW.a2 = boostconverter_nap_B.UARTRx_o3;

    /* DataStoreWrite: '<S7>/a3' incorporates:
     *  DataTypeConversion: '<S7>/Data Type Conversion2'
     */
    boostconverter_nap_DW.a3 = boostconverter_nap_B.UARTRx_o4;

    /* DataStoreWrite: '<S7>/a4' incorporates:
     *  DataTypeConversion: '<S7>/Data Type Conversion3'
     */
    boostconverter_nap_DW.Vd_cmd = boostconverter_nap_B.UARTRx_o5;
  }

  /* End of Outputs for SubSystem: '<S3>/Enabled Subsystem' */

  /* DataTypeConversion: '<S4>/Data Type Conversion' incorporates:
   *  DataStoreRead: '<S4>/Vout'
   */
  boostconverter_nap_B.DataTypeConversion = (real32_T)boostconverter_nap_DW.Vout;

  /* TRUYEN_DU_LIEUUARTTx: '<S4>/UART Tx' */
  {
    UART5_FlushTxBuffer();             /* Flush tx, so we can print to DMA buffer directly */
    sprintf((char*)UART5_Tx_Buffer, "vl=%f\r\n\r\n" ,
            boostconverter_nap_B.DataTypeConversion );
    UART5_TxUpdate(strlen((char*)UART5_Tx_Buffer));
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion1' incorporates:
   *  DataStoreRead: '<S4>/duty'
   */
  boostconverter_nap_B.DataTypeConversion1 = (real32_T)
    boostconverter_nap_DW.duty;

  /* TRUYEN_DU_LIEUUARTTx2: '<S4>/UART Tx2' */
  {
    UART5_FlushTxBuffer();             /* Flush tx, so we can print to DMA buffer directly */
    sprintf((char*)UART5_Tx_Buffer, "d=%f\r\n\r\n" ,
            boostconverter_nap_B.DataTypeConversion1 );
    UART5_TxUpdate(strlen((char*)UART5_Tx_Buffer));
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion2' incorporates:
   *  DataStoreRead: '<S4>/Vin'
   */
  boostconverter_nap_B.DataTypeConversion2 = (real32_T)boostconverter_nap_DW.Vin;

  /* TRUYEN_DU_LIEUUARTTx1: '<S4>/UART Tx1' */
  {
    UART5_FlushTxBuffer();             /* Flush tx, so we can print to DMA buffer directly */
    sprintf((char*)UART5_Tx_Buffer, "vi=%f\r\n\r\n" ,
            boostconverter_nap_B.DataTypeConversion2 );
    UART5_TxUpdate(strlen((char*)UART5_Tx_Buffer));
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion3' incorporates:
   *  DataStoreRead: '<S4>/a1'
   */
  boostconverter_nap_B.DataTypeConversion3 = (real32_T)boostconverter_nap_DW.a1;

  /* TRUYEN_DU_LIEUUARTTx3: '<S4>/UART Tx3' */
  {
    UART5_FlushTxBuffer();             /* Flush tx, so we can print to DMA buffer directly */
    sprintf((char*)UART5_Tx_Buffer, "a1=%f\r\n\r\n" ,
            boostconverter_nap_B.DataTypeConversion3 );
    UART5_TxUpdate(strlen((char*)UART5_Tx_Buffer));
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion4' incorporates:
   *  DataStoreRead: '<S4>/a2'
   */
  boostconverter_nap_B.DataTypeConversion4 = (real32_T)boostconverter_nap_DW.a2;

  /* TRUYEN_DU_LIEUUARTTx4: '<S4>/UART Tx4' */
  {
    UART5_FlushTxBuffer();             /* Flush tx, so we can print to DMA buffer directly */
    sprintf((char*)UART5_Tx_Buffer, "a2=%f\r\n\r\n" ,
            boostconverter_nap_B.DataTypeConversion4 );
    UART5_TxUpdate(strlen((char*)UART5_Tx_Buffer));
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion5' incorporates:
   *  DataStoreRead: '<S4>/a3'
   */
  boostconverter_nap_B.DataTypeConversion5 = (real32_T)boostconverter_nap_DW.a3;

  /* TRUYEN_DU_LIEUUARTTx5: '<S4>/UART Tx5' */
  {
    UART5_FlushTxBuffer();             /* Flush tx, so we can print to DMA buffer directly */
    sprintf((char*)UART5_Tx_Buffer, "a3=%f\r\n\r\n" ,
            boostconverter_nap_B.DataTypeConversion5 );
    UART5_TxUpdate(strlen((char*)UART5_Tx_Buffer));
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion6' incorporates:
   *  DataStoreRead: '<S4>/iL'
   */
  boostconverter_nap_B.DataTypeConversion6 = (real32_T)boostconverter_nap_DW.iL;

  /* TRUYEN_DU_LIEUUARTTx6: '<S4>/UART Tx6' */
  {
    UART5_FlushTxBuffer();             /* Flush tx, so we can print to DMA buffer directly */
    sprintf((char*)UART5_Tx_Buffer, "i=%f\r\n\r\n" ,
            boostconverter_nap_B.DataTypeConversion6 );
    UART5_TxUpdate(strlen((char*)UART5_Tx_Buffer));
  }
}

/* Model initialize function */
void boostconverter_nap_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(boostconverter_nap_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &boostconverter_nap_B), 0,
                sizeof(B_boostconverter_nap_T));

  /* states (dwork) */
  (void) memset((void *)&boostconverter_nap_DW, 0,
                sizeof(DW_boostconverter_nap_T));

  /* Start for DataStoreMemory: '<Root>/Data Store Memory' */
  boostconverter_nap_DW.Vd_cmd = 20.0;

  /* Level2 S-Function Block: '<Root>/UART Setup' (stm32f4_usart) */
  enable_UARTSetup();

  /* Level2 S-Function Block: '<S1>/Regular ADC' (stm32f4_regular_adc) */
  enable_RegularADC();

  /* Level2 S-Function Block: '<Root>/Timer (Time Base) IRQ' (stm32f4_bttbisr) */
  enable_TimerTimeBaseIRQ();

  /* Enable for S-Function (stm32f4_bttbisr): '<Root>/Timer (Time Base) IRQ' incorporates:
   *  Enable for SubSystem: '<Root>/Function-Call Subsystem'
   */

  /* Enable for function-call system: '<Root>/Function-Call Subsystem' */

  /* Level2 S-Function Block: '<S5>/Basic PWM' (stm32f4_basicpwm) */
  enable_FunctionCallSubsystemPBCBasicPWM();

  /* End of Enable for S-Function (stm32f4_bttbisr): '<Root>/Timer (Time Base) IRQ' */

  /* Level2 S-Function Block: '<S3>/UART Rx' (stm32f4_usart) */
  enable_NHAN_DU_LIEUUARTRx();

  /* Level2 S-Function Block: '<S4>/UART Tx' (stm32f4_usart) */
  enable_TRUYEN_DU_LIEUUARTTx();

  /* Level2 S-Function Block: '<S4>/UART Tx2' (stm32f4_usart) */
  enable_TRUYEN_DU_LIEUUARTTx2();

  /* Level2 S-Function Block: '<S4>/UART Tx1' (stm32f4_usart) */
  enable_TRUYEN_DU_LIEUUARTTx1();

  /* Level2 S-Function Block: '<S4>/UART Tx3' (stm32f4_usart) */
  enable_TRUYEN_DU_LIEUUARTTx3();

  /* Level2 S-Function Block: '<S4>/UART Tx4' (stm32f4_usart) */
  enable_TRUYEN_DU_LIEUUARTTx4();

  /* Level2 S-Function Block: '<S4>/UART Tx5' (stm32f4_usart) */
  enable_TRUYEN_DU_LIEUUARTTx5();

  /* Level2 S-Function Block: '<S4>/UART Tx6' (stm32f4_usart) */
  enable_TRUYEN_DU_LIEUUARTTx6();
}

/* Model terminate function */
void boostconverter_nap_terminate(void)
{
  /* Terminate for S-Function (stm32f4_regular_adc): '<S1>/Regular ADC' */

  /* terminate_RegularADC(); */

  /* Terminate for S-Function (stm32f4_bttbisr): '<Root>/Timer (Time Base) IRQ' */

  /* terminate_TimerTimeBaseIRQ(); */

  /* Terminate for S-Function (stm32f4_bttbisr): '<Root>/Timer (Time Base) IRQ' incorporates:
   *  Terminate for SubSystem: '<Root>/Function-Call Subsystem'
   */

  /* Termination for function-call system: '<Root>/Function-Call Subsystem' */

  /* Terminate for S-Function (stm32f4_basicpwm): '<S5>/Basic PWM' */

  /* terminate_FunctionCallSubsystemPBCBasicPWM(); */

  /* End of Terminate for S-Function (stm32f4_bttbisr): '<Root>/Timer (Time Base) IRQ' */
}

/* [EOF] */
