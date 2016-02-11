/*
---------------------------------------------------------
    Author:Mohamed Hassan Helal
    Team:  MOTSL TEAM
    library:IRQ nRF24L01P
    Date: 10/2/2016
---------------------------------------------------------
*/

#ifndef IRQNRF24L01P_H_INCLUDED
#define IRQNRF24L01P_H_INCLUDED


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "nrf24l01.h"


/* Functions ------------------------------------------------------------------*/

void nRF24L01_SPI_Send_Byte(unsigned char* data,unsigned long sizeof_data);
void GPIO_Configuration_PinB12(void);

#endif /* IRQNRF24L01P_H_INCLUDED */
