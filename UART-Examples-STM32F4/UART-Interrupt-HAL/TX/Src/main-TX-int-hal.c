/*********************************************************************************************************************************
*			                Author : Mostafa Yasin	        	                                                 *								  
*		                        Date   : 30-31/1/2016			                                                 *								  
*		                        Code   : UART HAL code ( Interrupt mode )	 TX			                 *
* ********************************************************************************************************************************     	     
*              modified 4/2/2016                                                                                                 *  
*                                       write a specific function for sending string.                                            *
*              modified 4/2/2016                                                                                                 *
*                                       Remove send_String function and use the simple function                                  *
*                                       from HAL libraries for that ... Look at line 75 or 82 .                                  *
**********************************************************************************************************************************
*              NOTE 1 :-  The higher the baud rate , the higher amount of data will be transfered in less time ,                 *
*                         the slower time to switch among transmittions .                                                        *                                        
*                    ***  Via trails , baud rate 460.8KBps is the best for UART ( 2 MBps is good also but slow )                 *
**********************************************************************************************************************************/		
  
// Includes .
#include "stm32f4xx_hal.h"

// Private variables .
UART_HandleTypeDef huart4;

// Private function prototypes .
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
//static void send_String(uint8_t,unsigned char*);

// Simple delay function .
void delay(__IO uint32_t time)
{ while( time-- ); }

// Variables definitions ---> for sending string.
unsigned char here_s[154] = {'m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.','m','o','s','t','a','f','a',' ','y','a','s','i','n','.'};
unsigned char here_c[154] = {'M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.','M','O','S','T','A','F','A',' ','Y','A','S','I','N','.'};
//unsigned char *text = "abcdep";
//unsigned char *text_1 = "ABCDEP";

/*
// Variables definitions ---> for sending char. .
uint8_t here = 'A';
uint8_t here_1 = 'B';
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
  
  while (1)
  {
    HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_SET );
    delay(1000000);
    HAL_GPIO_WritePin( GPIOD , GPIO_PIN_13 , GPIO_PIN_RESET );
    delay(1000000);
  }
}

  
void EXTI1_IRQHandler(void)
{ 
  HAL_UART_Transmit_IT( &huart4 , here_s , sizeof(here_s) );
  //send_String(sizeof(here_s),here_s);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)
{ 
  HAL_UART_Transmit_IT( &huart4 , here_c, sizeof(here_c) );
  //send_String(sizeof(here_c),here_c);  
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
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
// Send array of char. .
// Connect pin-3 and pin-4 port-D in TX with pin-3 and pin-4 port-D in RX 
void send_String( uint8_t size, unsigned char *array){
      i = 0;
      while(i<size){
        //HAL_GPIO_WritePin( GPIOD , GPIO_PIN_4 , GPIO_PIN_SET);
        HAL_UART_Transmit_IT( &huart4 , &array[i] , 1 );
        i++;
        while(HAL_GPIO_ReadPin( GPIOD , GPIO_PIN_3) == GPIO_PIN_SET);
      }
      //HAL_GPIO_WritePin( GPIOD , GPIO_PIN_4 , GPIO_PIN_RESET);
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

/* UART4 init function */
void MX_UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 460800;     // Kindly,look at node 1 above
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);
}


void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PD1 PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
   /*Configure GPIO pins : PD13 PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

