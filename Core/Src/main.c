/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "adc.h"
#include "config.h"
#include "lcd.h"
#include "matrix_key.h"
#include "pwm.h"
#include "interrupt.h"
#include "flash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define file_number_location       (*(volatile uint32_t *)(0x0800EC00))
FATFS fs;
FIL fil;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
char pass[4] = {'1', '2', '3', '4'};
char new_pass[4] = {'1', '2', '3', '4'};
char checkpass[4];
int value[4] = {100, 5, 50, 8};// s1, t1, s2, t2
int new_value[4] = {100, 5, 50, 8};// s1, t1, s2, t2
float T = 25.0;
int count = -1;
int x = 0;
int file_number = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

osThreadId defaultTaskHandle;
osThreadId Task2Handle;
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
void StartDefaultTask(void const * argument);
void Task2_init(void const * argument);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char Enter_Number(char *cache){
	int j = 8;
	char new = read_keypad();
	while ((new != '=') && (new != 'C')){
		new = read_keypad();
		if (new >= '0' && new <= '9'){
			LCD_PutChar(new);
			if (j<8) {
				for (int i = 1; i <= 8; i++){
					cache[i-1] = cache[i];
				}
			}
			cache[8] = new;
			j--;
		}
	}
	return new;
}

void Display_Data(void){
	LCD_Gotoxy(0,0);
	char buffer[25];
	int S1 = (int)value[0];
	int T1 = (int)value[1];
	int S2 = (int)value[2];
	int T2 = (int)value[3];
	sprintf(buffer, "%d, %d, %d, %d, %.2f", S1, T1, S2, T2, T);
	LCD_SendCommand(0x0C);
	LCD_Puts(buffer);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	uart2_config();
	uart2_gpio();
	adc_config();
	LCD_Init();
	matrix_key_init();
	pwm_init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	GPIOA_ODR |= (1 << 5);//select direction of rotation
	GPIOA_ODR &= ~(1 << 4);//select direction of rotation
	TIM2_CCR1 = 1000 * (value[0] / 100);      //PWM
	TIM2_CR1 |= TIM2_CR1_CEN; // start Timer
	TIM4_CR1 |= TIM4_CR1_CEN; // startTimer
	Display_Data();
  //save file_number to flash, whenever reset MCU creating a new file to save data
  if (file_number_location == 0xFF){
	  char buffer[5];
	  sprintf(buffer, "%d", file_number);
	  Flash_WriteString(0x0800EC00, buffer);
  }
  char buffer[5];
  Flash_ReadString(0x0800EC00, buffer, sizeof(buffer));
  file_number = file_number + atoi(buffer) + 1;
  sprintf(buffer, "%d", file_number);
  Flash_WriteString(0x0800EC00, buffer);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Task2 */
  osThreadDef(Task2, Task2_init, osPriorityIdle, 0, 128);
  Task2Handle = osThreadCreate(osThread(Task2), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	//config mode
  for(;;)
  {
	   // enter C and = to go to password
		char data = read_keypad();
		if (data == 'C'){
			TIM1_CNT = 0;
			TIM1_CR1 |=  TIM1_CR1_CEN;
			TIM1_SR &= ~(1 << 0);
			char data = read_keypad();
			while ((!(TIM1_SR & (1 << 0))) && (data == 0x01)){
				data = read_keypad();
			}
			TIM1_CR1 &= ~TIM1_CR1_CEN;
			if (data == '='){
				//enter password
				LCD_SendCommand(0x0E);
				LCD_Clear();
				LCD_Gotoxy(0,0);
				LCD_Puts("Enter Password:");
				bool password = false;
				while (password == false){
					LCD_Gotoxy(0,1);
					int i = 0;
					while (i < 4){
						char entered_pass = read_keypad();
						if (entered_pass != 0x01){
							checkpass[i] = entered_pass;
							LCD_PutChar(entered_pass);
							LCD_Gotoxy(i+1,1);
							i++;
						}
					}
					if ((pass[0]==checkpass[0]) && (pass[1]==checkpass[1])
							&& (pass[2]==checkpass[2]) && (pass[3]==checkpass[3])){
						password = true;
					}
					else {
						LCD_Clear();
						LCD_Puts("incorrect, again:");
					}
				}
				//go to menu
				LCD_Menu(0);
				char new_data;
				int a = 0;
				while (new_data != '='){
					new_data = read_keypad();
					if ((a>=0) && (a<=1) && (new_data == 'X')){
						a++;
						LCD_Menu(a);
					}
					if ((a>=1) && (a<=2) && (new_data == '%')){
						a--;
						LCD_Menu(a);
					}
					if (new_data == '+'){
						if (a == 0){
							//change values of S1 and T1
							char cache[10] = {'0','0','0','0','0','0','0','0','0','\0'};
							char cache1[10] = {'0','0','0','0','0','0','0','0','0','\0'};
							LCD_Clear();
							LCD_Gotoxy(0,0);
							LCD_Puts("+ Enter new value:");
							LCD_Gotoxy(0,1);
							LCD_Puts("S1:");
							LCD_Gotoxy(0,2);
							LCD_Puts("T1:");
							LCD_Gotoxy(3,1);
							char new = read_keypad();

							while (1){
								new = Enter_Number(cache);
								if (new == '='){
									LCD_Gotoxy(3,2);
									new = Enter_Number(cache1);
									if (new == '=') {
										new_value[0] = atoi(cache);
										new_value[1] = atoi(cache1);
										LCD_Menu(a);
										break;
									}
								}

								if (new == 'C') {
									LCD_Menu(a);
									break;
								}
							}
						}
						else if (a == 1){
							//change values of S2, T2
							char cache[10] = {'0','0','0','0','0','0','0','0','0','\0'};
							char cache1[10] = {'0','0','0','0','0','0','0','0','0','\0'};
							LCD_Clear();
							LCD_Gotoxy(0,0);
							LCD_Puts("+ Enter new value:");
							LCD_Gotoxy(0,1);
							LCD_Puts("S2:");
							LCD_Gotoxy(0,2);
							LCD_Puts("T2:");
							LCD_Gotoxy(3,1);
							char new = read_keypad();

							while (1){
								new = Enter_Number(cache);
								if (new == '='){
									LCD_Gotoxy(3,2);
									new = Enter_Number(cache1);
									if (new == '=') {
										new_value[2] = atoi(cache);
										new_value[3] = atoi(cache1);
										LCD_Menu(a);
										break;
									}
								}
								if (new == 'C') {
									LCD_Menu(a);
									break;
								}
							}
						}
						else{
							//change password
							bool format = false;
							LCD_Clear();
							LCD_Gotoxy(0,0);
							LCD_Puts("Enter new password:");
							LCD_Gotoxy(0,1);
							while (format == false){
								char cache[10] = {'0','0','0','0','0','0','0','0','0','\0'};
								char new = read_keypad();
								new = Enter_Number(cache);
								if (new == '='){
									if (cache[4] == '0'){
										new_pass[0] = cache[5];
										new_pass[1] = cache[6];
										new_pass[2] = cache[7];
										new_pass[3] = cache[8];
										LCD_Menu(a);
										format = true;
									}
									else{
										LCD_Clear();
										LCD_Gotoxy(0,0);
										LCD_Puts("Enter password again");
										LCD_Gotoxy(0,1);
										LCD_Puts("(only 4 characters!)");
										LCD_Gotoxy(0,2);
									}
								}
								if (new == 'C') {
									LCD_Menu(a);
									break;
								}

							}
						}
					}
				}
				if (new_data == '=') {
					//save to flash memory
					LCD_Clear();
					LCD_Gotoxy(0,0);
					LCD_Puts("Save the old values");
					LCD_Gotoxy(0,1);
					LCD_Puts("to flash memory:");
					LCD_Gotoxy(0,2);
					LCD_Puts("1: Save");
					LCD_Gotoxy(0,3);
					LCD_Puts("2: Do not save");
					while ((new_data != '1') && (new_data != '2')){
						new_data = read_keypad();
					}
					if (new_data == '1') {
						char buffer[25];
						int S1 = (int)value[0];
						int T1 = (int)value[1];
						int S2 = (int)value[2];
						int T2 = (int)value[3];
						sprintf(buffer, "%d, %d, %d, %d, %.2f", S1, T1, S2, T2, T);
						Flash_WriteString(0x0800FC00 + 25 * x, buffer);
						x ++;
					}
					value[0] = new_value[0];
					value[1] = new_value[1];
					value[2] = new_value[2];
					value[3] = new_value[3];
					pass[0] = new_pass[0];
					pass[1] = new_pass[1];
					pass[2] = new_pass[2];
					pass[3] = new_pass[3];
				}
				LCD_Clear();
			}
		}
		Display_Data();

    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Task2_init */
/**
* @brief Function implementing the Task2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task2_init */
void Task2_init(void const * argument)
{
  /* USER CODE BEGIN Task2_init */
  /* Infinite loop */
  for(;;)
  {
		T = calculate_temperature();
		char buffer1[20];
		int S1 = (int)value[0];
		int T1 = (int)value[1];
		int S2 = (int)value[2];
		int T2 = (int)value[3];
		sprintf(buffer1, "%d, %d, %d, %d, ", S1, T1, S2, T2);
		char buffer[10];
		int intPart = (int)T;
		int decimalPart = (int)((T - intPart) * 100);
		sprintf(buffer, "%d.%02d, \r\n", intPart, decimalPart);
		uart_sendstring(buffer1);
		uart_sendstring(buffer);
		//save to sd card
		char filename[20];
		sprintf(filename, "data_%d.txt", file_number);
		f_mount(&fs, "", 0);
		f_open(&fil, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		f_lseek(&fil, fil.fsize);
		f_puts(buffer1, &fil);
		f_puts(buffer, &fil);
		f_puts("\n", &fil);
		f_close(&fil);
		count ++;
		if (count == value[1]){
			GPIOA_ODR |= (1 << 4);
			GPIOA_ODR &= ~(1 << 5);
			TIM2_CCR1 = 1000 * (value[2] / 100);
		}
		if (count == (value[1] + value[3])){
			GPIOA_ODR |= (1 << 5);
			GPIOA_ODR &= ~(1 << 4);
			TIM2_CCR1 = 1000 * (value[0] / 100);
			count = 0;
		}
    osDelay(1000);
  }
  /* USER CODE END Task2_init */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
