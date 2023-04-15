/*
 * it.c
 *
 *  Created on: Mar 5, 2023
 *      Author: ekrm5
 */


#include "main.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}



void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}


void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
}

