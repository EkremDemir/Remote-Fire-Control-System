/*
 * ESP_Handle.c
 *
 *  Created on: Mar 25, 2023
 *      Author: ekrm5
 */

#include "ESP_Handle.h"
#include "main.h"
#include "stdio.h"

extern ringBuffer rx_buffer;
extern ringBuffer tx_buffer;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

extern bool receive_complete_flag;


#define ssid "eko"
#define psw  "ekosyf66"

extern bool receive_complete_flag;
extern bool receive_buffer_full_flag;
extern uint8_t rcvd_byte;

char *dataSend = "<!DOCTYPE HTML><html><head>\
    <title>ESP Input Form</title>\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
    </head><body>\
    <br><a href=\" ?pin=on\"><button type=\"button\"> LED ON</button></a><br>\
    <br><a href=\" ?pin=off\"><button type=\"button\">LED OFF</button></a><br><br>\
    <form action=\"/get\">\
      Motor_1 angle: <input type=\"text\" name=\"motor_a\">\
      <input type=\"submit\" value=\"Submit\">\
    </form><br>\
    <form action=\"/get\">\
      Motor_2 angle: <input type=\"text\" name=\"motor_b\">\
      <input type=\"submit\" value=\"Submit\">\
    </form><br>\
  </body></html>";

void ESP_Init(void)
{

/***********RESETTING**************************************************/
	Uart_transmit_string((uint8_t*)"AT+RST\r\n",2);
    HAL_Delay(3000); //Wait 2 second to reset the esp
    //while(!ringBuffer_contains_substring(&rx_buffer,"OK"));
    Clear_Buffer(&rx_buffer);

/**************************CHECK it is run or not**************************************************/
    Uart_transmit_string((uint8_t*)"AT\r\n",2);
    HAL_Delay(500);
    while(!ringBuffer_contains_substring(&rx_buffer,"OK"));
    Clear_Buffer(&rx_buffer);


/**************************Set ESP as client mode**************************************************/
    Uart_transmit_string((uint8_t*)"AT+CWMODE=1\r\n",2);
    while(!ringBuffer_contains_substring(&rx_buffer,"OK"));
    Clear_Buffer(&rx_buffer);



    /**************************Set ESP as client mode**************************************************/
    Uart_transmit_string((uint8_t*)"AT+CIFSR\r\n",2);
    HAL_Delay(500);
    while(!ringBuffer_contains_substring(&rx_buffer,"OK"));
    Clear_Buffer(&rx_buffer);


/**************************Handle the WIFI connection**************************************************/
   // sprintf(tx_buffer, ("AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,psw);
/*
    Uart_transmit_string((uint8_t*)"AT+CWJAP=\"eko\",\"ekosyf66\"\r\n",2);
    HAL_Delay(3000);
    while(!ringBuffer_contains_substring(&rx_buffer,"OK"));
    Clear_Buffer(&rx_buffer); // clear garbage value of received from esp


    Uart_transmit_string((uint8_t*)"Connected\r\n",1);
    HAL_Delay(100);

*/


/**************************Enable Multiple Connection**************************************************/

    Uart_transmit_string((uint8_t*)"AT+CIPMUX=1\r\n",2);
    HAL_Delay(500);
    while(!ringBuffer_contains_substring(&rx_buffer,"OK"));
    Clear_Buffer(&rx_buffer); // clear garbage value of received from esp


/**************************Enable Multiple Connection**************************************************/
    Uart_transmit_string((uint8_t*)"AT+CIPSERVER=1,80\r\n",2);
    HAL_Delay(500);
    while(!ringBuffer_contains_substring(&rx_buffer,"OK"));
    Clear_Buffer(&rx_buffer); // clear garbage value of received from esp
}


void ESP_Server_Start(void)
{

	int id = 0;


    while(!ringBuffer_contains_substring(&rx_buffer,"HTTP"));

    id = get_id();
    //while()



    //Uart_transmit(&rx_buffer);
    HAL_Delay(1000);

    if(ringBuffer_contains_substring(&rx_buffer,"pin=on"))
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);


    }
    else if(ringBuffer_contains_substring(&rx_buffer,"pin=off"))
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);



    }
    else if(ringBuffer_contains_substring(&rx_buffer, "motor_a"))
    {

    }
    else if(ringBuffer_contains_substring(&rx_buffer, "motor_b"))
    {



    }

    else if((ringBuffer_contains_substring(&rx_buffer, "favicon.ico")) == 1)
    {


    }

    else
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);


    }


    ESP_Server_Send(dataSend,id);



}

int ESP_Server_Send(char *str, int id)
{
    int len = strlen(str);
    char data[100] = {0};
    Clear_Buffer(&rx_buffer);;

    //HAL_UART_Transmit_IT(&huart2,(uint8_t*)rx_buffer.buffer,BUFFER_SIZE);
    sprintf(data, "AT+CIPSEND=%d,%d\r\n",id,len);
    HAL_Delay(50);
    Clear_Buffer(&rx_buffer);
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)data, strlen(data));
    HAL_Delay(50);
    while(!ringBuffer_contains_substring(&rx_buffer,">"))
    {

        if(ringBuffer_contains_substring(&rx_buffer,"ERROR"))
        {
            HAL_UART_Transmit_IT(&huart2,(uint8_t*)str, strlen(data));
            HAL_Delay(50);

            break;
        }
    }
    //while(!isSubString(Rx_Buffer,">"));
    //HAL_UART_Transmit_IT(&huart2,(uint8_t*)Rx_Buffer,BUFFER_SIZE);
    //HAL_Delay(50);
    Clear_Buffer(&rx_buffer);


    HAL_UART_Transmit_IT(&huart2,(uint8_t*)str, strlen(str));
    HAL_Delay(50);
    while(!ringBuffer_contains_substring(&rx_buffer,"SEND OK"))
    {

        if(ringBuffer_contains_substring(&rx_buffer,"ERROR"))
        {
            HAL_UART_Transmit_IT(&huart2,(uint8_t*)str, strlen(str));
            HAL_Delay(50);

            break;
        }
    }
    //HAL_UART_Transmit_IT(&huart1,(uint8_t*)str, strlen(str));
    //while(!isSubString(Rx_Buffer,"SEND OK"));





    Clear_Buffer(&rx_buffer);
    sprintf(data, "AT+CIPCLOSE=%d\r\n",id);
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)data, strlen(data));
    while(!ringBuffer_contains_substring(&rx_buffer,"OK"))
    {

        if(ringBuffer_contains_substring(&rx_buffer,"ERROR"))
        {
            HAL_UART_Transmit_IT(&huart2,(uint8_t*)str, strlen(data));
            HAL_Delay(50);
            break;
        }
    }
    Clear_Buffer(&rx_buffer);
    return 1;

}



int ESP_Server_Send1(char *str, int id)
{


	int len = strlen(str);
    Clear_Buffer(&rx_buffer);


    sprintf(tx_buffer.buffer, "AT+CIPSEND=%d,%d\r\n",id,len);
    Uart_transmit(&tx_buffer);
    HAL_Delay(500);
    /*
    while(!ringBuffer_contains_substring(&rx_buffer, ">"))
    {
    	Clear_Buffer(&rx_buffer);
    	Uart_transmit(&tx_buffer);
    	HAL_Delay(200);
    }*/
    //while(!isSubString(Rx_Buffer,">"));

    Clear_Buffer(&rx_buffer);


    Uart_transmit_string((uint8_t*)str,2);
    HAL_Delay(500);
    /*
    while(!ringBuffer_contains_substring(&rx_buffer,"SEND OK"))
    {

        if(ringBuffer_contains_substring(&rx_buffer,"ERROR"))
        {
        	Uart_transmit_string((uint8_t*)str,1);
        	HAL_Delay(50);
            break;
        }
    }*/

    Clear_Buffer(&rx_buffer);

    sprintf(tx_buffer.buffer, "AT+CIPCLOSE=%d\r\n",id);
    Uart_transmit(&tx_buffer);
    //while(!ringBuffer_contains_substring(&rx_buffer,"OK"));

    Clear_Buffer(&rx_buffer);
    return 1;


}


