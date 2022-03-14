/*
 * user_sw.c
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 */
#include "gpio.h"
#include "led.h"
#include "main.h"
#include "push_button.h"

/**
 * @brief EXTIライン割り込み処理
 * @param GPIO_Pin EXTIラインに対応する接続されたGPIOピン
 */
void
HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	if (pin == GPIO_PIN_13) {
		led_toggle(USER_LED_1);
		update_pwm_rate();
	}
}
