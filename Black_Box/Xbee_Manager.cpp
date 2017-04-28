#include "Xbee_Manager.h"

Xbee_Manager::Xbee_Manager(){
  xbeeBufferLen = 60;
  xbeeBuffer = new char[xbeeBufferLen];
  file_name_buf_len = 60;
  file_name_buf = new char[file_name_buf_len];
}

void Xbee_Manager::initialize(){
  XBEE.begin(115200);
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
