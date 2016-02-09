/**************************************************************************************************************************
*				Author : Mostafa Yasin									  *								  
*				Date   : 29/1/2016									  *									 
*				Code   : UART CMSIS code ( Polling mode )	 TX					  *
*         modified  2/2/2016                                                                                              *
*                               Increase the budrate from 9600 --> 921600                                                 *    
***************************************************************************************************************************/										



// Includes.
#include "stm32f4xx.h"


// Simple delay function.
void delay( __IO uint32_t time )
{ while(time--); }


// Instances.
GPIO_InitTypeDef  GPIO_LED;  
USART_InitTypeDef USART_InitTX;


int main (void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // enable clock for APB2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE); // port D  enable clk.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // USART 3 enable clk.
  
  
  // GPIO configuration : INPUT : pin_1 , pin_2
  GPIO_LED.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_LED.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_LED);
  
  
  // GPIO configuration : OUTPUT : pin_12
  GPIO_LED.GPIO_Pin   = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_LED);
  
  
  GPIO_PinAFConfig( GPIOD, GPIO_PinSource8, GPIO_AF_USART3 );
  
  // Tx pin confiuration.
  GPIO_LED.GPIO_Pin   = GPIO_Pin_8 ;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_LED.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_LED);
  
  // UART configuration.
  USART_InitTX.USART_BaudRate            = 921600;
  USART_InitTX.USART_WordLength          = USART_WordLength_8b;
  USART_InitTX.USART_StopBits            = USART_StopBits_1;
  USART_InitTX.USART_Parity              = USART_Parity_No;
  USART_InitTX.USART_Mode                = USART_Mode_Tx;
  USART_InitTX.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init( USART3, &USART_InitTX );
  
  USART_Cmd( USART3 ,ENABLE );

          
while(1)
  { 
    if ( GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)== Bit_RESET ){
       USART_SendData( USART3, 'A' );
       GPIO_WriteBit( GPIOD, GPIO_Pin_12, Bit_SET );
       GPIO_WriteBit( GPIOD, GPIO_Pin_13, Bit_RESET );
       while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET ){};
       
    }else if ( GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)== Bit_RESET ){
       USART_SendData( USART3, 'B' );
       GPIO_WriteBit( GPIOD, GPIO_Pin_13, Bit_SET );
       GPIO_WriteBit( GPIOD, GPIO_Pin_12, Bit_RESET );
       while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET ){};
    }  
  }
}
