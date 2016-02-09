/*  By: Abdallah Ali
    Date: 8-1-2016

    Rx_Example, 20 byte buffer that store 20 char
	Tested on STM32F103C8t
	
	Note: The flat STM32F103 has a Led connected to PB0
	and the "breadboard compatible STM32F103 board" has led connected to PC13
	you might need to edit the gpioConfig() function if you want to use them
	
*/

#include "stm32f10x_RCC.h"
#include "stm32f10x_GPIO.h"
#include "nRF24l01P.h"
#include "uart_polling_array_of_char.h"

/* Variables Deceleration */
unsigned char Buf[20];		//Empty buffer to store the received data


/* Function Deceleration */
void delay(__IO uint32_t timeDelay);		// Simple Delay fucntion
void gpioConfig(void);						// Configure the connected Led & other GPIO
void usartConfig(void);						// COnfigure the USART
void nrfConfig(void);						// Configure the Radio Module

int main(void){

    gpioConfig();
    usartConfig();
    nrfConfig();

    while(1){

		if(nRF24L01_RxPacket(Buf))      // This function return 1 when it receive data and store it in the buffer
			usart_send_buffer(Buf);     // Send the buffer to PC over UAART
        }

    return 0;
}

void delay(__IO uint32_t timeDelay) {while(timeDelay--);}


void gpioConfig(void){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	// Enable Clock for the blinking LED
	GPIO_InitTypeDef GPIO_InitStructure;			        // Structure to configre the blinkig LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;              // The required pin
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // Max Speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        // Enable Push Pull
	GPIO_Init(GPIOC, &GPIO_InitStructure);		        	// configure the led now
}

void usartConfig(void){

    usart_initialize(230400, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_HardwareFlowControl_None, (USART_Mode_Tx|USART_Mode_Rx));
    usart_start();
}

void nrfConfig(void){
	nRF24L01_HW_Init();					                // Start the nRF24l01
	nRF24L01_Set_RX_Address(0x01,0x23,0x45,0x67,0x89);	// Set the Adress
	nRF24L01_Config(120,P0dBm,R1mbps);		        	// Maximum power, 1 Mbps, channel = 120
	RX_Mode();						                    // Enable RX Mode
}
