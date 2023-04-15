/*
 * RingBuffer.c
 *
 *  Created on: Mar 24, 2023
 *      Author: ekrm5
 */



#include "RingBuffer.h"
#include <stdbool.h>

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

extern uint8_t rcvd_byte;
extern bool receive_complete_flag;
extern bool receive_buffer_full_flag;
ringBuffer rx_buffer;
ringBuffer tx_buffer;

void Buffer_Init()
{
	for(int i = 0; i < BUFFERSIZE; ++i)
	{
		rx_buffer.buffer[i] = 0;
		tx_buffer.buffer[i] = 0;
	}
	rx_buffer.head = 0;
	rx_buffer.tail = 0;

	tx_buffer.head = 0;
    tx_buffer.tail = 0;
}


void SendBuffer2(uint8_t c)
{
	if(HAL_UART_Transmit(&huart2,&c,1, HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
}

void SendBuffer1(uint8_t c)
{
	if(HAL_UART_Transmit(&huart1,&c,1, HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
}

void Uart_transmit(ringBuffer *buf)
{
	while(buf->buffer[buf->tail] != buf->buffer[buf->head])
	{
		SendBuffer1(buf->buffer[buf->tail]);
		buf->tail = (buf->tail + 1) %  BUFFERSIZE;
	}
	HAL_UART_Receive_IT(&huart2, &rcvd_byte,1);
}

void Uart_transmit_string(uint8_t *str,uint8_t huart)
{
	if(huart == 1)
	{
		while(*str)
		{
			SendBuffer1(*(str++));
		}
	}
	if(huart == 2)
	{
		while(*str)
		{
			SendBuffer2(*(str++));
		}
	}




	HAL_UART_Receive_IT(&huart2, &rcvd_byte,1);
}

/*
char* searchfor(ringBuffer* buf, char* str1, char* str2)
{
    char* ptr1 = strstr((char*)buf->buffer, str1);
    if (ptr1 == NULL) {
        return NULL;
    }
    ptr1 += strlen(str1);
    char* ptr2 = strstr(ptr1, str2);
    if (ptr2 == NULL) {
        return NULL;
    }
    size_t len = ptr2 - ptr1;
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, ptr1, len);
    result[len] = '\0';
    return result;
}
*/

/*
char* searchfor(ringBuffer* buffer, const char* start, const char* end)
{

	uint16_t i = buffer->tail;
	uint16_t start_len = strlen(start);
	uint16_t end_len = strlen(end);

	while(i != buffer->head)
	{

	}
}

*/


void Clear_Buffer(ringBuffer *buf)
{
	buf->head = 0;
	buf->tail = 0;
	receive_complete_flag = 0;
	receive_buffer_full_flag = 0;
}

char* searchfor(ringBuffer* buffer, const char* start, const char* end)
{
    uint16_t i = buffer->tail;
    uint16_t start_len = strlen(start);
    uint16_t end_len = strlen(end);


    while (i != buffer->head) {
        if (buffer->buffer[i] == start[0] && memcmp(&buffer->buffer[i], start, start_len) == 0)
        {
            i += start_len;
            uint16_t j = i;
            while (j != buffer->head)
            {
                if (buffer->buffer[j] == end[0] && memcmp(&buffer->buffer[j], end, end_len) == 0)
                {
                    uint16_t len = j - i;
                    char* result = (char*)malloc(len + 1);
                    memcpy(result, &buffer->buffer[i], len);
                    result[len] = '\0';
                    return result;
                }
                j = (j + 1) % BUFFERSIZE;
            }
            break;
        }
        i = (i + 1) % BUFFERSIZE;
    }
    return NULL;
    /* Usage EXAMPLE
     *
     * uint8_t *result = (uint8_t*)searchfor(&rx_buffer, "Ekrem", "Demir");
    	if(result != NULL)
    	{
    		Uart_transmit_string(result);
    	}

    	free(result);
     */
}

bool ringBuffer_contains_substring(ringBuffer *buffer, char *substring)
{
    int substring_length = strlen(substring);
    int search_start = buffer->tail;

    while (search_start != buffer->head) {
        int match_length = 0;
        for (int i = 0; i < substring_length; i++) {
            int buffer_index = (search_start + i) % BUFFERSIZE;
            if (buffer->buffer[buffer_index] == substring[i]) {
                match_length++;
            } else {
                break;
            }
        }

        if (match_length == substring_length) {
            return true;
        }

        search_start = (search_start + 1) % BUFFERSIZE;
    }

    return false;

}


