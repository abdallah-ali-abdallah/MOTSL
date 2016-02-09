
/*************************************************************************************************************************************
*			                Author : Mostafa Yasin	        	                                                     *								  
*		                        Date   : 30-31/1/2016			                                                     *								  
*		                        Code   : UART HAL code ( Polling mode )	 RX	         		                     *      	     
*              modified 2/2/2016                                                                                                     *  
*                                       write a specific function for receiving string.                                              *
*              modified 4/2/2016                                                                                                 *
*                                       Remove send_String function and use the simple function                                  *
*                                       from HAL libraries for that ... Look at line 75 or 82 .                                  *
**********************************************************************************************************************************
*              NOTE 1 :-  The higher the baud rate , the higher amount of data will be transfered in less time ,                 *
*                         the slower time to switch among transmittions .                                                        *                                        
*                    ***  Via trails , baud rate 460.8KBps is the best for UART ( 2 MBps is good also but slow )                 *
**********************************************************************************************************************************/	

// includes .
#include "stm32f4xx_hal.h"


// Private variables .
UART_HandleTypeDef huart4;

// Private function prototypes .
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
//static void receive_String(uint8_t);

// Simple delay function .
void delay(__IO uint32_t time)
{ while(time--); }  

// Variables definitions ---> for receiving string .
//unsigned char string[6];
//uint8_t here_RX;

// Variables definitions ---> for receive char. .
uint8_t here_RX ;

//************************************************************************

int main(void)
{
  // Reset of all peripherals, Initializes the Flash interface and the Systick . 
  HAL_Init();

  // Configure the system clock . 
  SystemClock_Config();

  // Initialize all configured peripherals .
  MX_GPIO_Init();
  MX_UART4_Init();

  // UART pins configuration .
  HAL_MspInit();
  HAL_UART_MspInit( &huart4 );
  
  while (1)
  { 
        HAL_UART_Receive( &huart4 , &here_RX , 1 , 1 );
    
      if( here_RX == 'A' ){
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_SET);
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_14 , GPIO_PIN_RESET);
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_12 , GPIO_PIN_RESET);
      }else if ( here_RX == 'B' ){
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_14 , GPIO_PIN_SET);
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_RESET);
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_12 , GPIO_PIN_RESET);
      }else{
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_12 , GPIO_PIN_SET);
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_RESET);
         HAL_GPIO_WritePin( GPIOD , GPIO_PIN_14 , GPIO_PIN_RESET);
       }
  }

}




/*   *********  before 2/2/2016 ************

int i = 0;
    while(i<5){
      HAL_UART_Receive( &huart4 , &here_RX , 1 , 1 );
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_3 , GPIO_PIN_RESET);
      delay(100);
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_3 , GPIO_PIN_SET);
      string[i] = here_RX;
      i++;
    }
**********************************************************************************
   
   *********  before 2/2/2016 ************
   
// Connect pin-3 port-D in TX with pin-3 port-D in RX 
void receive_String( uint8_t size ){
      i = 0;
      while(i<size){
        HAL_UART_Receive( &huart4 , &here_RX , 1 , 1 );
        HAL_GPIO_WritePin( GPIOD , GPIO_PIN_3 , GPIO_PIN_RESET);
        delay(30);
        HAL_GPIO_WritePin( GPIOD , GPIO_PIN_3 , GPIO_PIN_SET);
        if( here_RX != '\0'){
          string[i] = here_RX;
        }
        i++;
      }
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

/* UART4 init function */
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

// pins configuraton.
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PD12 PD13 PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
   /*Configure GPIO pins : PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}




