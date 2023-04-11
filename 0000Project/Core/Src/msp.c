/*
 * msp.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ekrm5
 */




#include "main.h"









void HAL_MspInit(void)
{
	//do low level processor specific inits
	//1. set up priority grouping of arm
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable reqired system exception
	SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault

	//3. configure the priority for system exceptions

	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);


}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	//Enable clock paths
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();

	//configure pins for usart pins   PA2->TX, PA3->RX
	GPIO_InitTypeDef  gpio_usart2;

	gpio_usart2.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio_usart2.Mode = GPIO_MODE_AF_PP;
	gpio_usart2.Alternate = GPIO_AF7_USART2;
	gpio_usart2.Pull      = GPIO_PULLUP;
	gpio_usart2.Speed     = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA,&gpio_usart2);


	//configure pins for usart1 pins   PA9->TX, PA10->RX
	GPIO_InitTypeDef  gpio_usart1;

	gpio_usart1.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	gpio_usart1.Mode = GPIO_MODE_AF_PP;
	gpio_usart1.Alternate = GPIO_AF7_USART1;
	gpio_usart1.Pull      = GPIO_PULLUP;
	gpio_usart1.Speed     = GPIO_SPEED_MEDIUM;

	HAL_GPIO_Init(GPIOB,&gpio_usart1);

	//configure the nvic
	HAL_NVIC_SetPriority(USART2_IRQn, 15,1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	HAL_NVIC_SetPriority(USART1_IRQn, 15,0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);


}


//for us and ms
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//Enable clock paths
	__HAL_RCC_TIM6_CLK_ENABLE();

	//configure pins for usart pins   PA2->TX, PA3->RX

	//NOT NEEDED

	//configure the nvic
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15,0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2OC_ch_gpios;

	//1. enable peripheral clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();

	//2. configure gpıos to behave as timer to channel 1,2,3 and 4
	//                              CH1        CH2           CH3         CH4
	tim2OC_ch_gpios.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	tim2OC_ch_gpios.Mode = GPIO_MODE_AF_PP;
	tim2OC_ch_gpios.Alternate = GPIO_AF1_TIM2;
	tim2OC_ch_gpios.Pull      = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&tim2OC_ch_gpios);



	//3. NVIV settings
	HAL_NVIC_SetPriority(TIM2_IRQn, 15,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

