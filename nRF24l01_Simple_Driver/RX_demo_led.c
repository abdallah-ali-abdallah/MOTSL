#include "stm32f10x_RCC.h"
#include "stm32f10x_GPIO.h"
#include "nRF24l01P.h"

unsigned char Buf[2]= {0};		//Empty buffer to store the recived data

void NRF_Receive(void)
{


   if(nRF24L01_RxPacket(Buf))            
   {
      if( Buf[0]==0 )
      {
         GPIO_SetBits(GPIOC, GPIO_Pin_13);
      }
      else if(  Buf[0]==1 )
      {
         GPIO_ResetBits(GPIOC, GPIO_Pin_13);
      }

   }

}
int main(void)
{
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	// Enable Clock for the blinking LED
	GPIO_InitTypeDef GPIO_InitStructure;			// Structure to configre the blinkig LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);			// configure the led now 
	
	nRF24L01_HW_Init();					// Start the nRF24l01
	nRF24L01_Set_RX_Address(0x01,0x23,0x45,0x67,0x89);	// Set the Adress
	nRF24L01_Config(120,P0dBm,R1mbps);			// Maximum power, 1 Mbps, channel = 120
	RX_Mode();						// Enable RX Mode

    while(1) NRF_Receive();

}
