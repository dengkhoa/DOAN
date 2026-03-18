/*
 * File: waijung_hwdrvlib.c
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

void enable_FunctionCallSubsystemPBCBasicPWM(void)
{
  /* Block: FunctionCallSubsystemPBCBasicPWM */
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* GPIOB configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect TIM4 pins to AF */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = FunctionCallSubsystemPBCBasicPWM_TIM4_ARR-1;/* 5.0E-5 sec */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* Init OC */
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  /* PWM1 Mode configuration: Channel 1 */
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* TIM4 configure auto reload */
  TIM_ARRPreloadConfig(TIM4, ENABLE);

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}

void disable_FunctionCallSubsystemPBCBasicPWM(void)
{
  TIM_Cmd(TIM4, DISABLE);
}

void terminate_FunctionCallSubsystemPBCBasicPWM(void)
{
  TIM_Cmd(TIM4, DISABLE);
}

/* ########################################################################
 * Name: <Root>/UART Setup
 * Id: UARTSetup
 * ########################################################################
 */

/* Tx buffer */
uint8_t UART5_Tx_Buffer[UTX5_BUFFER_SIZE];
UART_TX_STATE UARTSetup_Tx_State;

/* Rx buffer */
uint8_t UART5_Rx_Buffer[URX5_BUFFER_SIZE];
uint8_t UART5_Temp_Buffer[URX5_BUFFER_SIZE];

/* DMA Init struct */
DMA_InitTypeDef UARTSetup_DMA_Init;

/* Init buffer read */
void UART5_RestoreBytes(UARTRX_BUFFER_READ_STRUCT *read_struct, uint16_t count)
{
  uint16_t roll_count = count;

  /* Remove overflow buffer */
  while (roll_count > URX5_BUFFER_SIZE)
    roll_count -= URX5_BUFFER_SIZE;

  /* Return bytes back into buffer */
  if (roll_count > read_struct->index) {
    read_struct->index = URX5_BUFFER_SIZE - roll_count + read_struct->index;
  } else {
    read_struct->index -= roll_count;
  }
}

/* Read buffer from DMA
 ** Return value: Number of bytes vaiable.
 */
void UART5_Read(UARTRX_BUFFER_READ_STRUCT *read_struct)
{
  __IO uint16_t dma_curr_ndtr;
  __IO uint16_t dma_curr_index;
  uint16_t data_index = 0;
  uint16_t data_count = 0;

  /* Get current NDTR */
  if ((dma_curr_ndtr = DMA1_Stream0->NDTR) == 0) {/* Not initialize or turn-around state*/
    read_struct->count = 0;
    return;
  }

  /* Get current data indexs */
  dma_curr_index = URX5_BUFFER_SIZE - dma_curr_ndtr;
  if (read_struct->index < dma_curr_index) {
    /* Data is available */
    data_index = read_struct->index;
    data_count = dma_curr_index - read_struct->index;
    read_struct->index += data_count;
    read_struct->index &= (URX5_BUFFER_SIZE-1);
  } else if (read_struct->index > dma_curr_index) {
    /* Data is available with overlap */
    data_index = read_struct->index;
    data_count = URX5_BUFFER_SIZE-read_struct->index;
    read_struct->index = 0;
  } else {                             /* No new data */
  }

  /* Return the reading */
  if (data_count > 0) {
    read_struct->buffer = &UART5_Rx_Buffer[data_index];
    read_struct->count = data_count;
  } else {
    read_struct->count = 0;
  }
}

void UART5_ReadEx(UARTRX_BUFFER_READ_STRUCT *read_struct, uint8_t *buffer,
                  uint16_t buffer_size, uint16_t*reading_count)
{
  uint16_t bytes_to_read, data_read_index;
  bytes_to_read = buffer_size;         /* Tracking count of data readings */
  data_read_index = 0;                 /* Increment buffer index */
  do {
    UART5_Read(read_struct);
    if (read_struct->count <= bytes_to_read) {
      memcpy(&buffer[data_read_index], read_struct->buffer, read_struct->count);
      data_read_index += read_struct->count;
      bytes_to_read -= read_struct->count;
    } else {
      /* Return some byte back to buffer */
      //read_struct->index -= (read_struct->count - bytes_to_read);
      UART5_RestoreBytes(read_struct, (read_struct->count - bytes_to_read));/* Fixed: Waijung 14.05a */

      /* Return reading data */
      memcpy(&buffer[data_read_index], read_struct->buffer, bytes_to_read);
      bytes_to_read = 0;
    }
  } while ((bytes_to_read > 0) && (read_struct->count > 0));

  /* Number of reading bytes */
  *reading_count = buffer_size - bytes_to_read;
}

/* Read Ascii packet
 * Return char count, exclude NULL
 * Terminator: "\n", "\r", "\r\n"
 */
uint16_t UART5_ReadLine(UARTRX_BUFFER_READ_STRUCT *read_struct, const char
  *terminator, uint16_t terminator_count, uint8_t *buffer, uint16_t buffer_size)
{
  uint16_t count, packet_len = 0, receive_count = 0;
  uint16_t i;
  uint16_t terminator_found = 0;

  /* Determine maximum number of bytes to read */
  count = buffer_size - 1;
  if (count >= URX5_BUFFER_SIZE)
    count = URX5_BUFFER_SIZE-1;

  /* Ignore terminator is invalid */
  if (terminator_count < 1)
    return 0;

  /* Read packet */
  do {
    terminator_found = 0;
    UART5_Read(read_struct);           /* Check DMA buffer */
    receive_count += read_struct->count;/* Total number of data received */

    /* Search terminator */
    i = 0;
    while (!(terminator_found == terminator_count) && (i < read_struct->count))
    {
      if (read_struct->buffer[i] == (uint8_t)terminator[terminator_found/*terminator_count - 1*/
          ])
        terminator_found ++;
      i++;
    }

    packet_len += i;
    if (terminator_found == terminator_count) {
      terminator_found = 0;

      /* Roll-back buffer index */
      if ((packet_len > count) || (packet_len < terminator_count)) {/* Packet count is invalid, drop it */
        UART5_RestoreBytes(read_struct, (receive_count-packet_len));

        /* Reset */
        packet_len = 0;
        receive_count = 0;
      } else {
        UART5_RestoreBytes(read_struct, receive_count);

        /* Load data into buffer */
        UART5_ReadEx(read_struct, buffer, packet_len, &i);
        buffer[packet_len] = '\0';     /* Append NULL */

        /* Validate terminator */
        if (!strncmp((char *)&buffer[packet_len-terminator_count], terminator,
                     terminator_count)) {
          return packet_len;           /* packet reading success, return number of received bytes */
        } else {
          /* Invalid terminator */
          packet_len = 0;
          receive_count = 0;
        }
      }
    }
  } while (read_struct->count > 0);

  /* Could not find the packet terminator, reset reading struct to its original position */
  if (receive_count > 0) {
    UART5_RestoreBytes(read_struct, receive_count);
  }

  /* No byte receive */
  return 0;
}

/* Read Binary packet
 * 0: Not ready, 1: Data is ready
 */
uint8_t UART5_ReadBinary(UARTRX_BUFFER_READ_STRUCT *read_struct, const char
  *header, uint16_t header_count, const char *terminator, uint16_t
  terminator_count, uint8_t *buffer, uint16_t data_count)
{
  uint16_t receive_count = 0, drop_count = 0, binary_state = 0, binary_index = 0;
  uint16_t i;
  do {
    UART5_Read(read_struct);           /* Check DMA buffer */
    receive_count += read_struct->count;/* Total number of data received */

    /* Binary packet processing */
    for (i=0; i<read_struct->count; i++) {
      switch ( binary_state ) {
       case 0:                         /* Search for header */
        if (binary_index < header_count) {
          if (read_struct->buffer[i] == header[binary_index]) {
            binary_index ++;
          } else {
            binary_index = 0;
            drop_count = receive_count - (read_struct->count - i - 1);/* Drop packet */
          }
          break;
        } else {                       /* Change to DATA state */
          binary_index = 0;
          binary_state ++;
        }

       case 1:                         /* Wait for data */
        /* Wait for DATA */
        if (binary_index < data_count) {
          buffer[binary_index] = read_struct->buffer[i];
          binary_index ++;

          /* Check if ready (No terminator) */
          if ((binary_index >= data_count) && (terminator_count == 0)) {
            UART5_RestoreBytes(read_struct, (read_struct->count - i - 1));/* Restore some bytes */
            return 1;                  /* Return success status */
          }
          break;
        } else {                       /* Change to Terminator state */
          binary_index = 0;
          binary_state ++;
        }

       case 2:                         /* Scan for terminator */
        if (binary_index < terminator_count) {
          if (read_struct->buffer[i] == terminator[binary_index]) {
            binary_index ++;
          } else {
            binary_state = 0;
            binary_index = 0;
            drop_count = receive_count - (read_struct->count - i - 1);/* Drop packet */
          }
        }

        if (binary_index >= terminator_count) {/* Success */
          /* Restore some bytes */
          UART5_RestoreBytes(read_struct, (read_struct->count - i - 1));
          return 1;                    /* Return success status */
        }
        break;
      }
    }
  } while (read_struct->count > 0);

  /* Restore bytes */
  UART5_RestoreBytes(read_struct, (receive_count - drop_count));
  return 0;
}

/* Wait for DMA transfer to completion */
UART_TX_STATE UART5_FlushTxBuffer(void)
{
  if (UARTSetup_Tx_State != txIdle) {
    /* Chehck last DMA transmit */
    while ((DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7) == RESET)
           || (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET)) {
    }

    /* Clear DMA Streams flags */
    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_HTIF7 | DMA_FLAG_TCIF7);

    /* Disable the DMA Streams */
    DMA_Cmd(DMA1_Stream7, DISABLE);

    /* Disable the USART Tx DMA request */
    USART_DMACmd(UART5, USART_DMAReq_Tx, DISABLE);

    /* Update state */
    UARTSetup_Tx_State = txIdle;
  }

  return UARTSetup_Tx_State;
}

/* Write to DMA buffer */
void UART5_Write(uint8_t* data, uint16_t count)
{
  if (count > 0) {
    /* Wait for Tx state to idle */
    UART5_FlushTxBuffer();

    /* Setup */
    memcpy(UART5_Tx_Buffer, data, count);

    /* Update */
    UART5_TxUpdate(count);
  }
}

/* Update transmit buffer */
void UART5_TxUpdate(uint16_t count)
{
  if (count > 0) {
    /* Wait for Tx state to idle */
    UART5_FlushTxBuffer();
    UARTSetup_DMA_Init.DMA_BufferSize = (uint16_t)count;
    DMA_Init(DMA1_Stream7, &UARTSetup_DMA_Init);

    /* Enable */
    USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);
    USART_ClearFlag(UART5, USART_FLAG_TC);
    DMA_Cmd(DMA1_Stream7, ENABLE);

    /* Tx is busy */
    UARTSetup_Tx_State = txBusy;
  }
}

static uint8_t waijung_usart5_ready = 0;
void waijung_usart5_initial(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  if (!waijung_usart5_ready) {
    waijung_usart5_ready = 1;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    /* Enable the DMA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

    /* Configure USART pin as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    /* Configure USART Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure USART Rx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* USART Configuration */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* Over sampling x8 */
    USART_OverSampling8Cmd(UART5, ENABLE);

    /* USART configuration */
    USART_Init(UART5, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(UART5, ENABLE);

    /* DMA init struct configuration */
    UARTSetup_DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);
    UARTSetup_DMA_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    UARTSetup_DMA_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;
    UARTSetup_DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    UARTSetup_DMA_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    UARTSetup_DMA_Init.DMA_FIFOMode = DMA_FIFOMode_Disable;
    UARTSetup_DMA_Init.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    UARTSetup_DMA_Init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    UARTSetup_DMA_Init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    /* Configure DMA controller to manage RX DMA request */
    UARTSetup_DMA_Init.DMA_Channel = DMA_Channel_4;
    UARTSetup_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)UART5_Rx_Buffer;
    UARTSetup_DMA_Init.DMA_DIR = DMA_DIR_PeripheralToMemory;
    UARTSetup_DMA_Init.DMA_BufferSize = (uint16_t)URX5_BUFFER_SIZE;
    UARTSetup_DMA_Init.DMA_Mode = DMA_Mode_Circular;
    UARTSetup_DMA_Init.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_Init(DMA1_Stream0, &UARTSetup_DMA_Init);

    /* Enable the USART Rx DMA requests */
    USART_DMACmd(UART5, USART_DMAReq_Rx , ENABLE);

    /* Enable the DMA Stream */
    DMA_Cmd(DMA1_Stream0, ENABLE);

    /* Configure DMA controller to manage TX DMA request */
    UARTSetup_DMA_Init.DMA_Channel = DMA_Channel_4;
    UARTSetup_DMA_Init.DMA_Memory0BaseAddr = (uint32_t)UART5_Tx_Buffer;
    UARTSetup_DMA_Init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    UARTSetup_DMA_Init.DMA_BufferSize = (uint16_t)UTX5_BUFFER_SIZE;
    UARTSetup_DMA_Init.DMA_Mode = DMA_Mode_Normal;
    UARTSetup_DMA_Init.DMA_Priority = DMA_Priority_High;

    /* Init Tx state */
    UARTSetup_Tx_State = txIdle;
  }
}

/* Enable UART-DMA module */
void enable_UARTSetup(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Timer
 * ########################################################################
 */

/* Low precision non-Blocking delay timer.
 ** Relolution of timer is one TICK step interrupt.
 */
void SysTimer_Start(SYS_TIMER_STRUCT* timer, uint32_t ms)
{
  /* Calculate amount of tick count */
  timer->length = (uint32_t)((float)ms/(1000*SYS_TICKSTEP));

  /* Capture current tick */
  timer->start = (uint32_t)systick_count;
}

int SysTimer_IsTimeout(SYS_TIMER_STRUCT* timer)
{
  uint32_t Capture;

  /* Capture current tick */
  Capture = (uint32_t)systick_count;

  /* Check */
  if (Capture >= timer->start) {
    if ((Capture - timer->start) >= timer->length)
      return 1;
  } else {
    if (((0xFFFFFFFF - timer->start)+Capture) >= timer->length)
      return 1;
  }

  return 0;
}

/* Improved precision non-Blocking delay timer.
 ** Relolution of timer is 1uS.
 */
void SysTimer_uS_Start(SYS_TIMER_uS_STRUCT *timer, uint32_t us)
{
  /* Tick */
  timer->tick = SYS_CURRENT_TICK;

  /* Tick overflow count */
  timer->tid = (uint32_t)systick_count;

  /* Target */
  timer->target_us = us*((uint32_t)HCLK/1000000);
}

int SysTimer_uS_IsTimeout(SYS_TIMER_uS_STRUCT* timer)
{
  uint32_t curr_tick;
  uint32_t curr_tid;

  /* Already timeout ? */
  if (timer->target_us == 0) {
    return 1;
  }

  /* Capture current tick */
  curr_tick = SYS_CURRENT_TICK;
  curr_tid = systick_count;

  /* Processing TID*/
  while (timer->tid != curr_tid) {
    if (timer->target_us > timer->tick) {
      timer->target_us -= timer->tick;
      timer->tid ++;
      timer->tick = SYSTICKRELOADVALUE-1;
    } else {
      timer->target_us = 0;
      return 1;                        /* Timeout */
    }
  }

  /* Processing Tick */
  if ((timer->tick > curr_tick) && ((timer->tick - curr_tick) > timer->target_us))
  {
    timer->target_us = 0;
    return 1;                          /* Timeout */
  }

  /* Need wait */
  return 0;
}

/* Blocking delay, uS */
void SysTimer_delay_us(uint32_t us)
{
  SYS_TIMER_uS_STRUCT timer;
  SysTimer_uS_Start(&timer, us);
  while (SysTimer_uS_IsTimeout(&timer) == 0) ;
}

/* Blocking delay, mS */
void SysTimer_delay_ms(uint32_t ms)
{
  SYS_TIMER_STRUCT timer;
  SysTimer_Start(&timer, ms);
  while (SysTimer_IsTimeout(&timer) == 0) ;
}

/* ########################################################################
 * Name: <S1>/Regular ADC
 * Id: RegularADC
 * ########################################################################
 */
ADC_InitTypeDef RegularADC_ADC_InitStructure;
ADC_CommonInitTypeDef RegularADC_ADC_CommonInitStructure;
DMA_InitTypeDef RegularADC_DMA_InitStructure;

/* RegularADC */
uint16_t RegularADC_ConvertedValue[3];
void enable_RegularADC(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable ADCx, DMA and GPIO clocks ****************************************/
  /* ADC1 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Configure ADCx using channel pins as analog input ******************************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

  /* ADC pin A0*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ADC pin A1*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ADC pin A4*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* DMA2 Stream channel configuration **************************************/
  RegularADC_DMA_InitStructure.DMA_Channel = RegularADC_DMA_CHANNEL;
  RegularADC_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  RegularADC_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  RegularADC_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  RegularADC_DMA_InitStructure.DMA_PeripheralDataSize =
    DMA_PeripheralDataSize_HalfWord;
  RegularADC_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  RegularADC_DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  RegularADC_DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  RegularADC_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  RegularADC_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  RegularADC_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  RegularADC_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  RegularADC_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)
    RegularADC_ADC_DR_ADDRESS;
  RegularADC_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&
    RegularADC_ConvertedValue;
  RegularADC_DMA_InitStructure.DMA_BufferSize = 3;

  /* ADC Common Init **********************************************************/
  RegularADC_ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  RegularADC_ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  RegularADC_ADC_CommonInitStructure.ADC_DMAAccessMode =
    ADC_DMAAccessMode_Disabled;
  RegularADC_ADC_CommonInitStructure.ADC_TwoSamplingDelay =
    ADC_TwoSamplingDelay_5Cycles;

  /* ADCx Init ****************************************************************/
  RegularADC_ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  RegularADC_ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  RegularADC_ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  RegularADC_ADC_InitStructure.ADC_ExternalTrigConvEdge =
    ADC_ExternalTrigConvEdge_None;
  RegularADC_ADC_InitStructure.ADC_ExternalTrigConv =
    ADC_ExternalTrigConv_Ext_IT11;     // avoid assert_fail
  RegularADC_ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  RegularADC_ADC_InitStructure.ADC_NbrOfConversion = 3;
}

void RegularADCStartConversion(void)
{
  /* DMA */
  DMA_DeInit(RegularADC_DMA_STREAM);
  DMA_Init(RegularADC_DMA_STREAM, &RegularADC_DMA_InitStructure);
  DMA_Cmd(RegularADC_DMA_STREAM, ENABLE);

  /* ADC Common */
  ADC_CommonInit(&RegularADC_ADC_CommonInitStructure);

  /* ADC */
  ADC_Init(ADC1, &RegularADC_ADC_InitStructure);

  /* ADCx regular channel1 configuration *************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_3Cycles);

  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Start ADC Software Conversion */
  ADC_SoftwareStartConv(ADC1);

  /* Wait for ADA1-DMA transfer complete */
  while (!DMA_GetFlagStatus(RegularADC_DMA_STREAM, RegularADC_DMA_TC_FLAG)) ;

  /* Clear */
  DMA_ClearFlag(RegularADC_DMA_STREAM, RegularADC_DMA_TC_FLAG);
}

void disable_RegularADC(void)
{
}

void enable_TimerTimeBaseIRQ(void)
{
  /* Block: TimerTimeBaseIRQ */
  NVIC_InitTypeDef NVIC_InitStructureTimeBaseIRQ_TIM2;

  /* Configure Timer Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  NVIC_InitStructureTimeBaseIRQ_TIM2.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructureTimeBaseIRQ_TIM2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructureTimeBaseIRQ_TIM2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructureTimeBaseIRQ_TIM2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructureTimeBaseIRQ_TIM2);

  /* TIM2 Configuration */
  TIM_PrescalerConfig(TIM2, 0, TIM_PSCReloadMode_Update);
  TIM_SetAutoreload(TIM2, -1);

  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

void disable_TimerTimeBaseIRQ(void)
{
  /* Block: TimerTimeBaseIRQ */

  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, DISABLE);
}

/* ########################################################################
 * Name: <S3>/UART Rx
 * Id: NHAN_DU_LIEUUARTRx
 * ########################################################################
 */

/* Data read structure */
UARTRX_BUFFER_READ_STRUCT NHAN_DU_LIEUUARTRx_read_structure = { 0, 0, (uint8_t *)
  0 };

/* Data */
float NHAN_DU_LIEUUARTRx_data0 = 0;
float NHAN_DU_LIEUUARTRx_data1 = 0;
float NHAN_DU_LIEUUARTRx_data2 = 0;
float NHAN_DU_LIEUUARTRx_data3 = 0;

/* Non-Blocking */
uint8_t NHAN_DU_LIEUUARTRx_Receive(uint8_t *buffer, uint16_t size)
{
  uint8_T ready = 0;
  const char terminator[2] = { 13, 10 };/* Terminator */

  /* Rx Non-Blocking */
  if (UART5_ReadLine(&NHAN_DU_LIEUUARTRx_read_structure, terminator, 2, buffer,
                     size) > 0) {
    ready = (uint8_t)(sscanf((const char*)buffer,
      "a1=%f,a2=%f,a3=%f,vd_cmd=%f\r\n" ,&NHAN_DU_LIEUUARTRx_data0 ,
      &NHAN_DU_LIEUUARTRx_data1 ,&NHAN_DU_LIEUUARTRx_data2 ,
      &NHAN_DU_LIEUUARTRx_data3 ) == 4);
  }

  return ready;
}

/* Enable UART-DMA module */
void enable_NHAN_DU_LIEUUARTRx(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Name: <S4>/UART Tx
 * Id: TRUYEN_DU_LIEUUARTTx
 * ########################################################################
 */

/* Enable UART-DMA module */
void enable_TRUYEN_DU_LIEUUARTTx(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Name: <S4>/UART Tx2
 * Id: TRUYEN_DU_LIEUUARTTx2
 * ########################################################################
 */

/* Enable UART-DMA module */
void enable_TRUYEN_DU_LIEUUARTTx2(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Name: <S4>/UART Tx1
 * Id: TRUYEN_DU_LIEUUARTTx1
 * ########################################################################
 */

/* Enable UART-DMA module */
void enable_TRUYEN_DU_LIEUUARTTx1(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Name: <S4>/UART Tx3
 * Id: TRUYEN_DU_LIEUUARTTx3
 * ########################################################################
 */

/* Enable UART-DMA module */
void enable_TRUYEN_DU_LIEUUARTTx3(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Name: <S4>/UART Tx4
 * Id: TRUYEN_DU_LIEUUARTTx4
 * ########################################################################
 */

/* Enable UART-DMA module */
void enable_TRUYEN_DU_LIEUUARTTx4(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Name: <S4>/UART Tx5
 * Id: TRUYEN_DU_LIEUUARTTx5
 * ########################################################################
 */

/* Enable UART-DMA module */
void enable_TRUYEN_DU_LIEUUARTTx5(void)
{
  WAIJUNG_USARt5_INIT();
}

/* ########################################################################
 * Name: <S4>/UART Tx6
 * Id: TRUYEN_DU_LIEUUARTTx6
 * ########################################################################
 */

/* Enable UART-DMA module */
void enable_TRUYEN_DU_LIEUUARTTx6(void)
{
  WAIJUNG_USARt5_INIT();
}

/* [EOF] */
