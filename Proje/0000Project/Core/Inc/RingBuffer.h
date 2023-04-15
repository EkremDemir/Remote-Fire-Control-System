/*
 * RingBuffer.h
 *
 *  Created on: Mar 24, 2023
 *      Author: ekrm5
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#include "main.h"
#include <stdbool.h>


#define BUFFERSIZE 100


typedef struct
{
	uint8_t buffer[BUFFERSIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
}ringBuffer;



/*Initiliaze ring buffer*/

void Buffer_Init(void);

void SendBuffer1(uint8_t c);
void SendBuffer2(uint8_t c);

void Buffer_Write(void);

void Uart_transmit(ringBuffer *buf);

void Clear_Buffer(ringBuffer *buf);


//char* searchfor(ringBuffer* buf, char* str1, char* str2);

char* searchfor(ringBuffer* buffer, const char* start, const char* end);

bool ringBuffer_contains_substring(ringBuffer *buffer, char *substring);


void Uart_transmit_string(uint8_t *str,uint8_t huart);

#endif /* INC_RINGBUFFER_H_ */
