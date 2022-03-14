/*
 * timer1_proc.c
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 */
#include "led.h"
#include "usart.h"
#include "tim.h"

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

	}
}
