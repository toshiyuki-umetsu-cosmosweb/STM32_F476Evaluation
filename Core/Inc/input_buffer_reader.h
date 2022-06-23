/*
 * input_buffer_reader.h
 *
 *  Created on: Jun 22, 2022
 *      Author: toshiyuki_umetsu
 */

#ifndef INC_INPUT_BUFFER_READER_H_
#define INC_INPUT_BUFFER_READER_H_

#include <stdbool.h>
#include <stdint.h>

struct input_buffer_reader {
    uint8_t *buf;     // Buffer
    uint32_t bufsize; // Buffer size in bytes.
    uint32_t out;     // Read out
    uint32_t in;      // Write in
};

void input_buffer_reader_init(struct input_buffer_reader *reader, uint8_t *buf, uint32_t bufsize);
void input_buffer_reader_destroy(struct input_buffer_reader *reader);
void input_buffer_reader_clear(struct input_buffer_reader *reader);

bool input_buffer_reader_has_data(struct input_buffer_reader *reader);
uint8_t input_buffer_reader_get(struct input_buffer_reader *reader);
uint8_t *input_buffer_reader_get_writein_info(const struct input_buffer_reader *reader, uint32_t *length);
void input_buffer_reader_set_writein_done(struct input_buffer_reader *reader, uint32_t length);
void input_buffer_reader_get_state(const struct input_buffer_reader *reader, uint32_t *data_bytes, uint32_t *blank_bytes);

#endif /* INC_INPUT_BUFFER_READER_H_ */
