/*
 * main.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ekrm5
 */

/*
 * main.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */

/*Used to calculate time execution of a code block*/
/*
void delay_us(uint32_t us)
{
   DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


  uint32_t start_tick = DWT->CYCCNT;
  uint32_t us_ticks = HAL_RCC_GetHCLKFreq() / 1000000 * us;
  while ((DWT->CYCCNT - start_tick) < us_ticks);


    uint32_t start_tick = DWT->CYCCNT;


    uint32_t end_tick = DWT->CYCCNT;
	uint32_t elapsed_ticks = end_tick - start_tick;
	uint32_t elapsed_us = elapsed_ticks / (HAL_RCC_GetHCLKFreq() / 1000000);
}
*/




#include "main.h"
#include <string.h>


extern ringBuffer rx_buffer;
extern ringBuffer tx_buffer;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim2;


void SystemClock_Config(void);
void UART2_Init(void);
void UART1_Init(void);
void GPIO_Init(void);
void TIMER6_Init(void);
void TIMER2_Init(void);
void Step_Motor_Init(void);




StepperMotor stepMotor1;
StepperMotor stepMotor2;

char *result;

uint8_t rcvd_byte;

bool receive_complete_flag = 0;
bool receive_buffer_full_flag = 0;


int main(void)
{
	HAL_Init();
	SystemClock_Config();

	UART2_Init();
	UART1_Init();
	GPIO_Init();
    TIMER6_Init();
    TIMER2_Init();

    //prevent uncontrolled interrupts
    TIM2->SR =  0; //
    TIM6->SR = 0;


    Step_Motor_Init();
	Buffer_Init();
	ESP_Init();



	Uart_transmit_string((uint8_t*)"The application is running\r\n", 1);
	HAL_UART_Receive_IT(&huart2, &rcvd_byte,1);

	while(1)
	{

	}

}


///////HSI 16MH
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

void UART1_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits   = UART_STOPBITS_1;
	huart1.Init.Parity     = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	huart1.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart1) != HAL_OK)
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

	timPWM_init.OCMode = TIM_OCMODE_PWM1;
	timPWM_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	timPWM_init.Pulse   = 0;
	if(HAL_TIM_PWM_ConfigChannel(&htim2, &timPWM_init, TIM_CHANNEL_1) != HAL_OK)
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



	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef pwmgpio;

	pwmgpio.Mode = GPIO_MODE_OUTPUT_PP;
	pwmgpio.Pin  = GPIO_PIN_1;
	pwmgpio.Speed = GPIO_SPEED_LOW;
	pwmgpio.Pull  = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA,&pwmgpio);
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(rx_buffer.tail != ((rx_buffer.head + 1) %  BUFFERSIZE))
    {

		if( (rcvd_byte == '\n'))
		{
			rx_buffer.buffer[rx_buffer.head] = rcvd_byte;
			rx_buffer.head = (rx_buffer.head + 1) %  BUFFERSIZE;

			if((rx_buffer.buffer[rx_buffer.head -4] == 'O') && (rx_buffer.buffer[rx_buffer.head -3] == 'K'))
			{

				receive_complete_flag = 1;
			}
		}
		else
		{
			rx_buffer.buffer[rx_buffer.head] = rcvd_byte;
			rx_buffer.head = (rx_buffer.head + 1) %  BUFFERSIZE;
		}


		HAL_UART_Receive_IT(&huart2, &rcvd_byte,1);
    }
    else
    {
    	receive_buffer_full_flag = 1;
    	rcvd_byte = rcvd_byte;
    	HAL_UART_Receive_IT(&huart2, &rcvd_byte,1);
    }
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
	HAL_TIM_Base_Stop(&htim6);
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


void Error_Handler(void)
{
	while(1);
}

