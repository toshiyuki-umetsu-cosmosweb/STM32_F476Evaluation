/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "usart.h"
#include "led.h"
#include "htu21d.h"
#include "debug.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/**
 * Variable to store temperature.
 */
static float Temperature;
/**
 * Variable to store Humifier
 */
static float Humidity;
/**
 * Test message for test.
 */
static const uint8_t TestMessage[32u] = "This is test message.\n";
/**
 * Request flag to execute any process.
 */
static bool IsProcessRequested;

/**
 * Process Number.
 */
static int32_t ProcessNo;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void execute_process(void);
static void hal_uart_transmit_proc(void);
static bool hal_uart_transmit_it_proc(void);
static bool htu21d_measure_temperature_proc(void);
static bool htu21d_measure_humidity_proc(void);
static void send_measure_data_proc(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
static void print_tim_intr_cause(const char *name, TIM_HandleTypeDef *htim);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	Temperature = 0.0f;
	Humidity = 0.0f;
	IsProcessRequested = false;
	ProcessNo = 0L;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  htu21d_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    HAL_TIM_Base_Start_IT(&htim1); // Start TIM3 Interrupt.


	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if (IsProcessRequested) {
			execute_process();
			IsProcessRequested = false;
		}
		// Try wait.
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * Execute process.
 */
static void
execute_process(void)
{
    const char *op_msg = NULL;

	uint32_t begin = HAL_GetTick();
	switch (ProcessNo) {
		case 0L:
			hal_uart_transmit_proc();
			op_msg = "HAL_UART_Transmit";
			ProcessNo++;
			break;
		case 1L:
			if (hal_uart_transmit_it_proc()) {
				op_msg = "HAL_UART_Transmit_IT";
				ProcessNo++;
			} else {
				op_msg = NULL;
			}
			break;
		case 2L:
			if (htu21d_measure_temperature_proc()) {
				op_msg = "htu21d_measure_temperature";
			}
			ProcessNo++;
			break;
		case 3L:
			if (htu21d_measure_humidity_proc()) {
				op_msg = "htu21d_measure_humidity";
			}
			ProcessNo++;
			break;
		default:
			send_measure_data_proc();
			ProcessNo = 0u;
			op_msg = NULL;
			break;
	}

	uint32_t elapse = HAL_GetTick() - begin;
	if (op_msg != NULL) {
		dprintf("%s %lu msec\n", op_msg, elapse);
	}
}


/**
 * Send serial data with using HAL_UART_Transmit() function.
 */
static void
hal_uart_transmit_proc(void)
{
    if (HAL_UART_Transmit(&huart2, TestMessage, sizeof(TestMessage), 32u) != HAL_OK) {
    	// Ignore error.
    }
}
/**
 * Send serial data with using HAL_UART_Transmit_IT() function.
 */
static bool
hal_uart_transmit_it_proc(void)
{
	bool is_succeed = false;

	if (huart2.gState == HAL_UART_STATE_READY) {
	    if (HAL_UART_Transmit_IT(&huart2, TestMessage, sizeof(TestMessage)) == HAL_OK) {
	    	is_succeed = true;
	    }
	}

	return is_succeed;
}

/**
 * Measurement Temperature proc.
 */
static bool
htu21d_measure_temperature_proc(void)
{
	return htu21d_measure_temperature(&Temperature);
}

/**
 * Measurement Humiditiy proc.
 */
static bool
htu21d_measure_humidity_proc(void)
{
	return htu21d_measure_humidity(&Humidity);
}

/**
 * Message buffer which using to transmit measurement data.
 */
static char MsgBuf[256u];

/**
 * Send measurement data via UART.
 */
static void
send_measure_data_proc(void)
{
	int16_t temp_n = (int16_t)(Temperature);
	int16_t temp_d = (int16_t)((Temperature - temp_n) * 100.0f);
	int16_t humi_n = (int16_t)(Humidity);
	int16_t humi_d = (int16_t)((Humidity - humi_n) * 100.0f);
	snprintf(MsgBuf, sizeof(MsgBuf),
			"Temp=%d.%d Humi=%d.%d\n", temp_n, temp_d, humi_n, humi_d);

	size_t len = strlen(MsgBuf);
	if (huart2.gState == HAL_UART_STATE_READY) {
	    if (HAL_UART_Transmit_IT(&huart2, (uint8_t*)(MsgBuf), len) == HAL_OK) {
	    	// Ignore error.
	    }
	}
}

/**
 * Timer callback handler.
 *
 * @brief  Period elapsed callback in non-blocking mode
 * @param  htim TIM handle
 * @retval None
 */
void
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim1) {
		// TIM1 Interrupt.
		print_tim_intr_cause("TIM1", htim);

		IsProcessRequested = true;
	}
}
/**
 * Print TIMx interrupt cause.
 *
 * @param name Peripheral name. Ex) "TIM1"
 * @param htim Peripheral handle.
 */
static void
print_tim_intr_cause(const char *name, TIM_HandleTypeDef *htim)
{
	uint32_t tick = HAL_GetTick();

	char flags[8];

	flags[0] = TIM_GET_ITSTATUS(htim, TIM_IT_BREAK) ? 'B' : ' ';
	flags[1] = TIM_GET_ITSTATUS(htim, TIM_IT_UPDATE) ? 'U' : ' ';
	flags[2] = TIM_GET_ITSTATUS(htim, TIM_IT_TRIGGER) ? 'T' : ' ';
	flags[3] = TIM_GET_ITSTATUS(htim, TIM_IT_CC1) ? '1' : ' ';
	flags[4] = TIM_GET_ITSTATUS(htim, TIM_IT_CC1) ? '2' : ' ';
	flags[5] = TIM_GET_ITSTATUS(htim, TIM_IT_CC1) ? '3' : ' ';
	flags[6] = TIM_GET_ITSTATUS(htim, TIM_IT_CC1) ? '4' : ' ';
	flags[7] = '\0'; // NULL terminate

	dprintf("%lu:%s intr %s\n", tick, name, flags);

	return ;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
