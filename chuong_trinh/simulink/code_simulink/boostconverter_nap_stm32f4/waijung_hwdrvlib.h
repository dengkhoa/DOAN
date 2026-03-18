/*
 * File: waijung_hwdrvlib.h
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

#ifndef RTW_HEADER_waijung_hwdrvlib_h_
#define RTW_HEADER_waijung_hwdrvlib_h_
#include <string.h>
#include "rtwtypes.h"
#include "stm32f4xx.h"

/*
 * Define PWM Generation Scale Factor
 * From TIMx_CCRx = TIMx_ARR * (Duty Cycle / 100)
 * TIMx_CCRx = (TIMx_ARR/100) * Duty Cycle
 */
#define FunctionCallSubsystemPBCBasicPWM_TIM4_ARR 4200
#define FunctionCallSubsystemPBCBasicPWM_SF ((float)FunctionCallSubsystemPBCBasicPWM_TIM4_ARR / 100)

/*
 * Bit banding
 *
 * The Cortex memory map includes two bit-band regions.
 * These regions map each word in an alias region of memory to a bit in
 * a bit-band region of memory. Writing to a word in the alias region
 * has the same effect as a read-modify-write operation on the targeted bit
 * in the bit-band region.
 *
 * In the STM32F4xxx both peripheral registers and SRAM are mapped in
 * a bit-band region. This allows single bit-band write and read operations
 * to be performed.
 *
 * A mapping formula shows how to reference each word in the alias region
 * to a corresponding bit in the bit-band region. The mapping formula is:
 * bit_word_addr = bit_band_base + (byte_offset x 32) + (bit_number × 4)
 *
 * BB_base      is the base address of the bit-band region for a register. (PERIPH_BB_BASE at 0x42000000 or SRAM_BB_BASE at 0x22000000)
 * Reg_base     is the base address of a register in the memory region. (PERIPH_BASE at 0x40000000 or SRAM_BASE at 0x20000000)
 * Register     is a register in the memory region that contains the targeted bit.
 * Bit_number   is the targeted bit number (0 - 31).
 *
 * PERIPH_BB_BASE, SRAM_BB_BASE, PERIPH_BASE, and SRAM_BASE are defined in stm32f10x.h
 *
 * Example for determining bit banding bit.
 * Determine DMA Transfer Complete Flag using bit banding.
 * TCIFx is the flag for DMA Channel x
 * This bit is set by hardware. It is cleared by software writing 1 to
 * the corresponding bit in the DMA_IFCR register.
 * TCIFx = 0: No transfer complete (TC) event on channel x
 * TCIFx = 1: A transfer complete (TC) event occurred on channel x
 *
 * DMA Channel:       1 2 3  4  5  6  7
 * Flag bit position: 1 5 9 13 17 21 25 (in DMA_ISR register)
 * (For DMA2 Channel 6 and 7 is irrelevant since it only has 5 channels.)
 *
 * CTCIFx: Channel x transfer complete clear (x = 1 ..7)
 * This bit is set and cleared by software.
 * CTCIFx = 0: No effect
 * CTCIFx = 1: Clear the corresponding TCIF flag in the DMA_ISR register
 *
 */
#define BitBanding(Register, Bit_number, Reg_base, BB_base) ((volatile uint32_t*) ((BB_base) + (((uint32_t) &(Register)) - ((Reg_base))<<5) + ((Bit_number)<<2)))
#define Peripheral_BB(Register, Bit_number) BitBanding(Register, Bit_number, PERIPH_BASE, PERIPH_BB_BASE)
#define SRAM_BB(Register, Bit_number)  BitBanding(Register, Bit_number, SRAM_BASE, SRAM_BB_BASE)

/*
 * Sample usage:
 * USART1->SR can be found from "stm32f4xx.h" and USART_FLAG_TC (6) can be found from "stm32f4xx_usart.h"
 * #define USART1_TC Peripheral_BB(USART1->SR, 6) USART_SR.TC bit
 *
 * Another example:
 * #define GPIOC_8 Peripheral_BB(GPIOC->ODR, 8)
 * *GPIOC_8 = 1;
 */

/*
 * Systick is configured such that the SysTick interrupt every 5.0E-5 sec
 * which is the base sample time of the system.
 */
#define SYSTICKRELOADVALUE             8400

/*
 * HCLK (Hz)
 */
#define HCLK                           168000000

/*
 * Each Systick counter is TIMEPERSYSTICK sec.
 */
#define TIMEPERSYSTICK                 5.0E-5 / 8400

/* ########################################################################
 * Timer
 * ########################################################################
 */
#define SYS_CURRENT_TICK               (SysTick->VAL)
#define SYS_TICKSTEP                   5.0E-5f

typedef struct {
  uint32_t start;
  uint32_t length;
} SYS_TIMER_STRUCT;

#define SYSTIMER_FLAGS_TIMEOUT         0x01

typedef struct {
  uint8_t flags;
  uint32_t tid;                        /* Tick count */
  uint32_t tick;                       /* Capture cuttent tick */
  uint32_t target_us;                  /* Target of systick */
} SYS_TIMER_uS_STRUCT;

extern volatile unsigned int systick_count;
void SysTimer_Start(SYS_TIMER_STRUCT* timer, uint32_t ms);
void SysTimer_uS_Start(SYS_TIMER_uS_STRUCT *timer, uint32_t us);
int SysTimer_IsTimeout(SYS_TIMER_STRUCT* timer);
int SysTimer_uS_IsTimeout(SYS_TIMER_uS_STRUCT* timer);
void SysTimer_delay_us(uint32_t us);

/* ########################################################################
 * Name: <S1>/Regular ADC
 * Id: RegularADC
 * ########################################################################
 */
/* Conversion result */
extern uint16_t RegularADC_ConvertedValue[];

/* ADC1 Configuration */
#define RegularADC_ADC_DR_ADDRESS      ADC1_DR_ADDRESS

/* ADC1 DR Address */
#define ADC1_DR_ADDRESS                ((uint32_t)0x4001204C)

/* DMA */
#define RegularADC_DMA_CHANNEL         DMA_Channel_0
#define RegularADC_DMA_STREAM          DMA2_Stream4
#define RegularADC_DMA_TC_FLAG         DMA_FLAG_TCIF4

extern volatile unsigned int systick_count;
void enable_FunctionCallSubsystemPBCBasicPWM(void);/* FunctionCallSubsystemPBCBasicPWM */

/* ########################################################################
 * UART Utilities for packet processing
 * ########################################################################
 */
/* UART packet process struct */
typedef enum {
  txIdle = 0,
  txBusy
} UART_TX_STATE;

/* Data read structure */
typedef struct {
  uint16_t index;                      /* Index of data in buffer */
  uint16_t count;                      /* Return data count */
  uint8_t *buffer;                     /* Return buffer pointer of valid data */
} UARTRX_BUFFER_READ_STRUCT;

/* ########################################################################
 * Name: <Root>/UART Setup
 * Id: UARTSetup
 * ########################################################################
 */
#define UTX5_BUFFER_SIZE               512

extern uint8_t UART5_Tx_Buffer[];
extern UART_TX_STATE UARTSetup_Tx_State;
extern void UART5_TxUpdate(uint16_t count);

#define URX5_BUFFER_SIZE               512

extern uint8_t UART5_Rx_Buffer[];
extern uint8_t UART5_Temp_Buffer[];
uint16_t UART5_ReadLine(UARTRX_BUFFER_READ_STRUCT *read_struct, const char
  *terminator, uint16_t terminator_count, uint8_t *buffer, uint16_t buffer_size);
uint8_t UART5_ReadBinary(UARTRX_BUFFER_READ_STRUCT *read_struct, const char
  *header, uint16_t header_count, const char *terminator, uint16_t
  terminator_count, uint8_t *buffer, uint16_t data_count);
void UART5_RestoreBytes(UARTRX_BUFFER_READ_STRUCT *read_struct, uint16_t count);
extern uint16_t UART5_GetInitNDTR(void);
void UART5_Read(UARTRX_BUFFER_READ_STRUCT *read_struct);
void UART5_ReadEx(UARTRX_BUFFER_READ_STRUCT *read_struct, uint8_t *buffer,
                  uint16_t buffer_size, uint16_t*reading_count);
extern void UART5_Write(uint8_t* data, uint16_t count);
extern UART_TX_STATE UART5_FlushTxBuffer(void);
void waijung_usart5_initial(void);

#define WAIJUNG_USARt5_INIT()          waijung_usart5_initial()

void enable_UARTSetup(void);

/* ########################################################################
 * Name: <S1>/Regular ADC
 * Id: RegularADC
 * ########################################################################
 */
void enable_RegularADC(void);
void disable_RegularADC(void);
void RegularADCStartConversion(void);
void enable_TimerTimeBaseIRQ(void);    /* TimerTimeBaseIRQ */
void disable_TimerTimeBaseIRQ(void);   /* TimerTimeBaseIRQ */

/* ########################################################################
 * Name: <S3>/UART Rx
 * Id: NHAN_DU_LIEUUARTRx
 * ########################################################################
 */
extern UARTRX_BUFFER_READ_STRUCT NHAN_DU_LIEUUARTRx_read_structure;
uint8_t NHAN_DU_LIEUUARTRx_Receive(uint8_t *buffer, uint16_t size);

/* Data */
extern float NHAN_DU_LIEUUARTRx_data0;
extern float NHAN_DU_LIEUUARTRx_data1;
extern float NHAN_DU_LIEUUARTRx_data2;
extern float NHAN_DU_LIEUUARTRx_data3;

#define NHAN_DU_LIEUUARTRx_terminator_SIZE 2

extern const char NHAN_DU_LIEUUARTRx_terminator[];
void enable_NHAN_DU_LIEUUARTRx(void);

/* ########################################################################
 * Name: <S4>/UART Tx
 * Id: TRUYEN_DU_LIEUUARTTx
 * ########################################################################
 */
void enable_TRUYEN_DU_LIEUUARTTx(void);

/* ########################################################################
 * Name: <S4>/UART Tx2
 * Id: TRUYEN_DU_LIEUUARTTx2
 * ########################################################################
 */
void enable_TRUYEN_DU_LIEUUARTTx2(void);

/* ########################################################################
 * Name: <S4>/UART Tx1
 * Id: TRUYEN_DU_LIEUUARTTx1
 * ########################################################################
 */
void enable_TRUYEN_DU_LIEUUARTTx1(void);

/* ########################################################################
 * Name: <S4>/UART Tx3
 * Id: TRUYEN_DU_LIEUUARTTx3
 * ########################################################################
 */
void enable_TRUYEN_DU_LIEUUARTTx3(void);

/* ########################################################################
 * Name: <S4>/UART Tx4
 * Id: TRUYEN_DU_LIEUUARTTx4
 * ########################################################################
 */
void enable_TRUYEN_DU_LIEUUARTTx4(void);

/* ########################################################################
 * Name: <S4>/UART Tx5
 * Id: TRUYEN_DU_LIEUUARTTx5
 * ########################################################################
 */
void enable_TRUYEN_DU_LIEUUARTTx5(void);

/* ########################################################################
 * Name: <S4>/UART Tx6
 * Id: TRUYEN_DU_LIEUUARTTx6
 * ########################################################################
 */
void enable_TRUYEN_DU_LIEUUARTTx6(void);

#endif                                 /* RTW_HEADER_waijung_hwdrvlib_h_ */

/* [EOF] */
