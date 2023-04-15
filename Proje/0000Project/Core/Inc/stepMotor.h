/*
 * stepMotor.h
 *
 *  Created on: Mar 13, 2023
 *      Author: ekrm5
 */

#ifndef INC_STEPMOTOR_H_
#define INC_STEPMOTOR_H_

#include <stdbool.h>
#include "main.h"

typedef struct  {

	//soft varibles
	uint16_t  shaftDirection;    //defines the current direction of the shaft
	uint32_t stepSize;          //define the number of the step of the motor, determines  amount of the displacement of the shaft
	uint32_t usDelay;          //defines delay amount of the between steps, determines the speed of the shaft

	//track position variables

	int32_t netStepSize; //it will helps to find out and track the position of the shaft
    uint8_t moveState; //in Continuous movement it controls the stop and start state  of the motor

	//SET UP
	GPIO_TypeDef* GPIOx;
	uint16_t pin_step;
	uint16_t pin_dir;

}StepperMotor;

void InitializeStepper( StepperMotor *stepper);
void setDirection( StepperMotor *stepper,uint16_t direction);
void moveToX( StepperMotor *stepper,uint16_t stepMove);
void moveTwoMotor( StepperMotor *stepper1, StepperMotor *stepper2,uint16_t stepMove1, uint16_t stepMove2);
void moveContinous(StepperMotor *stepper);
void setSpeed( StepperMotor *stepper,uint16_t speed);
void calaculateNetStep( StepperMotor *stepper,uint16_t step);

#endif /* INC_STEPMOTOR_H_ */
