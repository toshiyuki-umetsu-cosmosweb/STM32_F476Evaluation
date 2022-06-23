/*
 * mhtu21d.h
 *
 *  Created on: Mar 11, 2022
 *      Author: toshiyuki_umetsu
 */

#ifndef SRC_MHTU21D_H_
#define SRC_MHTU21D_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void htu21d_init(void);
void htu21d_destroy(void);
bool htu21d_measure_temperature(float *temperature);
bool htu21d_measure_humidity(float *humidifier);

#ifdef __cplusplus
}
#endif

#endif /* SRC_MHTU21D_H_ */
