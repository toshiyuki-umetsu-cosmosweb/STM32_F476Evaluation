/*
 * timer1_proc.c
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 */
#include "led.h"
#include "usart.h"
#include "tim.h"
#include "debug.h"

#ifdef _DEBUG
static uint32_t Counter = 0u;

static void print_intr_cause(const char *name, TIM_HandleTypeDef *htim);
#endif

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim1) {
		// タイマー1割り込み
		if (TIM_GET_ITSTATUS(htim, TIM_IT_BREAK)) {
		    led_toggle(USER_LED_1);
		} else if (TIM_GET_ITSTATUS(htim, TIM_IT_UPDATE)) {
		    led_toggle(USER_LED_1);
		} else if (TIM_GET_ITSTATUS(htim, TIM_IT_TRIGGER)) {
		    led_toggle(USER_LED_1);
		}

#ifdef _DEBUG
		Counter++;
		if (Counter >= 500u) {
			print_intr_cause("HTIM1", htim);
			Counter = 0u;
		}
#endif
	}
}

#ifdef _DEBUG
static void
print_intr_cause(const char *name, TIM_HandleTypeDef *htim)
{
	if (TIM_GET_ITSTATUS(htim, TIM_IT_BREAK)) {
		dprintf("%s break intr\n", name);
	}
	if (TIM_GET_ITSTATUS(htim, TIM_IT_UPDATE)) {
		dprintf("%s update intr\n", name);
	}
	if (TIM_GET_ITSTATUS(htim, TIM_IT_TRIGGER)) {
		dprintf("%s trigger intr\n", name);
	}
	if (TIM_GET_ITSTATUS(htim, TIM_IT_CC1)) {
		dprintf("%s cc1 intr\n", name);
	}
	if (TIM_GET_ITSTATUS(htim, TIM_IT_CC2)) {
		dprintf("%s cc2 intr\n", name);
	}
	if (TIM_GET_ITSTATUS(htim, TIM_IT_CC3)) {
		dprintf("%s cc3 intr\n", name);
	}
	if (TIM_GET_ITSTATUS(htim, TIM_IT_CC4)) {
		dprintf("%s cc4 intr\n", name);
	}
}
#endif
