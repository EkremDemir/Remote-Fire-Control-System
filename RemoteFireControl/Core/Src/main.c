/*
 * main.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ekrm5
 */



//Header file inclusion
#include "main.h"
#include <string.h>


#define DEBOUNCE_DELAY 50 // debounce time in milliseconds

int i = 0;
//Global declarations for UART and Timer handlers
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

//Global declarations for stepper motor structures
StepperMotor stepMotor1;
StepperMotor stepMotor2;

//Buffer to hold the received byte and a string of received bytes
uint8_t rcvd_byte;
uint8_t receive[100] = {0};
uint8_t device_ID = 0;
//Hold time for debouncing
uint32_t buttonTimestamp = 0;
// Function prototypes
void SystemClock_Config(void);
void UART2_Init(void);
void UART1_Init(void);
void GPIO_Init(void);
void TIMER6_Init(void);
void TIMER2_Init(void);
void TIMER3_Init(void);
void Step_Motor_Init(void);
void Dc_Motor_Init();

//Externs
extern uint16_t autoFireFlag;

//Main function
int main(void)
{

    //Initialization functions
  	HAL_Init();
	SystemClock_Config();
	UART2_Init();
	GPIO_Init();
    TIMER6_Init();
    TIMER2_Init();
    TIMER3_Init();

    //Disabling interrupt for limit switch
    //HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

    //Preventing uncontrolled interrupts by clearing Status Registers for Timer 2, Timer 3 and Timer 6
    TIM2->SR =  0;
    TIM3->SR =  0;
    TIM6->SR = 0;

    //Initialization functions
	ESP_Init();
    Step_Motor_Init();
    Dc_Motor_Init();
    //StartDcMotor();
    //turnForward();
	while(1)
	{
        //Starting ESP server in the infinite while loop
		ESP_Server_Start();
		//StartDcMotor();
	}



}


///////HSI 16 MHz Clock frequency
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits   = UART_STOPBITS_1;
	huart2.Init.Parity     = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	huart2.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
}


//Get one micro second
void TIMER6_Init(void)
{
	//uint32_t pre =  HAL_RCC_GetHCLKFreq() /1000000;
	htim6.Instance = TIM6;
    htim6.Init.Prescaler = 0;
    htim6.Init.Period    = 16-1;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;

    if(HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
    	Error_Handler();
    }

}


//200 Hz pwm output for DC motor
void TIMER2_Init(void)
{
	TIM_OC_InitTypeDef timPWM_init;

	htim2.Instance = TIM2;
	htim2.Init.Period = 50000 - 1;
	htim2.Init.Prescaler = 16 - 1;

	if(HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	memset(&timPWM_init,0,sizeof(timPWM_init));

	timPWM_init.OCMode = TIM_OCMODE_PWM1;
	timPWM_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	timPWM_init.Pulse   = 0;
	if(HAL_TIM_PWM_ConfigChannel(&htim2, &timPWM_init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_TIM_PWM_ConfigChannel(&htim2, &timPWM_init, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

}



//50 Hz pwm output for continuous servo motor (SG90)
void TIMER3_Init(void)
{
	TIM_OC_InitTypeDef timPWM_init;

	htim3.Instance = TIM3;
	htim3.Init.Period = 20000 - 1;
	htim3.Init.Prescaler = 16 - 1;

	if(HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	memset(&timPWM_init,0,sizeof(timPWM_init));
	timPWM_init.OCMode = TIM_OCMODE_PWM1;
	timPWM_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	timPWM_init.Pulse   = 0;
	if(HAL_TIM_PWM_ConfigChannel(&htim3, &timPWM_init, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	timPWM_init.Pulse   = 0;
	if(HAL_TIM_PWM_ConfigChannel(&htim3, &timPWM_init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef stepgpio;

	stepgpio.Mode = GPIO_MODE_OUTPUT_PP;
	stepgpio.Pin  = GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_8; //pin7 is for direction, pin9 is for step
	stepgpio.Speed = GPIO_SPEED_LOW;
	stepgpio.Pull  = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOE,&stepgpio);



	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef dcgpio;
    //For direction of the dc motors
	dcgpio.Mode = GPIO_MODE_OUTPUT_PP;
	dcgpio.Pin  = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	dcgpio.Speed = GPIO_SPEED_LOW;
	dcgpio.Pull  = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOD,&dcgpio);


    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins : PE14 PE15 */
    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : PB11 PB13 */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)   // This function is called when the interrupt occurs
{

    uint32_t currentTime = HAL_GetTick();

    if((currentTime - buttonTimestamp) > DEBOUNCE_DELAY)
    {
        if(GPIO_Pin == GPIO_PIN_14 || GPIO_Pin == GPIO_PIN_15)
        {
        	stepMotor2.moveState = 0;
        	if(GPIO_Pin == GPIO_PIN_15)
        	{
        		setDirection(&stepMotor2, 0);
        		setSpeed(&stepMotor2, 500);
        		moveToX(&stepMotor2, 300);
        	}
        	if(GPIO_Pin == GPIO_PIN_14)
        	{
        		setDirection(&stepMotor2, 1);
        		setSpeed(&stepMotor2, 500);
        		moveToX(&stepMotor2, 300);


        	}
        }

        if(GPIO_Pin == GPIO_PIN_11 || GPIO_Pin == GPIO_PIN_13)
        {
        	if(GPIO_Pin == GPIO_PIN_13)
        	{
        		//stopServo();

        		turnForward();
        		delay_ms(100);
        		stopServo();
        	}
        	if(GPIO_Pin == GPIO_PIN_11)
        	{
        		StopDcMotor();
        		turnBack();
        	}
        }

        buttonTimestamp = currentTime;
    }

}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(rcvd_byte == '+')
	{
		HAL_UART_Receive(&huart2,receive, 50, 20);
    	if(containsString((char*)receive,"rightxend"))
    	{
    		stepMotor1.moveState = 0;
    		clearBuf();
    	}
        else if(containsString((char*)receive,"leftxend"))//pan left
        {
        	stepMotor1.moveState = 0;
        	clearBuf();
        }
        else if(containsString((char*)receive, "downxend"))//tilt below
        {
        	stepMotor2.moveState = 0;
        	clearBuf();
        }
        else if(containsString((char*)receive, "upxend"))//tilt up
        {
        	stepMotor2.moveState = 0;
        	clearBuf();
        }
        else if(containsString((char*)receive, "rightupend"))//pan tilt right up
        {

        	stepMotor1.moveState = 0;
        	stepMotor2.moveState = 0;
        	clearBuf();


        }
        else if(containsString((char*)receive, "leftupend"))//pan tilt left up
        {
        	stepMotor1.moveState = 0;
        	stepMotor2.moveState = 0;
        	clearBuf();
        }
        else if(containsString((char*)receive, "rightdownend"))
        {
        	stepMotor1.moveState = 0;
        	stepMotor2.moveState = 0;
        	clearBuf();
        }
        else if(containsString((char*)receive, "leftdownend"))
        {
        	stepMotor1.moveState = 0;
        	stepMotor2.moveState = 0;
        	clearBuf();
        }
        else if(containsString((char*)receive, "fireoff"))
        {
        	if(autoFireFlag)
        	{

        	}
        	else
        	{
            	stopServo();
            	StopDcMotor();
            	clearBuf();
        	}
        }

	}
	else
	{

	}
	HAL_UART_Receive_IT(&huart2, &rcvd_byte,1);
}




void delay_us(uint32_t time)
{
	HAL_TIM_Base_Start(&htim6);
	while(time)
	{
		//check update event flag is set
	    while( !(TIM6->SR & TIM_SR_UIF));
		TIM6->SR = 0; //clear TIM_SR_UIF
		--time;
	}
	//HAL_TIM_Base_Stop(&htim6);
}

void delay_ms(uint32_t time)
{
	time = time * 1000;
	HAL_TIM_Base_Start(&htim6);

	while(time)
	{
		//check update event flag is set
		while( !(TIM6->SR & TIM_SR_UIF));
		TIM6->SR = 0; //clear TIM_SR_UIF
		--time;
	}

	HAL_TIM_Base_Stop(&htim6);
}

void Step_Motor_Init(void)
{
	stepMotor1.GPIOx = GPIOE;
	stepMotor1.pin_dir = GPIO_PIN_7;
	stepMotor1.pin_step = GPIO_PIN_9;

	InitializeStepper(&stepMotor1);


	stepMotor2.GPIOx = GPIOE;
	stepMotor2.pin_dir = GPIO_PIN_8;
	stepMotor2.pin_step = GPIO_PIN_10;

	InitializeStepper(&stepMotor2);
}


void Dc_Motor_Init()
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
}

void Error_Handler(void)
{
	while(1);
}


