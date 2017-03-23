#ifndef DATA_LOG_STRUCT_H
#define DATA_LOG_STRUCT_H

typedef struct data_log_t {
  int type;
  int data_length;
  uint8_t *data;
} data_log_t;

#endif
