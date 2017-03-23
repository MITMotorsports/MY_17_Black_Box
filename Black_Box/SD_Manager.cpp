#include "SD_Manager.h"

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

  // //Wrapper for the data_types parse_message function
  // void SD_Manager::parse_message(CAN_message_t &msg){
  //   //std::function<int(SD_Manager&, data_log_t)> write_func = &SD_Manager::writeData;
  //   data_parse_message(msg, &SD_Manager::writeData, this);
  // }

int SD_Manager::writeData(data_log_t data){
  bool didItOpen = data_file.open(fileNameBuff,O_RDWR);

  int bytes_written = 0;
  //print the timestamp
  long t = now();
  String ts = String(t, DEC);
  ts = ts + "_";
  int tsLen = ts.length();
  char *tsBuff = new char[tsLen];
  bytes_written += data_file.write(tsBuff, tsLen);

  //print the data type
  bytes_written += data_file.write(&data.type, sizeof(char[1]));

  //print the data
  bytes_written += data_file.write(&data.data, sizeof(char[data.data_length]));

  data_file.close();
  return bytes_written;
}

// char* SD_Manager::readLine(){
//
// }
