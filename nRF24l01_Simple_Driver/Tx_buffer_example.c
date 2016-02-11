/*  By: Abdallah Ali
    Date: 8-1-2016

    Tx_Example, 20 byte buffer that Send 3 different buffers, each one is 20 char
	Tested on STM32F103C8t
	
	Note: The flat STM32F103 has a Led connected to PB0
	and the "breadboard compatible STM32F103 board" has led connected to PC13
	you might need to edit the gpioConfig() function if you want to use them
*/

#include "stm32f10x_RCC.h"
#include "stm32f10x_GPIO.h"
#include "nRF24l01P.h"

/* Variables Deceleration */
unsigned char* buf1 = "Hello funky world !!";
unsigned char* buf2 = "Hello MOTSL world !!";
unsigned char* buf3 = "Hello from MOTSL !!!";

/* Function Deceleration */
void delay(__IO uint32_t timeDelay);		// Simple Delay fucntion
void gpioConfig(void);						// Configure the connected Led & other GPIO
void nrfConfig(void);						// Configure the Radio Module


/* Main Function */
int main(void)
{
    gpioConfig();       // Configure the connected led on PC13 (Not used in this example
    nrfConfig();        // Configure the nrf24l01 to work as Tx

	while(1){
		nRF24L01_TxPacket(buf1);
		nRF24L01_TxPacket(buf2);
		nRF24L01_TxPacket(buf3);
	}

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


void nrfConfig(void){
	nRF24L01_HW_Init();									// Start the nRF24l01
	nRF24L01_Set_TX_Address(0x01,0x23,0x45,0x67,0x89);	// Set the Adress
	nRF24L01_Config(120,P0dBm,R1mbps);					// Maximum power, 1 Mbps, channel = 120
	TX_Mode();											// Enable TX mode
}

