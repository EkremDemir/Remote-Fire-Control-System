/*
 * Helper_Func.c
 *
 *  Created on: Oct 12, 2022
 *      Author: ekrm5
 */



#include "Helper_Func.h"





extern ringBuffer rx_buffer;
int get_id(void);
int get_number(char *str);
int map(int variable, int reference_min,int reference_max,int angle_min,int angle_max);

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;



int counter2 = 0;
char *read =    NULL;




int  get_id(void)
{
    int i =rx_buffer.tail;
    int id = 0;
    while(1)
    {
        if(((rx_buffer.buffer[i]) == 'I') && ((rx_buffer.buffer[i+1]) == 'P') && ((rx_buffer.buffer[i+2]) == 'D'))
        {
             id = (int)(rx_buffer.buffer[i+4]);
             id = id-48;
             break;
        }
        ++i;
    }
    return id;
}



int get_number(char *str)
{
    int num = 0;
    int total = 0;
    int counter;

    for (int i = 0; i < 64; i++)
    {
        if((*(str + i) == '_') && (*(str + i + 2)=='='))
        {
            counter = i + 3;
            while (*(str + counter) >= '0'  &&  (*(str + counter) <= '9'))
            {
                num =(int) (*(str + counter));
                num -=48;
                total = total*10 + num;
                ++counter;
            }

        }
    }

    return total;
}

int map(int variable, int reference_min,int reference_max,int angle_min,int angle_max)
{
    float x;
    x = reference_min + ((float)(reference_max-reference_min)/(angle_max - angle_min)) * variable;

    return (int)x;
}



