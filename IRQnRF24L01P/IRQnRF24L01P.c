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


unsigned char IRQnrf=0;

unsigned char data[]={0};
unsigned char R[]={0};




void nRF24L01_SPI_NSS_H(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void nRF24L01_SPI_NSS_L(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}


/**
  * @Function :  nRF24L01_SPI_Send_Byte
  * @brief  Sends an amount of data in non blocking mode.
  * @param  data: pointer to data buffer
  * @param  R: Pointer to receive buffer
  * @param  sizeof_tx: Amount of data to be sent
  * @retval NRF24L01P
  */
void nRF24L01_SPI_Send_Byte(unsigned char* data,unsigned long sizeof_tx){
    nRF24L01_SPI_NSS_L();                    		// Set CSN low, init SPI tranaction
    while(tx_element<=sizeof_tx){
        tx_element++;
        /* Send byte through the SPI2 peripheral */
        SPI_I2S_SendData(SPI2, data[tx_element]);
        }
    nRF24L01_SPI_NSS_H();
}



/**
  * @Function :  nRF24L01_SPI_receive_Byte
  * @brief  receive an amount of data in non blocking mode.
  * @param  R: Pointer to receive buffer
  * @param  sizeof_data: Amount of data to be sent
  * @retval NRF24L01P
  */
void nRF24L01_SPI_receive_Byte(unsigned char* R,unsigned long sizeof_rx){
    nRF24L01_SPI_NSS_L();                    		// Set CSN low, init SPI tranaction
    while(rx_element<=sizeof_rx){
        R[rx_element]=SPI_I2S_ReceiveData(SPI2);
        rx_element++;
        }
    nRF24L01_SPI_NSS_H();
}



/**
  * @Function :  GPIO_Configuration_PinB2
  * @brief  IRQ handler of NRF24l01P on PIN B2
  * @param  External interrupt
  * @retval NRF24L01P
  */

void GPIO_Configuration_PinB2(void){
GPIO_InitTypeDef irq ;
EXTI_InitTypeDef EXTI_InitStructure ;
NVIC_InitTypeDef NVIC_InitStructure ;
irq.GPIO_Pin = GPIO_Pin_2;
irq.GPIO_Speed = GPIO_Speed_10MHz;
irq.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOB, &irq);


GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);
// Configure EXTI1 line
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x07;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    void EXTI1_IRQHandler(void){
        if(EXTI_GetITStatus(EXTI_Line2) != RESET){
               // if(SPI_I2S_IT_TXE==1){
               if(MASK_TX_DS==0){                  //Active low
                    void nRF24L01_SPI_Send_Byte(unsigned char* data,unsigned long sizeof_tx){
                        nRF24L01_SPI_NSS_L();                    		// Set CSN low, init SPI tranaction
                        while(tx_element<=sizeof_tx){
                            tx_element++;
                            /* Send byte through the SPI2 peripheral */
                            SPI_I2S_SendData(SPI2, data[tx_element]);
                            }
                        nRF24L01_SPI_NSS_H();
                        }
                }
                if(MASK_RX_DR==0){
                    void nRF24L01_SPI_receive_Byte(unsigned char* R,unsigned long sizeof_rx){
                        nRF24L01_SPI_NSS_L();                    		// Set CSN low, init SPI tranaction
                            while(rx_element<=sizeof_rx){
                                R[rx_element]=SPI_I2S_ReceiveData(SPI2);
                                rx_element++;
                            }
                        nRF24L01_SPI_NSS_H();
                    }
                }
                if(MASK_MAX_RT==0){;}//when max re transmit occur with out response
        }



            EXTI_ClearITPendingBit(EXTI_Line2);
        }
    }


