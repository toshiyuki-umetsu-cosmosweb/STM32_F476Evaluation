/*
 * output_buffer_writer.h
 *
 *  Created on: Jun 22, 2022
 *      Author: toshiyuki_umetsu
 */

#ifndef INC_OUTPUT_BUFFER_WRITER_H_
#define INC_OUTPUT_BUFFER_WRITER_H_

#include <stdbool.h>
#include <stdint.h>

struct output_buffer_writer {
    uint8_t *buf;     // Buffer
    uint32_t bufsize; // Buffer size in bytes.
    uint32_t out;     // Read out
    uint32_t in;      // Write in
};

void output_buffer_writer_init(struct output_buffer_writer *writer, uint8_t *buf, uint32_t bufsize);
void output_buffer_destroy(struct output_buffer_writer *writer);

void output_buffer_writer_clear(struct output_buffer_writer *writer);
void output_buffer_writer_put(struct output_buffer_writer *writer, uint8_t d);
uint8_t *output_buffer_writer_get_readout_info(const struct output_buffer_writer *writer, uint32_t *length);
void output_buffer_writer_set_readout_done(struct output_buffer_writer *writer, uint32_t length);
bool output_buffer_writer_has_blank(struct output_buffer_writer *writer);

void output_buffer_writer_get_state(const struct output_buffer_writer *writer, uint32_t *data_bytes, uint32_t *blank_bytes);

#endif /* INC_OUTPUT_BUFFER_WRITER_H_ */
