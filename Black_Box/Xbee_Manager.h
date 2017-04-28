#ifndef XBEE_MANAGER_H
#define XBEE_MANAGER_H

#include "utils.h"

#define XBEE Serial4

//file command macros
#define LS 1
#define OPEN 2

class Xbee_Manager {
  public:
    Xbee_Manager();
    void initialize();
    int check_for_message(usb_serial_class &serial);
    bool send_message();
    int read_file_name();
    char* file_name_buf;
    int file_name_buf_len;
  private:
    char* xbeeBuffer;
    int xbeeBufferLen;

};

#endif
