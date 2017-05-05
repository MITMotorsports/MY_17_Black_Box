#include "SD_Manager.h"

SdFatSdioEX sdEx;

SD_Manager::SD_Manager(){
  //sdEx = new SdFatEX();
  data_pos = 0;
  }

void SD_Manager::initialize(time_t cur_time, usb_serial_class &serial){
  if (!sdEx.begin()) {
        sdEx.initErrorHalt("SdFatSdioEX begin() failed");
      }

  // make sdEx the current volume.
  bool didItOpen = sdEx.chdir();
  // serial.println(didItOpen);
  sdEx.chvol();

  //create logging file
  int tmp = month(cur_time);
  String month = String(tmp, DEC);
  int fnLen = month.length();

  tmp = day(cur_time);
  String day = String(tmp, DEC);
  fnLen = fnLen + day.length();

  tmp = hour(cur_time);
  String hour = String(tmp, DEC);
  fnLen = fnLen + hour.length();

  String s = String('_');

  String fileName = month + s + day + s + hour;
  int name_len = fileName.length();

  fnLen = name_len + 4; //4 for ".txt"
  fileNameBuff = new char[fnLen];
  // serial.print("fnlen: ");
  // serial.println(fnLen);
  for(int i = 0; i < fnLen-4; i++){
    // serial.print(i);
    // serial.print(" ");
    // serial.println(fileName[i]);
    fileNameBuff[i] = fileName[i];
  }

  String log_text = String(".txt");
  for(int i = 0; i < 4; i++){
    // serial.print(i+name_len);
    // serial.print(" ");
    // serial.println(log_text[i]);
    // serial.print(" ");
    // serial.println(fileNameBuff[i-1]);
    fileNameBuff[(i+name_len)] = log_text[i];
  }

  //Check if file with same name exists, change name if it does
  if(sdEx.exists(fileNameBuff)){
    //add an 'A' to the end of the name, increase the letter if file still taken
    int new_len = fnLen + 1;
    char newFileNameBuff[new_len]; //New array with extra space for new char
    copy_into(fileNameBuff, fnLen, newFileNameBuff, 0);

    //push back the '.txt'
    for(int i = new_len-2; i >= new_len-5; i--){
      newFileNameBuff[i+1] = newFileNameBuff[i];
    }

    //check if file name with added 'A' exists, if try 'B' then 'C' etc...
    char suffix = 'A';
    newFileNameBuff[new_len-5] = suffix;
    while(sdEx.exists(newFileNameBuff)){
      suffix = char(int(suffix)+1);
      newFileNameBuff[new_len-5] = suffix;
    }

    //found a good file name, use it
    Serial.print("CURRENT FILE NAME: ");
    serial.println(newFileNameBuff);
    didItOpen = data_file.open(newFileNameBuff,O_RDWR|O_CREAT);
    serial.println(didItOpen);
    data_file.close();
    sdEx.ls(serial);
  }else{
    Serial.print("CURRENT FILE NAME: ");
    serial.println(fileNameBuff);
    didItOpen = data_file.open(fileNameBuff,O_RDWR|O_CREAT);
    serial.println(didItOpen);
    data_file.close();
    sdEx.ls(serial);
  }
}

  // //Wrapper for the data_types parse_message function
  // void SD_Manager::parse_message(CAN_message_t &msg){
  //   //std::function<int(SD_Manager&, data_log_t)> write_func = &SD_Manager::writeData;
  //   data_parse_message(msg, &SD_Manager::writeData, this);
  // }



int SD_Manager::writeData(data_log_t data){
  bool didItOpen = data_file.open(fileNameBuff,O_RDWR);

  data_file.seekSet(data_pos);
  int bytes_written = 0;
  //print the timestamp
  long t = now();
  String ts = String(t, DEC);
  ts = ts + "_";
  int tsLen = ts.length();
  char *tsBuff = new char[tsLen];
  to_ascii_array(ts, tsBuff, tsLen);
  bytes_written += data_file.write(tsBuff, tsLen);

  // print the data type
  bytes_written += data_file.write(&data.type, sizeof(char[1]));

  // print the data
  bytes_written += data_file.write(&data.data, sizeof(char[data.data_length]));
  bytes_written += data_file.write("\n");

  data_pos = data_file.curPosition();
  data_file.close();
  return bytes_written;
}

//Writes a raw CAN message into an SD card file
//The format is:
//'TIMESTAMP'_'CAN ID'_'MESSAGE LENGTH'_'MESSAGE DATA'_'LINE COUNT'
//Everything is written as ASCCI except MESSAGE DATA which is just bytes
int SD_Manager::write_raw_data(CAN_message_t &msg, usb_serial_class &serial){
  //do not write if file is already open
  if(data_file.isOpen()){
    return -1;
  }else{
    data_file.open(fileNameBuff,O_RDWR);
  }

  Serial.print("Start Pos: ");
  Serial.println(data_pos);
  data_file.seekSet(data_pos);
  int bytes_written = 0;

  //print the timestamp
  long t = now();
  String ts = String(t, DEC);
  ts = ts + "_";
  int tsLen = ts.length();
  char *tsBuff = new char[tsLen];
  to_ascii_array(ts, tsBuff, tsLen);
  bytes_written += data_file.write(tsBuff, tsLen);
  for(int i = 0; i < tsLen; i++){
    serial.print(tsBuff[i]);
  }
  //print the data type
  uint8_t* id_array = (uint8_t*) &msg.id;

  bytes_written += data_file.write(id_array, sizeof(id_array));
  serial.write(id_array[0]);
  bytes_written += data_file.write('_');
  //print the data
  bytes_written += data_file.write(&msg.buf, (size_t)msg.len);
  serial.write(msg.buf[0]);
  serial.println();
  bytes_written += data_file.write('\n');

  data_pos = data_file.curPosition();
  Serial.print("Final Pos: ");
  Serial.println(data_pos);
  data_file.close();
  return bytes_written;
}

void SD_Manager::list_files(HardwareSerial *s){
  sdEx.ls(s);
}

int SD_Manager::dump_file(usb_serial_class &serial, char* path, int path_len){
  bool good = false;
  if(data_file.isOpen()){
    serial.println("File is Open");
    good = true;
  }else{
    serial.println("File is Closed, Opening File");
    // bool good = data_file.open("4_28_15.txt",O_RDWR);
    good = data_file.open(path,O_RDWR);
  }

  if(good){
    serial.print("Dumping File: ");
    for(int i = 0; i < path_len-1; i++){
      serial.print(path[i]);
    }
    serial.println();
  }

  data_file.rewind();
  int bytes_read = 0;
  serial.println("Line Start");
  int line_count = 0;
  while (data_file.available()) {
    int tmp = data_file.read();
    serial.print(char(tmp));
    if(tmp == int('\n')){
      bytes_read = bytes_read + XBEE.write('_');
      bytes_read = bytes_read + XBEE.print(line_count);
      bytes_read = bytes_read + XBEE.write('\n');
      line_count++;
      serial.println("Line End");
    }else {
      bytes_read = bytes_read + XBEE.write(byte(tmp));
    }
  }
  XBEE.write("end\n");
  data_file.close();
  serial.print("Lines Sent: ");
  serial.println(line_count);
  return bytes_read;
}

bool SD_Manager::open_file(char* path){
  return data_file.open(path,O_RDWR);
}
