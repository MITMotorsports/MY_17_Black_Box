#include "utils.h"

void float_to_int(float val, uint8_t *int_array){
  // Create union of shared memory space
  union {
    float float_variable;
    uint8_t temp_array[4];
  } u;
  // Overwrite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(int_array, u.temp_array, 4);
}

void copy_into(uint8_t *source_array, int source_len, uint8_t *dest_array, int dest_start_ind){
  for(int i = dest_start_ind; i < (dest_start_ind+source_len); i++){
    dest_array[i] = source_array[i - dest_start_ind];
  }
}

void copy_into(char *source_array, int source_len, char *dest_array, int dest_start_ind){
  for(int i = dest_start_ind; i < (dest_start_ind+source_len); i++){
    dest_array[i] = source_array[i - dest_start_ind];
  }
}

void copy_from(uint8_t *source_array, int source_start_ind, int source_len, uint8_t *dest_array, int dest_start_ind){
  for(int i = source_start_ind; i < (source_start_ind+source_len); i++){
    dest_array[dest_start_ind + i - source_start_ind] = source_array[i];
  }
}

bool compare_long_to_short(char *long_array, char *short_array, int len_short){
  for(int i = 0; i < len_short; i++){
    if(short_array[i] != long_array[i]){
      return false;
    }
  }
  return true;
}

void to_ascii_array(String input_string, char* char_array, int input_len){
  for(int i = 0; i < input_len; i++){
    char_array[i] = input_string[i];
  }
}

char to_ascii(char input){
  return ((int)input) - 48;
}

int int_byte_length(int input){
  int byte_length = 1;
  double divide = input/10.0;
  while(int(divide) >= 1){
    divide = divide/10.0;
    byte_length++;
  }
  return byte_length;
}

//convert an interger in to an ascii representation of itself
void to_ascii_array(int input, char* char_array, int input_len){
  int ind = 0;
  for(int i = input_len-1; i >= 0; i--){
    int place = pow(10, i);
    int value = input/place;
    char_array[ind] = (char)value + 48;
    input = input - place*value;
    ind++;
  }
}
