/*
 * led.h
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdbool.h>
#include <stdint.h>

enum led_no { USER_LED_1 };

#ifdef __cplusplus
extern "C" {
#endif

void led_set(enum led_no no, bool is_turn_on);
void led_toggle(enum led_no no);

#ifdef __cplusplus
}
#endif

#endif /* INC_LED_H_ */
