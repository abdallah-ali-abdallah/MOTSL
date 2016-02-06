
/*************************************************************************************************************************************
*			                Author : Mostafa Yasin	        	                                                     *								  
*		                        Date   : 30/1/2016			                                                     *								  
*		                        Code   : UART HAL code ( Polling mode )	 TX			                             *      	     
*              modified 2/2/2016                                                                                                     *  
*                                       write a specific function for sending string.                                                *
*              modified 4/2/2016                                                                                                 *
*                                       Remove send_String function and use the simple function                                  *
*                                       from HAL libraries for that ... Look at line 75 or 82 .                                  *
**********************************************************************************************************************************
*              NOTE 1 :-  The higher the baud rate , the higher amount of data will be transfered in less time ,                 *
*                         the slower time to switch among transmittions .                                                        *                                        
*                    ***  Via trails , baud rate 460.8KBps is the best for UART ( 2 MBps is good also but slow )                 *
**********************************************************************************************************************************/	

// Includes.
#include "stm32f4xx_hal.h"


// Private variables
UART_HandleTypeDef huart4;

// Simple delay function
void delay(__IO uint32_t time)
{ while(time--); }

// Private function prototypes 
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
//static void send_String(uint8_t,unsigned char*);

// Variables definitions ---> for sending string.
unsigned char here[6] = {'a','b','c','d','e','\0'};
unsigned char here_1[6] = {'A','B','C','D','E','\0'};

// Variables definitions ---> for sending char. .
//uint8_t here = 'A';
//uint8_t here_1 = 'B';

int i = 0;
int j = 0;
int main(void)
{

  // Reset of all peripherals, Initializes the Flash interface and the Systick. 
  HAL_Init();

  // Configure the system clock 
  SystemClock_Config();

  // Initialize all configured peripherals 
  MX_GPIO_Init();
  MX_UART4_Init();
  
  // UART pins configuration .
  HAL_UART_MspInit(&huart4);
  HAL_MspInit();
  
  while (1)
  {
    if ( HAL_GPIO_ReadPin( GPIOD , GPIO_PIN_1) == GPIO_PIN_RESET ){
      //send_String(sizeof(here),here);
      HAL_UART_Transmit( &huart4 , here , 6 , 1);
      
    }else if ( HAL_GPIO_ReadPin( GPIOD , GPIO_PIN_2) == GPIO_PIN_RESET ){
      //send_String(sizeof(here_1),here_1);
      HAL_UART_Transmit( &huart4 , here_1 , 6 , 1);
    }
  }
}




/*   *********  before 2/2/2016 ************
      j = 0;
      while(j<5){
        HAL_UART_Transmit( &huart4 , &here_1[j] , 1 , 1);
        while(HAL_GPIO_ReadPin( GPIOD , GPIO_PIN_3) == GPIO_PIN_SET);
        j++;

    i = 0;
      while(i<size){
        HAL_UART_Transmit( &huart4 , &array[i] , 1 , 1);
        while(HAL_GPIO_ReadPin( GPIOD , GPIO_PIN_3) == GPIO_PIN_SET);
        i++;
      }
**************************************************************************

******************** before 4/2/2016 *****************
void send_String( uint8_t size, unsigned char *array){
      i = 0;
      while(i<size){
        HAL_GPIO_WritePin( GPIOD , GPIO_PIN_4 , GPIO_PIN_SET);
        HAL_UART_Transmit( &huart4 , &array[i] , 1 , 1);
        while(HAL_GPIO_ReadPin( GPIOD , GPIO_PIN_3) == GPIO_PIN_SET);
        i++;
      }
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_4 , GPIO_PIN_RESET);
}


*/





// System Clock Configuration

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// UART4 init function 
void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;		// Kindly,look at node 1 above
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);

}

// Pins configuration.
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  /*Configure GPIO pin : Green_LED_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD1 PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}








