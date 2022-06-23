/*
 * output_buffer_writer.c
 *
 *  Created on: Jun 22, 2022
 *      Author: toshiyuki_umetsu
 */
// clang-format off
#include <stddef.h>
#include "output_buffer_writer.h"
// clang-format on

/**
 * @brief Initialize buffer writer.
 *
 * @param writer Writer
 * @param buf Buffer to use.
 * @param bufsize Buffer size in bytes.
 */
void output_buffer_writer_init(struct output_buffer_writer *writer, uint8_t *buf, uint32_t bufsize) {
    writer->buf = buf;
    writer->bufsize = bufsize;
    writer->in = 0u;
    writer->out = 0u;

    for (uint32_t i = 0u; i < writer->bufsize; i++) {
        writer->buf[i] = 0u;
    }

    return;
}
/**
 * @brief Deinitialize output buffer writer.
 *
 * @param writer Writer
 */
void output_buffer_destroy(struct output_buffer_writer *writer) {
    writer->buf = NULL;
    writer->bufsize = 0u;
    writer->in = 0;
    writer->out = 0u;

    return;
}

/**
 * @brief Clear buffer writer.
 *
 * @param writer Writer.
 */
void output_buffer_writer_clear(struct output_buffer_writer *writer) {
    writer->in = 0;
    writer->out = 0u;

    return;
}

/**
 * @brief Write data to buffer.
 *
 * @param writer Writer
 * @param d Data
 */
void output_buffer_writer_put(struct output_buffer_writer *writer, uint8_t d) {
    uint32_t next = writer->in + 1u;
    if (next >= writer->bufsize) {
        next = 0;
    }

    if (next != writer->out) {
        writer->buf[writer->in] = d;
        writer->in++;
        if (writer->in >= writer->bufsize) {
            writer->in = 0;
        }
    }

    return;
}
/**
 * @brief Getting continuous region to read.
 *
 * @param writer Writer
 * @param length Variable to store length.
 * @return Read address. When data not exists, return NULL.
 */
uint8_t *output_buffer_writer_get_readout_info(const struct output_buffer_writer *writer, uint32_t *length) {
    uint8_t *readout_ptr = NULL;

    if (writer->in != writer->out) { // Data exists ?
        if (writer->out < writer->in) {
            // |---(out)//////////(in)----|
            readout_ptr = &(writer->buf[writer->out]);
            (*length) = writer->in - writer->out;
        } else {
            // |///(in)----------(out)////|
            readout_ptr = &(writer->buf[writer->out]);
            (*length) = writer->bufsize - writer->out;
        }
    }
    return readout_ptr;
}

/**
 * @brief Confirm reading.
 *
 * @param writer Writer
 * @param length Read length in bytes.
 */
void output_buffer_writer_set_readout_done(struct output_buffer_writer *writer, uint32_t length) {
    if (writer->out <= writer->in) {
        // |----(out)///////(in)----|
        uint32_t data_count = writer->in - writer->out;
        if (length > data_count) {
            writer->out = writer->in;
        } else {
            writer->out += length;
        }
    } else {
        // |////(in)--------(out)///|
        uint32_t tail_data_count = writer->bufsize - writer->out;
        if (length >= tail_data_count) {
            length -= tail_data_count;
            writer->out = 0;
            if (length > 0) {
                uint32_t front_data_count = writer->in;
                if (length >= front_data_count) {
                    writer->out += front_data_count;
                } else {
                    writer->out += length;
                }
            }
        } else {
            writer->out += length;
        }
    }
    return;
}

/**
 * @brief Test whether blank exists or not.
 *
 * @param writer Writer
 * @return If blank exists, return true. Otherwise, return false.
 */
bool output_buffer_writer_has_blank(struct output_buffer_writer *writer) {
    uint32_t in;

    in = writer->in + 1u;
    if (in >= writer->bufsize) {
        in = 0;
    }
    return (in != writer->out);
}
/**
 * @brief Getting output buffer writer status.
 *
 * @param writer Writer
 * @param data_bytes Variable to store data count.
 * @param blank_bytes Variable to store blank size.
 */
void output_buffer_writer_get_state(const struct output_buffer_writer *writer, uint32_t *data_bytes, uint32_t *blank_bytes) {
    uint32_t data_count;

    if (writer->in == writer->out) {
        data_count = 0u; // No data.
    } else {
        if (writer->out <= writer->in) {
            // |----(out)///////(in)----|
            data_count = (writer->in - writer->out);
        } else {
            // |////(in)--------(out)///|
            data_count = writer->in + (writer->bufsize - writer->out);
        }
    }

    (*data_bytes) = data_count;
    (*blank_bytes) = writer->bufsize - data_count;

    return;
}
