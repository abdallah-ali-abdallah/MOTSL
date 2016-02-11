#ifndef __UART_POLLING_ARR_CHAR_H
#define __UART_POLLING_ARR_CHAR_H

#include "stm32f10x.h"

  #define USARTy                   USART1
  #define USARTy_GPIO              GPIOA
  #define USARTy_CLK               RCC_APB2Periph_USART1
  #define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USARTy_RxPin             GPIO_Pin_10
  #define USARTy_TxPin             GPIO_Pin_9

  #define BufferSize   20


  void usart_initialize (uint32_t Baudrate,uint16_t Word_length ,uint16_t stop_bits,uint16_t parity,uint16_t hardware_flow_control,uint16_t mode);
  void usart_start();
  void usart_send_char(unsigned char *TX_char);
  unsigned char usart_recieve_char();
  void usart_recieve_buffer(unsigned char *Buffer);
  void usart_send_buffer(unsigned char  *Buffer);


#endif /* __UART_POLLING_ARR_CHAR_H */



