/*
 * USART.h
 *
 *  Created on: 2016/06/01
 *      Author: evaota
 */

#ifndef MIDLEWARE_UART_H_
#define MIDLEWARE_UART_H_

#include "stm32f1xx_hal.h"

typedef enum
{
	USART1ID = 0,
	USART2ID = 1,
	USART3ID = 2
}usartid_t;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/**Sample code(main)
 *
 *
 *
 *
 *
 */

/******************************* USART ID *******************************/
#define IS_USART_ID(ID) (((ID) == USART1ID) || \
						 ((ID) == USART2ID) || \
						 ((ID) == USART3ID))

/**USART1 GPIO Configuration
PA9     ------> USART1_TX
PA10     ------> USART1_RX
*/

/**USART2 GPIO Configuration
PA2     ------> USART2_TX
PA3     ------> USART2_RX
*/

/**USART3 GPIO Configuration
PC10     ------> USART3_TX
PC11     ------> USART3_RX
**/

/* USART init function */
int MW_USARTInit(usartid_t id);

void MW_USARTSetBaudRate(usartid_t id,uint32_t BaudRate);

int32_t MW_USART1Transmit(const uint8_t *str,uint16_t datanum);
int32_t MW_USART1Receive(void);
int32_t MW_USART2Transmit(const uint8_t *str,uint16_t datanum);
int32_t MW_USART2Receive(void);
int32_t MW_USART3Transmit(const uint8_t *str,uint16_t datanum);
int32_t MW_USART3Receive(void);
int32_t MW_USART3ReceiveMult(int size,const uint8_t *data);

#endif /* MIDLEWARE_UART_H_ */
