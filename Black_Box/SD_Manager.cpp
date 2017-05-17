#include "SD_Manager.h"

SdFatSdioEX sdEx;

SD_Manager::SD_Manager(){
  //sdEx = new SdFatEX();
  data_pos = 0;
  line_count = 0;
  }

  void SD_Manager::initialize(time_t cur_time){
    if (!sdEx.begin()) {
          sdEx.initErrorHalt("SdFatSdioEX begin() failed");
        }

    // make sdEx the current volume.
    bool didItOpen = sdEx.chdir();
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

    fnLen = name_len + 4 + 1; //4 for ".txt" and null terminator
    char tmpFileNameBuff[fnLen];
    for(int i = 0; i < fnLen-4; i++){
      tmpFileNameBuff[i] = fileName[i];
    }

    String log_text = String(".txt");
    for(int i = 0; i < 4; i++){
      tmpFileNameBuff[(i+name_len)] = log_text[i];
    }
    tmpFileNameBuff[name_len+5] = '\0';

    int new_len;
    //Check if file with same name exists, change name if it does

    if(sdEx.exists(tmpFileNameBuff)){
      //add an 'A' to the end of the name, increase the letter if file still taken
      new_len = fnLen + 1;
      char newFileNameBuff[new_len]; //New array with extra space for new char
      copy_into(tmpFileNameBuff, fnLen, newFileNameBuff, 0);

      //push back the '.txt\0'
      for(int i = new_len-2; i >= new_len-6; i--){
        newFileNameBuff[i+1] = newFileNameBuff[i];
      }

      //check if file name with added 'A' exists, if try 'B' then 'C' etc...
      char suffix = 'A';
      newFileNameBuff[new_len-6] = suffix;
      while(sdEx.exists(newFileNameBuff)){
        suffix = char(int(suffix)+1);
        newFileNameBuff[new_len-6] = suffix;
      }

      //found a good file name, use it
      fileNameBuff = new char[new_len];
      // serial.println(fileNameBuff);
      copy_into(newFileNameBuff, new_len, fileNameBuff, 0);
    }else{
      fileNameBuff = new char[fnLen];
      copy_into(tmpFileNameBuff, fnLen, fileNameBuff, 0);
    }

    didItOpen = data_file.open(fileNameBuff,O_RDWR|O_CREAT);
    data_file.close();
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

  fnLen = name_len + 4 + 1; //4 for ".txt" and null terminator
  serial.print("fnlen: ");
  serial.println(fnLen);
  char tmpFileNameBuff[fnLen];
  // serial.print("fnlen: ");
  // serial.println(fnLen);
  for(int i = 0; i < fnLen-4; i++){
    // serial.print(i);
    // serial.print(" ");
    // serial.println(fileName[i]);
    tmpFileNameBuff[i] = fileName[i];
    serial.println(fileName[i]);
  }

  String log_text = String(".txt");
  for(int i = 0; i < 4; i++){
    // serial.print(i+name_len);
    // serial.print(" ");
    // serial.println(log_text[i]);
    // serial.print(" ");
    // serial.println(fileNameBuff[i-1]);
    tmpFileNameBuff[(i+name_len)] = log_text[i];
    serial.println(log_text[i]);
  }
  tmpFileNameBuff[name_len+5] = '\0';

  int new_len;
  //Check if file with same name exists, change name if it does
  serial.print(tmpFileNameBuff);
  serial.println("[Checking if this name exists]");
  serial.print("exists?: ");
  serial.println(sdEx.exists(tmpFileNameBuff));


  if(sdEx.exists(tmpFileNameBuff)){
    //add an 'A' to the end of the name, increase the letter if file still taken
    new_len = fnLen + 1;
    char newFileNameBuff[new_len]; //New array with extra space for new char
    copy_into(tmpFileNameBuff, fnLen, newFileNameBuff, 0);

    //push back the '.txt\0'
    for(int i = new_len-2; i >= new_len-6; i--){
      newFileNameBuff[i+1] = newFileNameBuff[i];
    }

    //check if file name with added 'A' exists, if try 'B' then 'C' etc...
    char suffix = 'A';
    newFileNameBuff[new_len-6] = suffix;
    while(sdEx.exists(newFileNameBuff)){
      suffix = char(int(suffix)+1);
      newFileNameBuff[new_len-6] = suffix;
    }

    //found a good file name, use it
    fileNameBuff = new char[new_len];
    // serial.println(fileNameBuff);
    copy_into(newFileNameBuff, new_len, fileNameBuff, 0);
    // serial.println(fileNameBuff);
    for(int i = 0; i < new_len; i++){
      serial.print(fileNameBuff[i]);
    }
    serial.print("[name existed this choosen instead]");
    serial.println();
    // serial.println(fileNameBuff);
  }else{
    fileNameBuff = new char[fnLen];
    copy_into(tmpFileNameBuff, fnLen, fileNameBuff, 0);
    for(int i = 0; i < fnLen; i++){
      serial.print(fileNameBuff[i]);
    }
    serial.print("[name did not exist]");
    serial.println();
  }

  serial.print("CURRENT FILE NAME: ");
  serial.println(fileNameBuff);
  didItOpen = data_file.open(fileNameBuff,O_RDWR|O_CREAT);
  serial.println(didItOpen);
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
int SD_Manager::write_raw_data(CAN_message_t &msg){
  //do not write if file is already open
  if(data_file.isOpen()){
    return -1;
  }else{
    data_file.open(fileNameBuff,O_RDWR);
  }

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

  //print the data type
  int id_print_len = int_byte_length(msg.id);
  char *id_buff = new char[id_print_len];
  to_ascii_array(msg.id, id_buff, id_print_len);
  for(int i = 0; i < id_print_len; i++){
    bytes_written += data_file.write(id_buff[i]);
  }
  bytes_written += data_file.write('_');

  //print the msg length
  bytes_written += data_file.write((char)msg.len+48);
  bytes_written += data_file.write('_');

  //print the data
  bytes_written += data_file.write(&msg.buf, (size_t)msg.len);
  bytes_written += data_file.write('_');

  //print the line count
  int lc_print_len = int_byte_length(line_count);
  // serial.print("print_len: ");
  // serial.println(lc_print_len);
  // serial.print("line_count: ");
  // serial.println(line_count);
  char *lc_buff = new char[lc_print_len];
  to_ascii_array(line_count, lc_buff, lc_print_len);
  for(int i = 0; i < lc_print_len; i++){
    bytes_written += data_file.write(lc_buff[i]);
  }
  bytes_written += data_file.write('\n');
  line_count++;

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

  serial.print("Start Pos: ");
  serial.println(data_pos);
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
    serial.write(tsBuff[i]);
  }
  //print the data type
  int id_print_len = int_byte_length(msg.id);
  char *id_buff = new char[id_print_len];
  to_ascii_array(msg.id, id_buff, id_print_len);
  for(int i = 0; i < id_print_len; i++){
    bytes_written += data_file.write(id_buff[i]);
    serial.print(id_buff[i]);
  }
  bytes_written += data_file.write('_');
  serial.write('_');

  //print the msg length
  bytes_written += data_file.write((char)msg.len+48);
  bytes_written += data_file.write('_');
  serial.write((char)msg.len+48);
  serial.write('_');

  //print the data
  bytes_written += data_file.write(&msg.buf, (size_t)msg.len);
  bytes_written += data_file.write('_');
  for(int i = 0; i < msg.len; i++){
    serial.write(msg.buf[i]);
  }
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
    bytes_written += data_file.write(lc_buff[i]);
    // serial.print("index[");
    // serial.print(i);
    // serial.print("]: ");
    serial.print(lc_buff[i]);
  }
  bytes_written += data_file.write('\n');
  serial.write('\n');
  // serial.print("print whole buf: ");
  // serial.print(lc_buff);
  // serial.println("[]");
  line_count++;

  data_pos = data_file.curPosition();
  serial.print("Final Pos: ");
  serial.println(data_pos);
  data_file.close();
  return bytes_written;
}

void SD_Manager::list_files(HardwareSerial *s){
  sdEx.ls(s);
}

int SD_Manager::dump_file(char* path, int path_len){
  bool good = false;
  if(data_file.isOpen()){
    good = true;
  }else{
    // bool good = data_file.open("4_28_15.txt",O_RDWR);
    good = data_file.open(path,O_RDWR);
  }

  data_file.rewind();
  int bytes_read = 0;
  int under_count = 0;
  while (data_file.available()) {
    int tmp = data_file.read();
    if(tmp == int('_')){
      under_count++;
    }
    if(under_count == 1){
      int msglen = data_file.read();
      int tmp2 = data_file.read();
      bytes_read = bytes_read + XBEE.write(byte(tmp));
      bytes_read = bytes_read + XBEE.write(byte(msglen));
      bytes_read = bytes_read + XBEE.write(byte(tmp2));

      msglen = msglen - 48; //convert to int from ascii
      for(int i = 0; i < msglen; i++){
        tmp = data_file.read();
        bytes_read = bytes_read + XBEE.write(byte(tmp));
      }
    }else if(under_count > 1 & tmp == int('\n')){
      bytes_read = bytes_read + XBEE.write('\n');
    }else {
      bytes_read = bytes_read + XBEE.write(byte(tmp));
    }
  }
  XBEE.write("end\n");
  data_file.close();
  return bytes_read;
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
  int under_count = 0;
  serial.print("data_file available?: ");
  serial.println(data_file.available());
  while (data_file.available()) {
    int tmp = data_file.read();
    serial.print(char(tmp));
    if(tmp == int('_')){
      under_count++;
    }
    if(under_count == 1){
      int msglen = data_file.read();
      int tmp2 = data_file.read();
      bytes_read = bytes_read + XBEE.write(byte(tmp));
      bytes_read = bytes_read + XBEE.write(byte(msglen));
      bytes_read = bytes_read + XBEE.write(byte(tmp2));

      msglen = msglen - 48; //convert to int from ascii
      for(int i = 0; i < msglen; i++){
        tmp = data_file.read();
        bytes_read = bytes_read + XBEE.write(byte(tmp));
      }
    }else if(under_count > 1 & tmp == int('\n')){
      bytes_read = bytes_read + XBEE.write('\n');
      serial.println("Line End");
    }else {
      bytes_read = bytes_read + XBEE.write(byte(tmp));
    }
  }
  XBEE.write("end\n");
  data_file.close();
  return bytes_read;
}

bool SD_Manager::open_file(char* path){
  return data_file.open(path,O_RDWR);
}
