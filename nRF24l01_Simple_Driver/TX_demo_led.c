/*Note: when using the "Flate STM32F103 board - from Amer-Store" 
you need to change the blinking led from GPIOC, PIN13 to GPIOB, PIN0*/


#include "stm32f10x_RCC.h"
#include "stm32f10x_GPIO.h"
#include "nRF24l01P.h"

void NRF_Send(void)
{unsigned char buf[20] = {0};
unsigned char i = 0;
 static unsigned char flag=0;
   if(flag==0)
   {
      flag=1;

      for(i=0;i<20;i++)
    	  buf[i] = 0;
      nRF24L01_TxPacket(buf);

      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
      GPIO_SetBits(GPIOC,GPIO_Pin_13);
   }
   else
   {
      flag=0;

      for(i=0;i<20;i++)
          	  buf[i] = 1;
      nRF24L01_TxPacket(buf);
      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
   }

}


int main(void)
{
	unsigned long i = 100000;	 			// Counter variable used to make some delay

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	// Enable Clock for the blinking LED
	GPIO_InitTypeDef GPIO_InitStructure;			// Structure to configre the blinkig LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);			// configure the led now 



	nRF24L01_HW_Init();					// Start the nRF24l01
	nRF24L01_Set_TX_Address(0x01,0x23,0x45,0x67,0x89);	// Set the Adress
	nRF24L01_Config(120,P0dBm,R1mbps);			// Maximum power, 1 Mbps, channel = 120

	TX_Mode();						// Enable TX mode 
/* Note: the library need to be edited to include the "CE" control pin for TX mode and stand by"*/

    while(1)
    {
    	NRF_Send();		// Call the send function
    	i = 10000000;		// Just a useless delay 
    	while(i--);
    }
}

