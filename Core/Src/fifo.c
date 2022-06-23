/*
 * fifo.c
 *
 *  Created on: Jun 22, 2022
 *      Author: toshiyuki_umetsu
 */
// clang-format off
#include "fifo.h"
// clang-format on

/**
 * @brief Initialize FIFO.
 *
 * @param f FIFO.
 * @param buf Buffer for FIFO
 * @param bufsize Buffer size in bytes.
 */
void fifo_init(struct fifo *f, uint8_t *buf, uint32_t bufsize) {
    f->buf = buf;
    f->bufsize = bufsize;
    f->in = 0u;
    f->out = 0u;
    for (uint32_t i = 0u; i < f->bufsize; i++) {
        f->buf[i] = 0x0u;
    }

    return;
}

/**
 * @brief Deinitialize FIFO.
 *
 * @param f FIFO
 */
void fifo_deinit(struct fifo *f) {
    f->out = 0u;
    f->in = 0u;

    return;
}

/**
 * @brief Clear FIFO.
 *
 * @param f FIFO
 */
void fifo_clear(struct fifo *f) {
    f->out = 0u;
    f->in = 0u;

    return;
}

/**
 * @brief Add data to FIFO.
 *
 * @param f FIFO
 * @param d Data
 */
void fifo_put(struct fifo *f, uint8_t d) {
    uint32_t next;

    next = f->in + 1u;
    if (next >= f->bufsize) {
        next = 0;
    }
    if (next == f->out) {
        // No enough space.
        return;
    } else {
        f->buf[f->in] = d;
        f->in++;
        if (f->in >= f->bufsize) {
            f->in = 0;
        }
    }
}

/**
 * @brief Getting one byte from FIFO.
 *
 * @param f FIFO
 * @retval Data. If FIFO is empty, return 0.
 */
uint8_t fifo_get(struct fifo *f) {
    if (f->in == f->out) {
        return 0; // No data.
    }

    uint8_t ret = f->buf[f->out];
    f->out++;
    if (f->out >= f->bufsize) {
        f->out = 0;
    }
    return ret;
}

/**
 * @brief Testing whether FIFO has data.
 *
 * @param f FIFO
 * @return When FIFO has data, return true. Otherwise, return false.
 */
bool fifo_has_data(const struct fifo *f) { return (f->in != f->out); }

/**
 * @brief Testing whether FIFO has blank.
 *
 * @param f FIFO
 * @retval When FIFO has blank, return true. Otherwise, return false.
 */
bool fifo_has_blank(const struct fifo *f) {
    uint32_t in;

    in = f->in + 1u;
    if (in >= f->bufsize) {
        in = 0;
    }
    return (in != f->out);
}

/**
 * @brief Getting data count in bytes.
 *
 * @param f FIFO
 * @retval Data count in bytes.
 */
uint32_t fifo_get_data_count(const struct fifo *f) {
    uint32_t data_count;

    if (f->in == f->out) {
        data_count = 0u; // No data.
    } else {
        if (f->out <= f->in) {
            // |----(out)///////(in)----|
            data_count = (f->in - f->out);
        } else {
            // |////(in)--------(out)///|
            data_count = f->in + (f->bufsize - f->out);
        }
    }
    return data_count;
}
