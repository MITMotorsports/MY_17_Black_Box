#include "utils.h"

void float_to_int(float val, uint8_t *int_array){
  // Create union of shared memory space
  union {
    float float_variable;
    uint8_t temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(int_array, u.temp_array, 4);
}

void copy_into(uint8_t *source_array, int source_len, uint8_t *dest_array, int dest_start_ind){
  for(int i = dest_start_ind; i < (dest_start_ind+source_len); i++){
    dest_array[i] = source_array[i - dest_start_ind];
  }
}

void copy_from(uint8_t *source_array, int source_start_ind, int source_len, uint8_t *dest_array, int dest_start_ind){
  for(int i = source_start_ind; i < (source_start_ind+source_len); i++){
    dest_array[dest_start_ind + i - source_start_ind] = source_array[i];
  }
}
