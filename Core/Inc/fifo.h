/*
 * fifo.h
 *
 *  Created on: Jun 22, 2022
 *      Author: toshiyuki_umetsu
 */

#ifndef INC_FIFO_H_
#define INC_FIFO_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * FIFO
 */
struct fifo {
    uint8_t *buf;     // Buffer
    uint32_t bufsize; // Buffer size.
    uint32_t out;     // Read out
    uint32_t in;      // Write in
};

void fifo_init(struct fifo *f, uint8_t *buf, uint32_t bufsize);
void fifo_deinit(struct fifo *f);
void fifo_clear(struct fifo *f);
void fifo_put(struct fifo *f, uint8_t d);
uint8_t fifo_get(struct fifo *f);
bool fifo_has_data(const struct fifo *f);
bool fifo_has_blank(const struct fifo *f);
uint32_t fifo_get_data_count(const struct fifo *f);

#endif /* INC_FIFO_H_ */
