/*
 * timer1.h
 *
 *  Created on: Apr 4, 2022
 *      Author: toshiyuki_umetsu
 */

#ifndef TIMER1_H_
#define TIMER1_H_

void timer1_set_handler(void (*timer_func)(void));
void timer1_start(void);
void timer1_on_update(void);
void timer1_on_output_compare(void);
void timer1_on_pwm_pulse_finished(void);

#endif /* TIMER1_H_ */
