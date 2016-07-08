#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "SystemTaskManager.h"
#include "MW_USART.h"
#include "message.h"
#include "MW_I2C.h" 
#include "MW_GPIO.h"
#include "DD_Gene.h"
#include "app.h"

volatile uint32_t SY_systemCounter;
volatile uint8_t g_data[10];

int SY_Initialize(void);
int SY_I2CConnectionTest(int timeout);
int SY_WaitForRC(int timeout);
int SY_ApplicationTask(void);
void SY_SystemClock_Config(void);
void SY_GPIOInit(void);
int SY_DevDriverTasks(void);

int main(void){
  int ret;

  ret = SY_Initialize();
  if(ret){
    message("err","initialize Faild%d",ret);
    return EXIT_FAILURE;
  }
  ret = SY_I2CConnectionTest(10);
  if(ret){
    message("err","I2CConnectionTest Faild%d",ret);
    return EXIT_FAILURE;
  }
  ret = SY_WaitForRC(10000);//timeout 10000[ms].
  if(ret){
    message("err","WaitForRC Faild%d",ret);
    return EXIT_FAILURE;
  }
  SY_systemCounter = 0;  

  MW_USART3ReceiveMult(8,g_data);

  while(1){
    MW_printf("%d\n",SY_systemCounter);
    SY_ApplicationTask();
    if(SY_systemCounter % 30 == 0){
      flush();//out message.
    }
    while(SY_systemCounter%_INTERVAL_MS!=_INTERVAL_MS/2-1);
    ret = SY_DevDriverTasks();
    if(ret){
      message("err","Device Driver Tasks Faild%d",ret);
    }
    while(SY_systemCounter%_INTERVAL_MS!=0);
  }
}

int SY_ApplicationTask(void){
  return appTask();
}

int SY_DevDriverTasks(void){
  return DD_Tasks();
}

int SY_WaitForRC(int timeout){
  UNUSED(timeout);
  return EXIT_SUCCESS;
}

int SY_I2CConnectionTest(int timeout){
  UNUSED(timeout);
  return EXIT_SUCCESS;
}

int SY_Initialize(void){
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SY_SystemClock_Config();

  /*Initialize printf null transit*/
  flush();
  
  /*Initialize GPIO*/
  SY_GPIOInit();

  /* Initialize all configured peripherals */
  MW_SetI2CClockSpeed(I2C1ID,40000);
  MW_I2CInit(I2C1ID);

  /*UART initialize*/
  MW_USARTInit(USART2ID);
  MW_USARTInit(USART3ID);

  return EXIT_SUCCESS;
}

void SY_SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      //    Error_Handler();
    }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      //    Error_Handler();
    }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      //    Error_Handler();
    }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void SY_GPIOInit(void)
{
  ENABLECLKGPIOA();
  ENABLECLKGPIOB();
  ENABLECLKGPIOD();
  
  /*Configure GPIO pin : PC13 */
  MW_SetGPIOPin(GPIO_PIN_13);
  MW_SetGPIOMode(GPIO_MODE_INPUT);
  MW_SetGPIOPull(GPIO_NOPULL);
  MW_GPIOInit(GPIOCID);

  /*Configure GPIO pin : PA5 */
  MW_SetGPIOPin(GPIO_PIN_5);
  MW_SetGPIOMode(GPIO_MODE_OUTPUT_PP);
  MW_SetGPIOSpeed(GPIO_SPEED_FREQ_LOW);
  MW_GPIOInit(GPIOAID);

  /*Configure GPIO pin : PC4 */
  MW_SetGPIOPin(GPIO_PIN_4);
  MW_SetGPIOMode(GPIO_MODE_IT_RISING);
  MW_SetGPIOPull(GPIO_NOPULL);
  MW_GPIOInit(GPIOCID);

  /*Configure GPIO pin Output Level */
  MW_GPIOWrite(GPIOAID,GPIO_PIN_5,0);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
  MW_USART3ReceiveMult(8,g_data);
  message("msg","recieveData%x",g_data[0]);
}
