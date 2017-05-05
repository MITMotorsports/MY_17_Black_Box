#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"

void float_to_int(float val, uint8_t *int_array);
void copy_into(uint8_t *source_array, int source_len, uint8_t *dest_array, int dest_start_ind);
void copy_into(char *source_array, int source_len, char *dest_array, int dest_start_ind);
void copy_from(uint8_t *source_array, int source_start_ind, int source_len, uint8_t *dest_array, int dest_start_ind);
bool compare_long_to_short(char *long_array, char *short_array, int len_short);
void to_ascii_array(String input_string, char* char_array, int input_len);
char to_ascii(char input);

#endif
