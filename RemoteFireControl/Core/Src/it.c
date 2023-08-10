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
extern StepperMotor stepMotor1; //pan step motor
extern StepperMotor stepMotor2; //tilt step motor
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
void USART2_IRQHandler(void)  // This function handles USART 2 interrupts.
{
	HAL_UART_IRQHandler(&huart2);
}


void TIM2_IRQHandler(void) // This function handles TIMER 2 interrupts.
{
	HAL_TIM_IRQHandler(&htim2);
}


/* EXTI15_10 interrupt handler */
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    delay_us(0);
}


/* This is the interrupt handler function, which gets called when an external interrupt is triggered
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8 | GPIO_PIN_9);
}
*/
