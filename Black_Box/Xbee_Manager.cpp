#include "Xbee_Manager.h"

int ID_LIST[NUM_MSG_TYPES] = {1320, 48};

Xbee_Manager::Xbee_Manager(){
  xbeeBufferLen = 60;
  xbeeBuffer = new char[xbeeBufferLen];
  file_name_buf_len = 60;
  file_name_buf = new char[file_name_buf_len];
  line_count = 0;
}

void Xbee_Manager::initialize(){
  XBEE.begin(57600, SERIAL_8E1);
}

int Xbee_Manager::check_for_message(usb_serial_class &serial){
  if(XBEE.available()){
    int bytesRead = XBEE.readBytesUntil('\n', xbeeBuffer, xbeeBufferLen);
    //recognize an LS
    if(bytesRead == 2){
      char ls[2] = {'l','s'};
      if(compare_long_to_short(xbeeBuffer, ls, 2)){
        return LS;
      }
      //recognize an OPEN
    }else if(bytesRead == 4){
      char open[4] = {'o','p','e','n'};
      if(compare_long_to_short(xbeeBuffer, open, 4)){
        return OPEN;
      }
    }
  }
  return 0;
}

int Xbee_Manager::read_file_name(){
  int TIMEOUT = 2000;
  int start = millis();
  while(millis() - start < TIMEOUT){
    if(XBEE.available()){
      int bytesRead = XBEE.readBytesUntil('\n', file_name_buf, file_name_buf_len);
      return bytesRead;
    }
  }
  return 0;
}

bool Xbee_Manager::is_live_data(int id){
  bool is_live_data = false;

  for(int i = 0; i < NUM_MSG_TYPES; i++){
    if(id == ID_LIST[i]){
      is_live_data = true;
      break;
    }
  }

  return is_live_data;
}

int Xbee_Manager::write_raw_data(CAN_message_t &msg, usb_serial_class &serial){
  //do not write if file is already open
  if(!is_live_data(msg.id)){
    return -1;
  }

  int bytes_written = 0;

  //print the timestamp
  long t = now();
  String ts = String(t, DEC);
  ts = ts + "_";
  int tsLen = ts.length();
  char *tsBuff = new char[tsLen];
  to_ascii_array(ts, tsBuff, tsLen);
  for(int i = 0; i < tsLen; i++){
    bytes_written += XBEE.write(tsBuff[i]);
    serial.write(tsBuff[i]);
  }
  //print the data type
  int id_print_len = int_byte_length(msg.id);
  char *id_buff = new char[id_print_len];
  to_ascii_array(msg.id, id_buff, id_print_len);
  for(int i = 0; i < id_print_len; i++){
    bytes_written += XBEE.write(id_buff[i]);
    serial.print(id_buff[i]);
  }
  bytes_written += XBEE.write('_');
  serial.write('_');

  //print the msg length
  bytes_written += XBEE.write((char)msg.len+48);
  bytes_written += XBEE.write('_');
  serial.write((char)msg.len+48);
  serial.write('_');

  //print the data
  for(int i = 0; i < msg.len; i++){
    bytes_written += XBEE.write(msg.buf[i]);
    serial.write(msg.buf[i]);
  }
  bytes_written += XBEE.write('_');
  serial.write('_');
  // serial.println();
  //print the line count
  int lc_print_len = int_byte_length(line_count);
  // serial.print("print_len: ");
  // serial.println(lc_print_len);
  // serial.print("line_count: ");
  // serial.println(line_count);
  char *lc_buff = new char[lc_print_len];
  to_ascii_array(line_count, lc_buff, lc_print_len);
  for(int i = 0; i < lc_print_len; i++){
    bytes_written += XBEE.write(lc_buff[i]);
    // serial.print("index[");
    // serial.print(i);
    // serial.print("]: ");
    serial.print(lc_buff[i]);
  }
  bytes_written += XBEE.write('\n');
  serial.write('\n');
  // serial.print("print whole buf: ");
  // serial.print(lc_buff);
  // serial.println("[]");
  line_count++;

  return bytes_written;
}
