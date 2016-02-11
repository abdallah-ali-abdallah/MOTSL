/*
---------------------------------------------------------
    Author:Mohamed Hassan Helal
    Team:  MOTSL TEAM
    library:IRQ nRF24L01P
    Date: 10/2/2016
---------------------------------------------------------
*/


#include "IRQnRF24l01P.h"


unsigned long tx_element=0;
unsigned long rx_element=0;

unsigned long sizeof_tx=5;
unsigned long sizeof_rx=5;

unsigned char IRQnrf=0;

unsigned char data[]={0};
unsigned char R[]={0};



/**
  * @Function :  nRF24L01_SPI_Send_Byte
  * @brief  Sends an amount of data in non blocking mode.
  * @param  data: pointer to data buffer
  * @param  R: Pointer to receive buffer
  * @param  sizeof_data: Amount of data to be sent
  * @retval NRF24L01P
  */

void nRF24L01_SPI_Send_Byte(unsigned char* data,unsigned long sizeof_data){
    while(tx_element<=sizeof_data){
        tx_element++;
        /* Send byte through the SPI2 peripheral */
        SPI_I2S_SendData(SPI2, data[tx_element]);
        R[rx_element]=SPI_I2S_ReceiveData(SPI2);
        rx_element++;
        /* Return the byte read from the SPI bus */
        }
}


/**
  * @Function :  GPIO_Configuration_PinB12
  * @brief  IRQ handler of NRF24l01P on PIN B12
  * @param  External interrupt
  * @retval NRF24L01P
  */

void GPIO_Configuration_PinB12(void){
GPIO_InitTypeDef irq ;
EXTI_InitTypeDef EXTI_InitStructure ;
NVIC_InitTypeDef NVIC_InitStructure ;
irq.GPIO_Pin = GPIO_Pin_12;
irq.GPIO_Speed = GPIO_Speed_10MHz;
irq.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOB, &irq);


GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
// Configure EXTI1 line
  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x07;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    void EXTI1_IRQHandler(void){
        if(EXTI_GetITStatus(EXTI_Line12) != RESET){
               // if(SPI_I2S_IT_TXE==1){
               if(MASK_TX_DS==0||MASK_RX_DR==0){                  //Active low
                     nRF24L01_SPI_Send_Byte(data,sizeof_tx);
                }
        }
        if(MASK_MAX_RT==0){;}//when max re transmit occur with out response


            EXTI_ClearITPendingBit(EXTI_Line12);
        }
    }


