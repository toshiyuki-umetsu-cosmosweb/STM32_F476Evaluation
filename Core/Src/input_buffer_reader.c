/*
 * input_buffer_reader.c
 *
 *  Created on: Jun 22, 2022
 *      Author: toshiyuki_umetsu
 */
// clang-format off
#include <stddef.h>
#include "input_buffer_reader.h"
// clang-format on

/**
 * @brief Initialize input buffer reader.
 *
 * @param reader Reader
 * @param buf Buffer to use.
 * @param bufsize Buffer size in bytes.
 */
void input_buffer_reader_init(struct input_buffer_reader *reader, uint8_t *buf, uint32_t bufsize) {
    reader->buf = buf;
    reader->bufsize = bufsize;
    reader->in = 0u;
    reader->out = 0u;

    for (uint32_t i = 0u; i < reader->bufsize; i++) {
        reader->buf[i] = 0u;
    }

    return;
}
/**
 * @brief Deinitialize input buffer reader.
 *
 * @param reader Reader
 */
void input_buffer_reader_destroy(struct input_buffer_reader *reader) {
    reader->buf = NULL;
    reader->bufsize = 0u;
    reader->in = 0u;
    reader->out = 0u;

    return;
}

/**
 * @brief Clear input buffer reader.
 *
 * @param reader Reader
 */
void input_buffer_reader_clear(struct input_buffer_reader *reader) {
    reader->in = 0u;
    reader->out = 0u;
}

/**
 * @brief Test whether data exists or not.
 *
 * @param reader Reader
 * @retva When data exists, return true. Otherwise, return false.
 */
bool input_buffer_reader_has_data(struct input_buffer_reader *reader) { return (reader->in != reader->out); }
/**
 * @brief Getting data from input buffer reader.
 *
 * @param reader Reader
 * @retval Data. When data not exists, return 0.
 */
uint8_t input_buffer_reader_get(struct input_buffer_reader *reader) {
    if (reader->in == reader->out) {
        return 0; // No data.
    }

    uint8_t ret = reader->buf[reader->out];
    reader->out++;
    if (reader->out >= reader->bufsize) {
        reader->out = 0;
    }
    return ret;
}

/**
 * @brief Getting continuous region to write.
 *
 * @param reader Reader
 * @param length Variable to store length.
 * @return Write address returned. When blank not exists, return NULL.
 */
uint8_t *input_buffer_reader_get_writein_info(const struct input_buffer_reader *reader, uint32_t *length) {
    uint8_t *writein_ptr = NULL;

    uint32_t next_in = reader->in + 1u;
    if (next_in >= reader->bufsize) {
        next_in = 0u;
    }
    if (next_in != reader->out) { // Has blank ?
        if (reader->out <= reader->in) {
            // |----(out)///////(in)----|
            uint32_t tail_blank = (reader->out > 0u) ? reader->bufsize - reader->in : reader->bufsize - reader->in - 1u;
            if (tail_blank > 0) {
                writein_ptr = &(reader->buf[reader->in]);
                (*length) = tail_blank;
            }
        } else {
            // |////(in)------(out)/////|
            writein_ptr = &(reader->buf[reader->in]);
            (*length) = reader->out - reader->in - 1u;
        }
    }

    return writein_ptr;
}
/**
 * @brief Confirm write data.
 *
 * @param reader Reader
 * @param length Written data size in bytes.
 */
void input_buffer_reader_set_writein_done(struct input_buffer_reader *reader, uint32_t length) {
    if (reader->out <= reader->in) {
        // |----(out)///////(in)----|
        uint32_t front_blank = (reader->out > 0u) ? reader->out - 1u : 0u;
        uint32_t tail_blank = (front_blank > 0u) ? reader->bufsize - reader->in : reader->bufsize - reader->in - 1u;
        uint32_t left = length;
        if (left <= tail_blank) {
            reader->in += left;
            if (reader->in >= reader->bufsize) {
                reader->in = 0u;
            }
        } else {
            left -= tail_blank;
            reader->in += tail_blank;
            if (reader->in >= reader->bufsize) {
                reader->in = 0u;
            }
            if (left > front_blank) {
                reader->in += front_blank;
            } else {
                reader->in += left;
            }
        }
    } else {
        // |////(in)--------(out)///|
        uint32_t blank = reader->out - reader->in - 1u;
        if (length <= blank) {
            reader->in += length;
        } else {
            reader->in += blank;
        }
    }

    return;
}

/**
 * @brief Getting buffer status.
 *
 * @param reader Reader
 * @param data_bytes Variable to store data size.
 * @param blank_bytes Variable to store blank size.
 */
void input_buffer_reader_get_state(const struct input_buffer_reader *reader, uint32_t *data_bytes, uint32_t *blank_bytes) {
    uint32_t data_count;

    if (reader->in == reader->out) {
        data_count = 0u; // No data.
    } else {
        if (reader->out <= reader->in) {
            // |----(out)///////(in)----|
            data_count = (reader->in - reader->out);
        } else {
            // |////(in)--------(out)///|
            data_count = reader->in + (reader->bufsize - reader->out);
        }
    }

    (*data_bytes) = data_count;
    (*blank_bytes) = reader->bufsize - data_count;

    return;
}
