/*
 * stepMotor.h
 *
 *  Created on: Mar 18, 2023
 *      Author: Ekrem Demir
 */

#include "stepMotor.h"
#include "main.h"


#define antiClockWise HIGH

/**
  * @brief  Initialize Stepper Motors
  *
  * @note   This function takes an step motor and initialize it for starting
  *
  * @param  stepper  defines a stepper motor structure
  *
  * @arg  usDelay defines time between two consecutive step of motor, its minimum value is 400 us
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  stepSize  defines NEMA17 step motor step size, taken from datasheet
  *
  * @param  moveState  holds   stop/start state of  the motor in continuous movement, 0-> stop, 1-> start
  *
  * @func   setDirection  Initialize the starting direction of stepper motor in clockwise direction

  */

void InitializeStepper( StepperMotor *stepper)
{
	stepper->usDelay = 500;
	stepper->stepSize = 200;

	setDirection(stepper,0);
	stepper->moveState = false;

}




void setDirection( StepperMotor *stepper,uint16_t direction)
{
	stepper->shaftDirection = direction;
	HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_dir, stepper->shaftDirection);
	delay_us(stepper->usDelay);
}

//void moveAngle()






void setSpeed( StepperMotor *stepper, uint16_t speed)
{
	stepper->usDelay = 500 + ((2000-500)/(10 - 1) * speed);
}


void moveContinous(StepperMotor *stepper)
{
	while(stepper->moveState == 0)
	{
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_SET);
		delay_us(stepper->usDelay);
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_RESET);
		delay_us(stepper->usDelay);
	}
}



void moveTwoMotor( StepperMotor *stepper1, StepperMotor *stepper2,uint16_t stepMove1, uint16_t stepMove2)
{
	uint16_t step1 = stepMove1 / 0.9; //calculate number of steps that motor1 moves using angle info
	uint16_t step2 = stepMove2 / 0.9; //calculate number of steps that motor2 moves using angle info

	calaculateNetStep(stepper1,step1); //calculate Net position
	calaculateNetStep(stepper2,step2); //calculate Net position

	// divider = 2;
	while(step1 > 0 || step2 > 0)
	{
		if(step1 > 0)
		{
			HAL_GPIO_WritePin(stepper1->GPIOx, stepper1->pin_step, GPIO_PIN_SET);
			delay_us( (stepper1->usDelay) );
			HAL_GPIO_WritePin(stepper1->GPIOx, stepper1->pin_step, GPIO_PIN_RESET);
			//delay_us((stepper1->usDelay) / divider);
			step1--;
		}
		else
		{
			//divider = 1;
			delay_us( (stepper1->usDelay) );
		}

		if(step2 > 0)
		{
			HAL_GPIO_WritePin(stepper2->GPIOx, stepper2->pin_step, GPIO_PIN_SET);
			delay_us((stepper2->usDelay) );
			HAL_GPIO_WritePin(stepper2->GPIOx, stepper2->pin_step, GPIO_PIN_RESET);
			//delay_us((stepper2->usDelay) / divider);
			step2--;
		}
		else
		{
			//divider = 1;
			delay_us( (stepper1->usDelay) );
		}


	}
}






//move to desired angle
void moveToX( StepperMotor *stepper,uint16_t stepMove)
{
	uint16_t step = stepMove / 0.9; //calculate number of steps that motor moves using angle info

	calaculateNetStep(stepper,step); //calculate Net position

	for(; step > 0; step--)
	{
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_SET);
		delay_us(stepper->usDelay);
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_RESET);
		delay_us(stepper->usDelay);
	}

}


void calaculateNetStep( StepperMotor *stepper,uint16_t step)
{
	int sign = 1;
	if(stepper->shaftDirection) //used for adition/subtraction of shaft net position
	{
		sign = -1;
	}
	stepper->netStepSize = (stepper->netStepSize + (sign * step)) / 400;  //calculate shaft net position
}



