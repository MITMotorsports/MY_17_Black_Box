#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include "Arduino.h"
#include <functional>
#include "SdFat.h"
#include <FlexCAN.h>
#include <TimeLib.h>
#include "Data_Log_Struct.h"


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
    int writeData(data_log_t data);
    //char* read_line();
  private:
    //string? getDataFormat();
    //SdFatEX *sdEx;
    String fileName;
    char *fileNameBuff;
    File data_file;
};

#endif
