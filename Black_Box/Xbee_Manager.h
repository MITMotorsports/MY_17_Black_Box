#ifndef XBEE_MANAGER_H
#define XBEE_MANAGER_H

#include "utils.h"
#include <FlexCAN.h>
#include <TimeLib.h>

#define XBEE Serial4

//file command macros
#define LS 1
#define OPEN 2

#define NUM_MSG_TYPES 2
//This is the list of IDs that the xbee will transmit
//CURRENT_SENSOR_ENERGY - 0
//FRONT_CAN_NODE_DRIVER_OUTPUT - 1
extern int ID_LIST[NUM_MSG_TYPES];

//This is the minimum period between message that data should be sent live
//Indexes should have same ID values as ID_LIST
extern int ID_PERIOD[NUM_MSG_TYPES]; //In milliseconds

//The last time a message of ID corresponding to that index was sent
extern int ID_LAST_SENT[NUM_MSG_TYPES]; //In milliseconds


class Xbee_Manager {
  public:
    Xbee_Manager();
    void initialize();
    int check_for_message();
    bool send_message();
    int read_file_name();
    char* file_name_buf;
    int file_name_buf_len;
    bool valid_live(int id, int ts);
    int write_raw_data(CAN_message_t &msg, usb_serial_class &serial, int ts);
    int write_bytes(uint32_t data, int num_bytes);
  private:
    char* xbeeBuffer;
    int xbeeBufferLen;
    int line_count;

};

#endif
