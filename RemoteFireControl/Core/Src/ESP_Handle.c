  /*
 * ESP_Handle.c
 *  Created on: Mar 25, 2023
 *      Author: ekrm5
 */

#include "ESP_Handle.h"
#include "main.h"
#include "stdio.h"


//Global declarations for UART and Timer handlers
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

//Global declarations for stepper motor structures
extern StepperMotor stepMotor1; //pan step motor
extern StepperMotor stepMotor2; //tilt step motor

//Buffer to hold the received byte and a string of received bytes
extern uint8_t rcvd_byte;
extern uint8_t receive[100];

uint16_t   speedFlag = 1;
uint16_t autoFireFlag = 0;
/**
  * @brief  Initializes the ESP module.
  * @param  None
  * @retval None
  * @Note For detailed explanations of the commands check this; https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/
  */
void ESP_Init(void)
{
	/***********RESETTING**************************************************/

	clearBuf();
	HAL_UART_Transmit(&huart2,(uint8_t*) "AT+RST\r\n", 8, 5);
	HAL_Delay(1500);

	/**************************CHECK it is run or not**************************************************/
	do{
	    clearBuf();
	    HAL_UART_Transmit(&huart2,(uint8_t*) "AT\r\n",4, 5);
	    HAL_UART_Receive(&huart2, receive, 20, 5);
    }while(!containsString((char*)receive,"OK"));

	/**************************Set ESP as client mode**************************************************/
	////




	////
	do{
		clearBuf();
	    HAL_UART_Transmit(&huart2,(uint8_t*) "AT+CWMODE=3\r\n",13, 5);
	    HAL_UART_Receive(&huart2, receive, 30, 5);
     }while(!containsString((char*)receive,"OK"));

	/**************************Enable Multiple Connection**************************************************/
	do{
		clearBuf();
	    HAL_UART_Transmit(&huart2,(uint8_t*) "AT+CIPMUX=1\r\n",13, 5);
	    HAL_UART_Receive(&huart2, receive, 30, 5);
	}while(!containsString((char*)receive,"OK"));


	/**************************Start TCP/IP Communication**************************************************/
	do{
		clearBuf();
		HAL_UART_Transmit(&huart2,(uint8_t*) "AT+CIPSTART=1,\"TCP\",\"172.20.10.2\",80\r\n",42, 100);
		HAL_UART_Receive(&huart2, receive, 100, 100);
		delay_ms(1000);
	    //if(containsString((char*)receive,"busy")){
	    //    HAL_Delay(2000); // Delay for 2 seconds
	    //}
	    //attempts++;
	    /*
	    if(containsString((char*)receive,"1,CLOSED")){
	    	clearBuf();
	        HAL_Delay(1000); // Delay for 2 seconds
			HAL_UART_Transmit(&huart2,(uint8_t*) "AT+CIPSTART=1,\"TCP\",\"192.168.50.228\",80\r\n",42, 100);
			HAL_UART_Receive(&huart2, receive, 100, 100);
	    }
*/
	    if(containsString((char*)receive,"ALREADY CONNECTED"))
		{
	    	break;
		}


	}
	while(!containsString((char*)receive,"OK"));
	clearBuf();

	//Start Uart Interrupt for take an actions from control station
	HAL_UART_Receive_IT(&huart2, &rcvd_byte,1);


}


/**
  * @brief  Starts the ESP server.
  * @param  None
  * @retval None
  */
void ESP_Server_Start(void)
{
	//Wait for response
    while(!containsString((char*)receive,"IPD"));
    conditions();
}

/**
  * @brief  Clears the receive buffer.
  * @param  None
  * @retval None
  */
void clearBuf(void)
{
	for(int i = 0; i < 100; ++i)
	{
		receive[i] = 0;
	}
}


/**
  * @brief  Checks if a string1 contains string2.
  * @param  *string1 Pointer to the first string.
  * @param  *string2 Pointer to the second string.
  * @retval Returns 1 if string1 contains string2, else 0.
  */
int containsString(const char *string1, const char *string2) {
    if (strstr(string1, string2) != NULL) {
        return 1; // string1 contains string2
    } else {
        return 0; // string1 doesn't contain string2
    }
}

/**
  * @brief  Parses the received commands and perform the associated actions.
  * @param  None
  * @retval None
  */
void conditions(void)
{

    if(containsString((char*)receive,"rightx")) //pan right
    {
    	clearBuf();
        setDirection(&stepMotor1, 0);
        setSpeed(&stepMotor1, 90);
        moveContinous(&stepMotor1);
    }
    else if(containsString((char*)receive,"leftx"))//pan left
    {
    	clearBuf();
    	setDirection(&stepMotor1, 1);
    	setSpeed(&stepMotor1, 90);
    	moveContinous(&stepMotor1);
    }
    else if(containsString((char*)receive, "downx"))//tilt below
    {
    	clearBuf();
    	setDirection(&stepMotor2, 0);
    	setSpeed(&stepMotor2, 500);
    	moveContinous(&stepMotor2);
    }
    else if(containsString((char*)receive, "upx"))//tilt up
    {
    	clearBuf();
    	setDirection(&stepMotor2, 1);
    	setSpeed(&stepMotor2, 500);
    	moveContinous(&stepMotor2);
    }
    else if(containsString((char*)receive, "rightup"))//pan tilt right up
    {

    	clearBuf();
    	setDirection(&stepMotor1, 1);
    	setDirection(&stepMotor2, 1);
    	setSpeed(&stepMotor1, 500);
    	setSpeed(&stepMotor2, 500);
    	moveTwoMotorContinous_alter(&stepMotor1, &stepMotor2);



    }
    else if(containsString((char*)receive, "leftup"))//pan tilt left up
    {
    	clearBuf();
    	setDirection(&stepMotor1, 0);
    	setDirection(&stepMotor2, 1);
    	setSpeed(&stepMotor1, 500);
    	setSpeed(&stepMotor2, 500);
    	moveTwoMotorContinous_alter(&stepMotor1, &stepMotor2);
    }
    else if(containsString((char*)receive, "rightdown")) //rightup leftup rightdown leftdown
    {
    	clearBuf();
    	setDirection(&stepMotor1, 1);
    	setDirection(&stepMotor2, 0);
    	setSpeed(&stepMotor1, 500);
    	setSpeed(&stepMotor2, 500);
    	moveTwoMotorContinous_alter(&stepMotor1, &stepMotor2);
    }
    else if(containsString((char*)receive, "leftdown"))
    {
    	clearBuf();
    	setDirection(&stepMotor1, 0);
    	setDirection(&stepMotor2, 0);
    	setSpeed(&stepMotor1, 500);
    	setSpeed(&stepMotor2, 500);
    	moveTwoMotorContinous_alter(&stepMotor1, &stepMotor2);
    }
    else if(containsString((char*)receive, "autofire"))
    {
    	clearBuf();
        //setTurnSpeed(1300);
    	autoFireFlag = 1;
    }
    else if(containsString((char*)receive, "singlefire"))
    {
    	clearBuf();
        //setTurnSpeed(1400);
    	autoFireFlag = 0;
    }
    else if(containsString((char*)receive, "fireon"))
    {
    	if(autoFireFlag)
    	{
        	clearBuf();
        	StartDcMotor();
    		if(speedFlag == 2)
    		{
    			SetDcMotorSpeed(99);
    		}
    		else if(speedFlag == 1)
    		{
    			SetDcMotorSpeed(66);
    		}
    		else
    		{
    			SetDcMotorSpeed(33);
    		}
        	delay_ms(2000); // wait until dc motor gets speed
        	turnForward();
    	}
    	else
    	{
        	clearBuf();
        	StartDcMotor();
    		if(speedFlag == 2)
    		{
    			SetDcMotorSpeed(99);
    		}
    		else if(speedFlag == 1)
    		{
    			SetDcMotorSpeed(66);
    		}
    		else
    		{
    			SetDcMotorSpeed(33);
    		}
        	//clearBuf();
        	//StartDcMotor();
        	delay_ms(2000); // wait until dc motor gets speed
        	turnForward();

    	}

    }
    else if(containsString((char*)receive, "high"))
    {
    	clearBuf();
    	//SetDcMotorSpeed(99);
    	speedFlag = 2;
    }
    else if(containsString((char*)receive, "medium"))
    {
    	clearBuf();
    	speedFlag = 1;

    }
    else if(containsString((char*)receive, "low"))
    {
    	clearBuf();
    	speedFlag = 0;
    }
    else
    {
    	clearBuf();
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);

    }
    clearBuf();
}
