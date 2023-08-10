/**
  * @file    ServoMotor.c
  * @brief   Controls a servo motor using PWM signal.
  * @author  ekrm5
  * @date    Apr 4, 2023
  */

#include "ServoMotor.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;

/**
  * @brief  Starts the PWM signal generation on TIM_CHANNEL_2 for forward rotation.
  * @param  None
  * @retval None
  */
void turnForward()
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1550);
}

/**
  * @brief  Starts the PWM signal generation on TIM_CHANNEL_2 for backward rotation.
  * @param  None
  * @retval None
  */
void turnBack()
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1450);
}

/**
  * @brief  Stops the servo motor by stopping the PWM signal generation on TIM_CHANNEL_2.
  * @param  None
  * @retval None
  */
void stopServo(void)
{
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1500);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
}

//1000 max.   1500 min
void setTurnSpeed(uint16_t speed)
{
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,speed);
}
