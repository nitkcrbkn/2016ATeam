/*
 * USART.c
 *
 *  Created on: 2016/06/01
 *      Author: evaota
 */
#include "MW_USART.h"
#include <stdlib.h>

UART_HandleTypeDef huart1 = {
  .Instance = USART1,
  .Init = {
    .BaudRate = 115200,
    .WordLength = UART_WORDLENGTH_8B,
    .StopBits = UART_STOPBITS_1,
    .Parity = UART_PARITY_NONE,
    .Mode = UART_MODE_TX_RX,
    .HwFlowCtl = UART_HWCONTROL_NONE,
    .OverSampling = UART_OVERSAMPLING_16
  },
};

UART_HandleTypeDef huart2 = {
  .Instance = USART2,
  .Init = {
    .BaudRate = 115200,
    .WordLength = UART_WORDLENGTH_8B,
    .StopBits = UART_STOPBITS_1,
    .Parity = UART_PARITY_NONE,
    .Mode = UART_MODE_TX_RX,
    .HwFlowCtl = UART_HWCONTROL_NONE,
    .OverSampling = UART_OVERSAMPLING_16
  },
};

UART_HandleTypeDef huart3 = {
  .Instance = USART3,
  .Init = {
    .BaudRate = 9600,
    .WordLength = UART_WORDLENGTH_8B,
    .StopBits = UART_STOPBITS_1,
    .Parity = UART_PARITY_NONE,
    .Mode = UART_MODE_TX_RX,
    .HwFlowCtl = UART_HWCONTROL_NONE,
    .OverSampling = UART_OVERSAMPLING_16
  },
};

static UART_HandleTypeDef *uartid[3] = {
  &huart1,
  &huart2,
  &huart3
};

void MW_USARTSetBaudRate(usartid_t id, uint32_t BaudRate){
  assert_param(IS_USART_ID(id));
  uartid[(uint32_t)id]->Init.BaudRate = BaudRate;
}

/* USART init function */
int MW_USARTInit(usartid_t id){
  if( HAL_UART_Init(uartid[(uint32_t)id]) != HAL_OK ){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int32_t MW_USART1Transmit(const uint8_t *str, uint16_t datanum){
  if( HAL_UART_Transmit(&huart1, (uint8_t*)str, datanum, 0xFFFF) != HAL_OK ){
    return -1;
  }
  return 0;
}

int32_t MW_USART1Receive(void){
  uint8_t c;
  if( HAL_UART_Receive(&huart1, &c, 1, 100) != HAL_OK ){
    return -1;
  }
  return (uint32_t)c;
}

int32_t MW_USART2Transmit(const uint8_t *str, uint16_t datanum){
  if( HAL_UART_Transmit_DMA(&huart2, (uint8_t*)str, datanum) != HAL_OK ){
    return -1;
  }
  return 0;
}

int32_t MW_USART2Receive(void){
  uint8_t c;
  if( HAL_UART_Receive(&huart2, &c, 1, 100) != HAL_OK ){
    return -1;
  }
  return (uint32_t)c;
}

int32_t MW_USART3Transmit(const uint8_t *str, uint16_t datanum){
  if( HAL_UART_Transmit(&huart3, (uint8_t*)str, datanum, 0xFFFF) != HAL_OK ){
    return -1;
  }
  return 0;
}

int32_t MW_USART3Receive(void){
  uint8_t c;
  if( HAL_UART_Receive(&huart3, &c, 1, 100) != HAL_OK ){
    return -1;
  }
  return (uint32_t)c;
}

int32_t MW_USART3ReceiveMult(int size, const uint8_t *data){
  if( HAL_UART_Receive_DMA(&huart3, (uint8_t*)data, size) != HAL_OK ){
    return -1;
  }
  return 0;
}

