/*
 * timer1.c
 *
 *  Created on: Apr 4, 2022
 *      Author: toshiyuki_umetsu
 */
#include "debug.h"
#include "tim.h"
#include "timer1.h"

static void (*TimerHandler)(void) = NULL;

static void print_tim_intr_cause(const char *name, TIM_HandleTypeDef *htim);

/**
 * Timer1のハンドラを設定する。
 */
void timer1_set_handler(void (*timer_handler)(void)) { TimerHandler = timer_handler; }
/**
 * Timer1を開始する。
 */
void timer1_start(void) {
    // TIM1 動作開始
    HAL_TIM_Base_Start_IT(&htim1);
    // TIM1 PWM出力開始
    HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
    // TIM1 アウトプットコンペア開始
    HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_2);

    return;
}

/**
 * タイマー1の更新処理をする。
 *
 * @note
 * プリスケーラ 100usec, ARR=5000-1 => 500msec.
 */
void timer1_on_update(void) {
    if (TimerHandler != NULL) {
        TimerHandler();
    }
    print_tim_intr_cause("TIM1_PeriodElapsed", &htim1);
}

/**
 * タイマー1のアウトプットコンペアを処理する。
 */
void timer1_on_output_compare(void) {
    print_tim_intr_cause("TIM1_OC_DelayElapsed", &htim1);

    return;
}

/**
 * タイマー1のパルス完了割り込みを処理する。
 */
void timer1_on_pwm_pulse_finished(void) {
    print_tim_intr_cause("TIM1_PWM_PulseFinished", &htim1);

    return;
}

/**
 * タイマー割り込みの要因をデバッグ出力する。
 *
 * @param name Peripheral name. Ex) "TIM1"
 * @param htim Peripheral handle.
 */
static void print_tim_intr_cause(const char *name, TIM_HandleTypeDef *htim) {
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

    return;
}
