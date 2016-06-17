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
void MW_USARTInit(usartid_t id);

void MW_SetUSARTMode(usartid_t id,uint32_t Mode);
void MW_SetHwFlowCtl(usartid_t id,uint32_t HwFlowCtl);
void MW_SetParity(usartid_t id,uint32_t Parity);
void MW_SetStopBits(usartid_t id,uint32_t StopBits);
void MW_SetWordLength(usartid_t id,uint32_t WordLength);
void MW_SetBaudRate(usartid_t id,uint32_t BaudRate);

int32_t MW_USART1Transmit(uint8_t c);
int32_t MW_USART1Receive(void);
int32_t MW_USART2Transmit(uint8_t c);
int32_t MW_USART2Receive(void);
int32_t MW_USART3Transmit(uint8_t c);
int32_t MW_USART3Receive(void);

#endif /* MIDLEWARE_UART_H_ */
