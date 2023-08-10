
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

#include "Helper_Func.h"
#include "ESP_Handle.h"
#include "stepMotor.h"
#include "dcMotor.h"
#include "ServoMotor.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void Error_Handler(void);
void delay_us(uint32_t time);
void delay_ms(uint32_t time);

#define true 1
#define false 0
#endif /* __MAIN_H */
