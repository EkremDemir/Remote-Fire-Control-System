/**
  * @file    dcMotor.c
  * @brief   Controls a DC motor using PWM signal.
  * @author  ekrm5
  * @date    Mar 28, 2023
  */

#include "dcMotor.h"

extern TIM_HandleTypeDef htim2;

/**
  * @brief  Stops the DC motor by setting compare values of TIM_CHANNEL_1 and TIM_CHANNEL_2 to 0.
  * @param  None
  * @retval None
  */
void StopDcMotor(void)
{
	//Make pulse value of pwm as zero to stop.
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);

	//Stop also pwm module
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
}

/**
  * @brief  Sets the speed of the DC motor, values are constrained between 0 and 100.
  * @param  speed Speed value to set. If provided value is not between 0 and 100, it is set to the closest limit.
  * @retval None
  */
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

	//Adjust pulse value of pwm to get desired dc motor speed
	speed = speed * 450;
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed);
}

/**
  * @brief  Starts the DC motor by starting the PWM signal generation on TIM_CHANNEL_1.
  * @param  None
  * @retval None
  */
void StartDcMotor(void)
{

	//Start the dc motor with default speed with 50% duty cycle
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,20000);
	//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,20000);

	//HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,25000);
	//for(int i = 0; i < 25000; i = i + 400)
	//{
		//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,i);
		//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,i);
		//delay_ms(100);
	//}
}
