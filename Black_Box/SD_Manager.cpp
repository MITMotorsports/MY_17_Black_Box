#include "Arduino.h"
#include "SdFat.h"
#include <TimeLib.h>
#include <FlexCAN.h>

#include "SD_Manager.h"
#include "Data_Types.h"

SdFatSdioEX sdEx;

SD_Manager::SD_Manager(){
  //sdEx = new SdFatEX();
  }

void SD_Manager::initialize(time_t cur_time, usb_serial_class &serial){
  if (!sdEx.begin()) {
        sdEx.initErrorHalt("SdFatSdioEX begin() failed");
      }

  // make sdEx the current volume.
  bool didItOpen = sdEx.chdir();
  serial.println(didItOpen);
  sdEx.chvol();

  //create logging file
  int tmp = year(cur_time);
  String year = String(tmp, DEC);
  tmp = month(cur_time);
  String month = String(tmp, DEC);
  tmp = day(cur_time);
  String day = String(tmp, DEC);
  tmp = hour(cur_time);
  String hour = String(tmp, DEC);
  String s = String("_");

  String fileName = year + s + month + s + day + s + hour + s + String("log.txt");
  Serial.println(fileName);
  int fnLen = fileName.length();
  fileNameBuff = new char[fnLen];
  fileName.toCharArray(fileNameBuff, fnLen);
  serial.println(fileNameBuff);
  didItOpen = data_file.open(fileNameBuff,O_RDWR|O_CREAT);
  serial.println(didItOpen);
  data_file.write(4);
  data_file.close();
  sdEx.ls(serial);
  }

//Wrapper for the data_types parse_message function
void SD_Manager::parse_message(CAN_message_t &msg){
  parse_message(msg, writeData);
}

int SD_Manager::writeData(data_log_t data){
  bool didItOpen = data_file.open(fileNameBuff,O_RDWR);

  bytes_written = 0;
  //print the timestamp
  long t = now();
  String ts = String(t, DEC);
  ts = ts + "_";
  int tsLen = ts.length();
  tsBuff = new char[tsLen];
  bytes_written += data_file.write(tsBuff, tsLen);

  //print the data type
  bytes_written += data_file.write(data.type, 1);

  //print the data
  bytes_written += data_file.write(data.data, data.data_length);

  data_file.close();
  return w;
}

char* SD_Manager::readLine(){

}
