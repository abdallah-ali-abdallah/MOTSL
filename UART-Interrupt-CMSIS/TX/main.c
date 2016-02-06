/*************************************************************************************************************************************
*			                    Author : Mostafa Yasin									                                             *								  
*		                        Date   : 29/1/2016									                                                 *								  
*		                        Code   : UART CMSIS code ( Interrupt mode )	 TX			                                 		     *			              
**************************************************************************************************************************************/												



// Includes.
#include "stm32f4xx.h"


// Simple delay function.
void delay( __IO uint32_t time )
{ while(time--); }


// Instances.
GPIO_InitTypeDef  GPIO_LED;  
EXTI_InitTypeDef  EXTI_Intr;
NVIC_InitTypeDef  NVIC_InitSw;
USART_InitTypeDef USART_InitTX;


int main (void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // enable clock for APB2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE); // port D  enable clk.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // USART 3 enable clk.
  
  // GPIO configuration : Interrupt : pin_1 , pin_2
  GPIO_LED.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_LED.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_LED);
  
  // GPIO configuration : OUTPUT : pin_12
  GPIO_LED.GPIO_Pin   = GPIO_Pin_12;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_LED);
  
  
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);
  
  EXTI_Intr.EXTI_Line    = EXTI_Line1;
  EXTI_Intr.EXTI_Mode    = EXTI_Mode_Interrupt;
  EXTI_Intr.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_Intr.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_Intr);
  
  EXTI_Intr.EXTI_Line    = EXTI_Line2;
  EXTI_Intr.EXTI_Mode    = EXTI_Mode_Interrupt;
  EXTI_Intr.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_Intr.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_Intr);
  
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  // Nested Vector Interrupt Controller pin_1
  NVIC_InitSw.NVIC_IRQChannel                   = EXTI1_IRQn;
  NVIC_InitSw.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitSw.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitSw.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitSw);
  
  // Nested Vector Interrupt Controller pin_1 EXTI
  NVIC_InitSw.NVIC_IRQChannel                   = EXTI2_IRQn;
  NVIC_InitSw.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitSw.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitSw.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitSw);
  
  
  GPIO_PinAFConfig( GPIOD, GPIO_PinSource8, GPIO_AF_USART3 );
  
  // Tx pin confiuration.
  GPIO_LED.GPIO_Pin   = GPIO_Pin_8 ;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_LED.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_LED);
  
  // UART configuration.
  USART_InitTX.USART_BaudRate            = 9600;
  USART_InitTX.USART_WordLength          = USART_WordLength_8b;
  USART_InitTX.USART_StopBits            = USART_StopBits_1;
  USART_InitTX.USART_Parity              = USART_Parity_No;
  USART_InitTX.USART_Mode                = USART_Mode_Tx;
  USART_InitTX.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init( USART3, &USART_InitTX );
  
  USART_Cmd( USART3 ,ENABLE );

          
while(1)
  { 
      GPIO_WriteBit( GPIOD, GPIO_Pin_12, Bit_SET );
      delay(5000000);
      GPIO_WriteBit( GPIOD, GPIO_Pin_12, Bit_RESET );
      delay(5000000);
  }  
}



//  ISR 1
void EXTI1_IRQHandler(void){
  if(EXTI_GetITStatus(EXTI_Line1) != RESET){
    USART_SendData( USART3, 'A' );
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET ){};
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}


// ISR 2
void EXTI2_IRQHandler(void){
  if(EXTI_GetITStatus(EXTI_Line2) != RESET){
    USART_SendData( USART3, 'B' );
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET ){};
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}



