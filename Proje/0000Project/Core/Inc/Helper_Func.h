/*
 * Helper_Func.h
 *
 *  Created on: Oct 12, 2022
 *      Author: ekrm5
 */

#ifndef INC_HELPER_FUNC_H_
#define INC_HELPER_FUNC_H_

#include "main.h"
#include <stdbool.h>



void clear_Rxbuffer(void);
bool isSubString(char *str11, char *str22);
int get_id(void);
int get_number(char *str);
int map(int variable, int reference_min,int reference_max,int angle_min,int angle_max);

//char* searchfor(char* str, char* start, char* end);


#endif /* INC_HELPER_FUNC_H_ */
