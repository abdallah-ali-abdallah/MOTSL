/**************************************************************************************************************************
*				Author : Mostafa Yasin									  *								  
*				Date   : 29/1/2016									  *									 
*				Code   : UART CMSIS code ( Polling mode )	 RX					  *
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
USART_InitTypeDef USART_InitRX;
uint16_t ch;

int main (void)
{
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE); // port D  enable clk.
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // USART 3 enable clk.
 
 
  // GPIO configuration : OUTPUT : pin_12 , pin_13 
  GPIO_LED.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_12;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_LED.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_LED);

  GPIO_PinAFConfig( GPIOD, GPIO_PinSource9, GPIO_AF_USART3 );
  //Rx pin confiuration.
  GPIO_LED.GPIO_Pin   = GPIO_Pin_9 ;              
  GPIO_LED.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_LED.GPIO_OType = GPIO_OType_PP;
  GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_LED.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_LED);
  
  // UART configuration.
  USART_InitRX.USART_BaudRate            = 921600;
  USART_InitRX.USART_WordLength          = USART_WordLength_8b;
  USART_InitRX.USART_StopBits            = USART_StopBits_1;
  USART_InitRX.USART_Parity              = USART_Parity_No;
  USART_InitRX.USART_Mode                = USART_Mode_Rx;
  USART_InitRX.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init( USART3, &USART_InitRX );
  
  USART_Cmd( USART3 ,ENABLE );

  
          
while(1)
  { 
        ch = USART_ReceiveData(USART3);
    
        if( ch== 'A' ){
          GPIO_WriteBit( GPIOD, GPIO_Pin_12, Bit_SET );
          GPIO_WriteBit( GPIOD, GPIO_Pin_13, Bit_RESET );
        }else if( ch== 'B' ){
          GPIO_WriteBit( GPIOD, GPIO_Pin_13, Bit_SET );
          GPIO_WriteBit( GPIOD, GPIO_Pin_12, Bit_RESET );
        }else{}
    
  }  
}

