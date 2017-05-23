#include "Xbee_Manager.h"

int ID_LIST[NUM_MSG_TYPES] = {1320, 48};
int ID_PERIOD[NUM_MSG_TYPES] = {100, 100};
int ID_LAST_SENT[NUM_MSG_TYPES] = {0, 0};

Xbee_Manager::Xbee_Manager(){
  xbeeBufferLen = 60;
  xbeeBuffer = new char[xbeeBufferLen];
  file_name_buf_len = 60;
  file_name_buf = new char[file_name_buf_len];
  line_count = 0;
}

void Xbee_Manager::initialize(){
  //The xbee actually runs at 111111 baud not 115200 since its 16MHz clock
  //cannot be evenly divided into 115200.
  XBEE.begin(111111, SERIAL_8E1);
}

int Xbee_Manager::check_for_message(){
  if(XBEE.available()){
    int bytesRead = XBEE.readBytesUntil('\n', xbeeBuffer, xbeeBufferLen);
    Serial.print("Xbee message recieved: ");
    for(int i = 0; i < bytesRead; i++){
      Serial.print(xbeeBuffer[i]);
    }
    Serial.println();
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

//This function checks if a CAN ID is in the Live data ID_LIST
//If it is, it returns the minimum period from the ID_PERIOD list
//If it is not, it returns -1
bool Xbee_Manager::valid_live(int id, int ts){
  bool valid = false;

  //try to find ID in ID_LIST
  for(int i = 0; i < NUM_MSG_TYPES; i++){
    if(id == ID_LIST[i]){
      //Check if last sent was more than the minimum period ago
      int min_period = ID_PERIOD[i];
      if(ts - ID_LAST_SENT[i] > min_period){
        valid = true; //It is time to send more data
        ID_LAST_SENT[i] = ts;
      }
      break;
    }
  }
  return valid;
}

int Xbee_Manager::write_raw_data(CAN_message_t &msg, usb_serial_class &serial, int ts){
  //Only send data that should be sent live
  if(!valid_live(msg.id, ts)){
    return -1;
  }

  int bytes_written = 0;

  //Create the data frame
  //29 bits - milliseconds since log start
  //11 bits - CAN ID
  //64 bits - DATA
  uint64_t meta = 0;
  uint64_t mil = ts; //time since log start
  mil = mil << 11; //shift the data to make room for the CAN ID
  meta = mil + msg.id; // merge the data for writing
  bytes_written += write_bytes(meta, 5);

  //write the data - 8 bytes zero padded
  for(int i = 0; i < 8; i++){
    if(i >= msg.len) {
      //print zero padding byte
      uint8_t pad = 0;
      bytes_written += XBEE.write(pad);
    }else{
      //print data byte
      bytes_written += XBEE.write(msg.buf[i]);
    }
  }

  return bytes_written;
}

//Writes multiple bytes little endian
int Xbee_Manager::write_bytes(uint32_t data, int num_bytes){
  uint8_t tmp_byte = 0;
  int bytes_written = 0;
  while(bytes_written != num_bytes){
    tmp_byte = 0;
    tmp_byte = data | tmp_byte; //take value least significant byte
    bytes_written += XBEE.write(tmp_byte);
    data = (unsigned int)data >> 8; // shift out byte
  }

  return bytes_written;
}
