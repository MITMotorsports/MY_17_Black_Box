#include "Data_Types.h"

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the accelerator pedal positions
//Data = {right_pos, left_pos}
data_log_t extract_accel_pos(CAN_message_t &msg){
  data_log_t output;
  output.type = THROTTLE;
  output.data_length = 2;
  uint8_t data[2];
  copy_from(msg.buf, 0, 2, data, 0);
  output.data = data;

  return output;
}

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the brake pedal positions
//Data = {front_pos, rear_pos}
data_log_t extract_brake_pos(CAN_message_t &msg){
  data_log_t output;
  output.type = BRAKE;
  output.data_length = 2;
  uint8_t data[2];
  copy_from(msg.buf, 2, 2, data, 0);
  output.data = data;

  return output;
}

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the steering position
//Data = {pos}
data_log_t extract_steering_pos(CAN_message_t &msg){
  data_log_t output;
  output.type = BRAKE;
  output.data_length = 1;
  uint8_t data[2];
  copy_from(msg.buf, 4, 1, data, 0);
  output.data = data;

  return output;
}

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the front wheel speed
//Data = {right_spd[0:3], left_spd[4:7]}
data_log_t extract_front_spd(CAN_message_t &msg){
  data_log_t output;
  output.type = FRONT_SPEED;
  output.data_length = 8;
  output.data = msg.buf;

  return output;
}

int data_parse_message(CAN_message_t &msg, int (SD_Manager::*write_func)(data_log_t), SD_Manager& s){
  uint32_t id = msg.id;
  //uint8_t len = msg.len;

  int bytes_written = 0;

  switch(id){
    case FRONT_CAN_NODE_ANALOG_SENSORS: {
      bytes_written += (s.*write_func)(extract_accel_pos(msg));
      bytes_written += (s.*write_func)(extract_brake_pos(msg));
      bytes_written += (s.*write_func)(extract_steering_pos(msg));
      }
      break;
    case FRONT_CAN_NODE_WHEEL_SPEED:
      bytes_written += (s.*write_func)(extract_front_spd(msg));
      break;
    default:
      break;
  }

  return bytes_written;
}
