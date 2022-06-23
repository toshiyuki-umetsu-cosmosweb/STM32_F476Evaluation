/*
 * timer2_pwm.c
 *
 *  Created on: Apr 4, 2022
 *      Author: toshiyuki_umetsu
 */
#include "tim.h"
#include "timer2_pwm.h"

/**
 * PWM Rate
 */
static uint32_t PwmRate;

/**
 * Timer2 PWM出力開始
 */
void timer2_pwm_start(void) {
    PwmRate = 50;

    HAL_TIM_Base_Start_IT(&htim2); // Start TIM2.
    HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);

    return;
}

/**
 * Update PWM rate.
 */
void timer2_pwm_update_rate(void) {
    PwmRate += 5u;
    if (PwmRate > 100u) {
        PwmRate = 0u;
    }

    uint32_t period = htim2.Instance->ARR + 1u;
    if (PwmRate >= 100u) {
        uint32_t tim3_or1 = htim2.Instance->OR1 & ~0x3u;
        htim2.Instance->OR1 = tim3_or1 | 0x01u; // Connect to COMP1_OUT
        htim2.Instance->CCR3 = period;          //
    } else if (PwmRate == 0u) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
        uint32_t tim3_or1 = htim2.Instance->OR1 & ~0x3u;
        htim2.Instance->OR1 = tim3_or1 | 0x00u; // Connect to I/O
        htim2.Instance->CCR3 = 0u;
    } else {
        uint32_t tim3_or1 = htim2.Instance->OR1 & ~0x3u;
        htim2.Instance->OR1 = tim3_or1 | 0x01u; // Connect to COMP1_OUT
        htim2.Instance->CCR3 = ((period * PwmRate) / 100u) - 1u;
    }

    return;
}
