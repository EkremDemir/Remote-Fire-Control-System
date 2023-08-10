/*
 * ServoMotor.h
 *
 *  Created on: Apr 4, 2023
 *      Author: ekrm5
 */

#ifndef INC_SERVOMOTOR_H_
#define INC_SERVOMOTOR_H_

#include "main.h"

void turnForward();
void turnBack();
void stopServo(void);
void setTurnSpeed(uint16_t speed);

#endif /* INC_SERVOMOTOR_H_ */
