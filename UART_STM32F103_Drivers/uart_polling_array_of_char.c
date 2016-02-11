#include "uart_polling_array_of_char.h"

void usart_initialize (uint32_t Baudrate,uint16_t Word_length ,uint16_t stop_bits,uint16_t parity,uint16_t hardware_flow_control,uint16_t mode)

{
    /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);
  /* Enable USARTy Clock */
  RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);


  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = Baudrate ;
  USART_InitStructure.USART_WordLength = Word_length;
  USART_InitStructure.USART_StopBits = stop_bits;
  USART_InitStructure.USART_Parity = parity;
  USART_InitStructure.USART_HardwareFlowControl = hardware_flow_control;
  USART_InitStructure.USART_Mode = mode;
  USART_Init(USARTy, &USART_InitStructure);

}

void usart_start()
{
    USART_Cmd(USARTy, ENABLE);
}

void usart_send_char(unsigned char* TX_char)
{
    USART_SendData(USARTy, *TX_char);
    while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == 0);
}


unsigned char usart_recieve_char()
{
     while(USART_GetFlagStatus(USARTy, USART_FLAG_RXNE) == 0);

    /* Store the received byte in RxBuffer */
    return (USART_ReceiveData(USARTy));
}

void usart_send_buffer(unsigned char *Buffer)
{
        uint8_t i;
        for(i= 0 ; i < BufferSize ; i++)
              {
                    usart_send_char(Buffer);
                    Buffer++;
              }

}


void usart_recieve_buffer(unsigned char *Buffer)
{
      uint8_t i;
      uint8_t RX_byte;
      for(i= 0 ; i < BufferSize ; i++)
      {
            RX_byte = usart_recieve_char();
            Buffer[i] = RX_byte;
      }

}
