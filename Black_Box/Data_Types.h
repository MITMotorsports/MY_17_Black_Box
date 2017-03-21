#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#define TIME 0
#define BATTERY_SOC 1
#define BATTERY_TEMP 2
#define BATTERY_VOLTAGE 3
#define THROTTLE 4
#define BRAKE 5
#define STEERING 6
#define FRONT_SPEED 7

typedef struct data_log_t {
  int type;
  int data_length;
  uint8_t *data;
} data_log_t;

//CAN NODE
//ANALOG SENSORS
data_log_t extract_accel_pos(CAN_message_t &msg);
data_log_t extract_brake_pos(CAN_message_t &msg);
data_log_t extract_steering_pos(CAN_message_t &msg);
//WHEEL SPEED
data_log_t extract_front_spd(CAN_message_t &msg);

//Accepts a CAN message and a write function
//Returns the number of bytes written by the write function
//The write function should accept a data log and then return the number of
//bytes written 
int parse_message(CAN_message_t &msg, int (*write_func)(data_log_t));

#endif
