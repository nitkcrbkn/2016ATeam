/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "MW_GPIO.h"
#include "MW_USART.h"
#include "message.h"
#include "MW_I2C.h" 
#include "MW_TIM.h"

/*
 *TODO 
 *wakabayashi
 *  UARTでマルチバイト送信
 *  I2C slaveなしでロジアナで確認...Done
 *  Timer 一定時間でLEDをトグル
 *oishi
 *  UARTの受信チェック
 *　I2Cのチェック
 *　細かいところのチェック...Done
 */
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
extern void MW_TIM1Hadler(void);
extern void MW_TIM2Hadler(void);
void MW_wait(uint32_t wait);
void GPIOInit(void);

int _main(void){
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  
  /*Initialize printf*/
  flush();
  
  GPIOInit();

  /* Initialize all configured peripherals */
  
  MW_SetI2CClockSpeed(I2C1ID,40000);
  MW_I2CInit(I2C1ID);

  MW_SetTIMPrescaler(TIM1ID,10000);
  MW_SetTIMPeriod(TIM1ID,1800*4);
  MW_SetTIMClockDivision(TIM1ID,TIM_CLOCKDIVISION_DIV4);
  MW_TIMInit(TIM1ID);

  MW_TIMStartIT(TIM1ID);
  ENABLETIM1HANDLE();

  MW_USARTInit(USART2ID);
  
  while (1) {
    message("msg","%d",MW_GetTIMCounter(TIM1ID));
    MW_GPIOToggle(GPIOAID,GPIO_PIN_5);
    flush();
    MW_wait(100);
  }
}

void MW_TIM1Hadler(void)
{
  MW_GPIOToggle(GPIOAID,GPIO_PIN_5);
}

void MW_TIM2Hadler(void)
{
}

void MW_wait(uint32_t wait)
{
  uint32_t i,i2;
  for(i2=0;i2<wait;i2++)
    {
      for(i=0;i<1000;i++)
	{
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");
	}
    }
}
