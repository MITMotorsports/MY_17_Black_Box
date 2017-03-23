#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"

void float_to_int(float val, uint8_t *int_array);
void copy_into(uint8_t *source_array, int source_len, uint8_t *dest_array, int dest_start_ind);
void copy_from(uint8_t *source_array, int source_start_ind, int source_len, uint8_t *dest_array, int dest_start_ind);

#endif
