#include <FlexCAN.h>

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the accelerator pedal positions
//Data = {right_pos, left_pos}
data_log_t extract_accel_pos(CAN_message_t &msg){
  data_log_t output;
  output.type = THROTTLE;
  output.data_length = 2;
  output.data = {msg.buf[0], msg.buf[1]};

  return output;
}

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the brake pedal positions
//Data = {front_pos, rear_pos}
data_log_t extract_brake_pos(CAN_message_t &msg){
  data_log_t output;
  output.type = BRAKE;
  output.data_length = 2;
  output.data = {msg.buf[2], msg.buf[3]};

  return output;
}

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the steering position
//Data = {pos}
data_log_t extract_steering_pos(CAN_message_t &msg){
  data_log_t output;
  output.type = BRAKE;
  output.data_length = 1;
  output.data = {msg.buf[4]};

  return output;
}

//Accepts a CAN Message from the Front CAN node
//Returns a Data Log with the front wheel speed
//Data = {right_spd[0:3], left_spd[4:7]}
data_log_t extract_front_spd(CAN_message_t &msg);
  data_log_t output;
  output.type = FRONT_SPEED;
  output.data_length = 8;
  output.data = msg.buf;

  return output;
}

int parse_message(CAN_message_t &msg, int (*write_func)(data_log_t)){
  uint32_t id = msg.id;
  uint8_t len = msg.len;

  int bytes_written = 0;

  switch(id){
    case 'CAN node sensors id': {
      bytes_written += (*write_func)(extract_accel_pos(msg));
      bytes_written += (*write_func)(extract_brake_pos(msg));
      bytes_written += (*write_func)(extract_steering_pos(msg));
      }
      break;
    case 'CAN node speed id':
      bytes_written += (*write_func)(extract_front_spd(msg));
      break;
    default:
      break;
  }

  return bytes_written;
}
