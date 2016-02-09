/*************************************************************************************************************************************
*			                Author : Mostafa Yasin	        	                                                     *								  
*		                        Date   : 30-31/1/2016			                                                     *								  
*		                        Code   : UART HAL code ( Interrupt mode )	 RX			                     *
* ************************************************************************************************************************************     	     
*              modified 4/2/2016                                                                                                     *  
*                                       write a specific function for sending string.                                                *
*              modified 4/2/2016                                                                                                     *
*                                       Remove send_String function and use the simple function                                      *
*                                       from HAL libraries for that ... Look at line 75 or 82 .                                      *
**************************************************************************************************************************************
*              NOTE 1 :-  The higher the baud rate , the higher amount of data will be transfered in less time ,                     *
*                         the slower time to switch among transmittions .                                                            *                                                        
*                    ***  Via trails , baud rate 460.8KBps is the best for UART ( 2 MBps is good also but slow )                     *
**************************************************************************************************************************************/	
  

// Includes .
#include "stm32f4xx_hal.h"


// Private variables .
UART_HandleTypeDef huart4;


// Private function prototypes 
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
//static void receive_String(uint8_t);

// Simple delay function .
void delay(__IO uint32_t time)
{ while( time-- ); }


// Variables definitions ---> for receiving string .
unsigned char string[154];

/*
// Variables definitions ---> for receive char. .
uint8_t here_RX ;
*/

int main(void)
{

  // Reset of all peripherals, Initializes the Flash interface and the Systick .
  HAL_Init();

  // Configure the system clock .
  SystemClock_Config();

  // Initialize all configured peripherals .
  MX_GPIO_Init();
  MX_UART4_Init();
  
  // Initializes the Global MSP .
  HAL_MspInit();
  HAL_UART_MspInit( &huart4 );
  
  //SysTick_Handler();
  //HAL_NVIC_GetActive(UART4_IRQn);
  //HAL_NVIC_GetPendingIRQ(UART4_IRQn);
  //((uint32_t)(UART_CR1_REG_INDEX << 28 | USART_CR1_RXNEIE));
  
  __HAL_UART_ENABLE_IT( &huart4, UART_IT_RXNE);
  HAL_UART_Receive_IT( &huart4 , string , 154 );
  
  while (1)
  {
    HAL_GPIO_WritePin( GPIOD , GPIO_PIN_15 , GPIO_PIN_SET );
    delay(5000000);
    HAL_GPIO_WritePin( GPIOD , GPIO_PIN_15 , GPIO_PIN_RESET );
    delay(5000000);
  }
  
}

void UART4_IRQHandler(void)
{
 HAL_UART_IRQHandler(&huart4);
 
     HAL_UART_Receive_IT( &huart4 , string , 154);  
   
     if( (string[0] == 'm') && (string[152] == 'n') ){
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_SET );
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_14 , GPIO_PIN_RESET );
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_12 , GPIO_PIN_RESET );
    }else if( (string[0] == 'M') && (string[152] == 'N') ){
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_14 , GPIO_PIN_SET );
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_RESET );
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_12 , GPIO_PIN_RESET );
    }else{
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_12 , GPIO_PIN_SET );
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_RESET );
      HAL_GPIO_WritePin( GPIOD , GPIO_PIN_14 , GPIO_PIN_RESET );
    }
 HAL_UART_IRQHandler(&huart4);
}





/*              *********  before 4/2/2016 ************
      ****** These were written inside ISR instead of send_String function.     
i=0;
  while(i<5){
    HAL_UART_Transmit_IT( &huart4 , &here_1[i] , 1 );
    while( HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3)==GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOD , GPIO_PIN_14 , GPIO_PIN_SET );
    i++;
*************************************************************************

                *********  before 4/2/2016 ************ 
// Receive array of char. .
// Connect pin-3 and pin-4 port-D in TX with pin-3 and pin-4 port-D in RX 
void receive_String( uint8_t size ){
      i = 0;
      while(i<size){
        HAL_UART_Receive_IT( &huart4 , &here_RX , 1);
        HAL_GPIO_WritePin( GPIOD , GPIO_PIN_3 , GPIO_PIN_RESET);
        //delay(30);
        HAL_GPIO_WritePin( GPIOD , GPIO_PIN_3 , GPIO_PIN_SET);
        if( here_RX != '\0'){
          string[i] = here_RX;
        }
        i++;
      }
}
*/







// System Clock Configuration .
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


// UART4 init function .
void MX_UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 460800;  // Kindly,look at node 1 above
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);

}


// GPIO Configuratios .
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PD12 PD13 PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}








#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
