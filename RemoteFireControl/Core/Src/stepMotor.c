/**
  * @file    stepMotor.h
  * @brief   Provides functions to handle stepper motors.
  * @author  Ekrem Demir
  * @date    Mar 18, 2023
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


/**
  * @brief  Initializes the stepper motor.
  * @param  *stepper Pointer to the stepper motor structure.
  * @retval None
  */
void InitializeStepper( StepperMotor *stepper)
{
	stepper->usDelay = 100;
	stepper->stepSize = 200;

	setDirection(stepper,0);
	stepper->moveState = false;

}



/**
  * @brief  Sets the direction of the stepper motor.
  * @param  *stepper Pointer to the stepper motor structure.
  * @param  direction Desired direction of rotation.
  * @retval None
  */
void setDirection( StepperMotor *stepper,uint16_t direction)
{
	stepper->shaftDirection = direction;
	HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_dir, stepper->shaftDirection);
	delay_us(stepper->usDelay);
}

/**
  * @brief  Sets the speed of the stepper motor.
  * @param  *stepper Pointer to the stepper motor structure.
  * @param  speed Desired speed of rotation.
  * @retval None
  */
void setSpeed( StepperMotor *stepper, uint16_t speed)
{
	stepper->usDelay = speed;
}

/**
  * @brief  Moves the stepper motor continuously.
  * @param  *stepper Pointer to the stepper motor structure.
  * @retval None
  */
void moveContinous(StepperMotor *stepper)
{
	// To disable the interrupt
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	stepper->moveState = 1;
  	while(stepper->moveState == 1)
	{
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_SET);
		delay_us(stepper->usDelay);
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_RESET);
		delay_us(stepper->usDelay);
	}
}


/**
  * @brief  Moves two stepper motors for a specified number of steps.
  * @param  *stepper1 Pointer to the first stepper motor structure.
  * @param  *stepper2 Pointer to the second stepper motor structure.
  * @param  stepMove1 Number of steps for the first motor.
  * @param  stepMove2 Number of steps for the second motor.
  * @retval None
  */
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

/**
  * @brief  Moves two stepper motors continuously.
  * @param  *stepper1 Pointer to the first stepper motor structure.
  * @param  *stepper2 Pointer to the second stepper motor structure.
  * @retval None
  */
void moveTwoMotorContinous( StepperMotor *stepper1, StepperMotor *stepper2)
{
	// To disable the interrupt
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	stepper1->usDelay = 325;
	stepper2->usDelay = 325;
	stepper1->moveState = 1;
	stepper2->moveState = 1;
	uint8_t  stopFlag = 0;

	while(stepper1->moveState )
	{

		HAL_GPIO_WritePin(stepper1->GPIOx, stepper1->pin_step, GPIO_PIN_SET);
		delay_us( (stepper1->usDelay) );
		HAL_GPIO_WritePin(stepper1->GPIOx, stepper1->pin_step, GPIO_PIN_RESET);
		//delay_us((stepper1->usDelay) / divider);
		//divider = 1;
		delay_us((stepper1->usDelay));

		if(!stepper2->moveState)
		{

			if(!stopFlag)
			{
				stepper1->usDelay += stepper1->usDelay;
			}
			stopFlag = 1;
			continue;
		}
		HAL_GPIO_WritePin(stepper2->GPIOx, stepper2->pin_step, GPIO_PIN_SET);
		delay_us((stepper2->usDelay) );
		HAL_GPIO_WritePin(stepper2->GPIOx, stepper2->pin_step, GPIO_PIN_RESET);
		//delay_us((stepper2->usDelay) / divider);

		//divider = 1;
		delay_us( (stepper1->usDelay) );
	}
	// To disable the interrupt
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}

void moveTwoMotorContinous_alter( StepperMotor *stepper1, StepperMotor *stepper2)
{
	// To disable the interrupt
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	stepper1->moveState = 1;
	stepper2->moveState = 1;


	while(stepper1->moveState )
	{

		HAL_GPIO_WritePin(stepper1->GPIOx, stepper1->pin_step, GPIO_PIN_SET);
		HAL_GPIO_WritePin(stepper2->GPIOx, stepper2->pin_step, GPIO_PIN_SET);
		delay_us( (stepper1->usDelay) );
		HAL_GPIO_WritePin(stepper1->GPIOx, stepper1->pin_step, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(stepper2->GPIOx, stepper2->pin_step, GPIO_PIN_RESET);
		delay_us((stepper1->usDelay));
		if(!stepper2->moveState)
		{
			if(!stepper1->moveState)
			{
				break;
			}
            setSpeed(stepper1, 40);
			moveContinous(stepper1);
		}
	}
	// To disable the interrupt

}



/**
  * @brief  Moves the stepper motor to a specified angle.
  * @param  *stepper Pointer to the stepper motor structure.
  * @param  stepMove Desired angle to move to.
  * @retval None
  */
void moveToX( StepperMotor *stepper,uint16_t stepMove)
{
	//uint16_t step = stepMove / 0.9; //calculate number of steps that motor moves using angle info

	//calaculateNetStep(stepper,step); //calculate Net position

	for(; stepMove > 0; stepMove--)
	{
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_SET);
		delay_us(stepper->usDelay);
		HAL_GPIO_WritePin(stepper->GPIOx, stepper->pin_step, GPIO_PIN_RESET);
		delay_us(stepper->usDelay);
		--stepMove;
	}
}

/**
  * @brief  Calculates the net steps the stepper motor needs to take.
  * @param  *stepper Pointer to the stepper motor structure.
  * @param  step Number of steps.
  * @retval None
  */
void calaculateNetStep( StepperMotor *stepper,uint16_t step)
{
	int sign = 1;
	if(stepper->shaftDirection) //used for addition/subtraction of shaft net position
	{
		sign = -1;
	}
	stepper->netStepSize = (stepper->netStepSize + (sign * step)) / 400;  //calculate shaft net position
}



