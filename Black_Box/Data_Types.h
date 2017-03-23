#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <functional>
#include <FlexCAN.h>
#include "utils.h"
#include "CAN_IDs.h"
#include "Data_Log_Struct.h"
#include "SD_Manager.h"

#define TIME 0
#define BATTERY_SOC 1
#define BATTERY_TEMP 2
#define BATTERY_VOLTAGE 3
#define THROTTLE 4
#define BRAKE 5
#define STEERING 6
#define FRONT_SPEED 7
#define BB_ACCEL 8
#define BB_GYRO 9
#define BB_MAG 10

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
//int data_parse_message(CAN_message_t &msg, int (*write_func)(data_log_t));
//int parse_message(CAN_message_t &msg, std::function<int(const SD_Manager&, data_log_t)> write_func);
int data_parse_message(CAN_message_t &msg, int (SD_Manager::*write_func)(data_log_t), SD_Manager& s);

#endif
