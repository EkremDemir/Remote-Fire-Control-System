/*
 * dcMotor.c
 *
 *  Created on: Mar 28, 2023
 *      Author: ekrm5
 */


#include "dcMotor.h"

extern TIM_HandleTypeDef htim2;

void StopDcMotor(void)
{
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
}

void SetDcMotorSpeed(uint16_t speed)
{
	if(speed >  100)
	{
		speed = 100;
	}
	if(speed < 0)
	{
		speed = 0;
	}

	speed = speed * 500;
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed);
	//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed);
}

void StartDcMotor(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,25000);
}
