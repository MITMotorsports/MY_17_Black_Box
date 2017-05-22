#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include "Arduino.h"
#include "SdFat.h"
#include <FlexCAN.h>
#include <TimeLib.h>
#include "Data_Log_Struct.h"
#include "Xbee_Manager.h"
#include "utils.h"


extern SdFatSdioEX sdEx;

/* --- SD MANAGER ---
* This class is responsible for interfacing with the SD card in the Black Box
* Every CAN message recieved by the Black Box should be sent to the parse_message
* function which will determine if the message should be logged or not.
*/
class SD_Manager {
  public:
    SD_Manager();
    void initialize(time_t cur_time, usb_serial_class &serial);
    //void parse_message(CAN_message_t &msg);
    int write_raw_data(CAN_message_t &msg);
    int write_raw_data(CAN_message_t &msg, usb_serial_class &serial);
    int writeData(data_log_t data);
    void list_files(HardwareSerial *s);
    bool open_file(char* path);
    int dump_file(char* path, int path_len);
    int dump_file(char* path, int path_len,
                  Xbee_Manager* xbee, usb_serial_class &serial);
    char line_buffer[100];
  private:
    //string? getDataFormat();
    //SdFatEX *sdEx;
    String fileName;
    char *fileNameBuff;
    File data_file;
    uint32_t data_pos;
    int line_count;
    uint32_t log_start; //The millis() when the data log starts
    int write_32_bit(uint32_t data, int num_bytes);
};

#endif
