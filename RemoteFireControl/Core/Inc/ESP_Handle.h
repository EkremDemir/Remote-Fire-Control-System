/*
 * ESP_Handle.h
 *
 *  Created on: Mar 25, 2023
 *      Author: ekrm5
 */

#ifndef INC_ESP_HANDLE_H_
#define INC_ESP_HANDLE_H_

void ESP_Init(void);
void ESP_Server_Start(void);
int  ESP_Server_Send(char *str, int id);
int  ESP_Server_Send1(char *str, int id);
void clearBuf();
int containsString(const char *string1, const char *string2);
void conditions(void);

#endif /* INC_ESP_HANDLE_H_ */
