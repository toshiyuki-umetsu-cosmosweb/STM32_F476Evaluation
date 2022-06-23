/*
 * led.c
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 */
#include "gpio.h"
#include "led.h"

void led_set(enum led_no no, bool is_turn_on) {
    GPIO_PinState state;

    switch (no) {
    case USER_LED_1:
        state = (is_turn_on) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, state);
        break;
    default:
        // do nothing.
        break;
    }

    return;
}
/**
 * LEDの点灯状態を反転させる。
 */
void led_toggle(enum led_no no) {
    switch (no) {
    case USER_LED_1:
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        break;
    default:
        // do nothing.
        break;
    }

    return;
}
