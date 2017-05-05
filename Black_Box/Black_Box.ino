#include <TimeLib.h>
#include <FlexCAN.h>
#include <stdio.h>

#include "Xbee_Manager.h"
#include "SD_Manager.h"
#include "IMU_Manager.h"
#include "utils.h"

//SD Card Variables
SD_Manager sd;

//CAN Variables
FlexCAN CAN_bus_main(500000, 0);
FlexCAN CAN_bus_sensor(500000, 1);
static CAN_message_t msg;

//IMU Variables
//IMU_Manager imu;

//Xbee Variables
Xbee_Manager xbee;

void setup(){
  //Serial initialization
  Serial.begin(9600); //start Serial port, baud rate defaults to full USB speed

  //CAN initialization
  CAN_bus_main.begin();
  CAN_bus_sensor.begin();

  //RTC initialization
  setSyncProvider(getTeensy3Time);
  while(!Serial);
  delay(100);
  if (timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }

  //SD initialization
  int tmp = year(now());
  String year = String(tmp, DEC);
  tmp = month(now());
  String month = String(tmp, DEC);
  tmp = day(now());
  String day = String(tmp, DEC);
  tmp = hour(now());
  String hour = String(tmp, DEC);
  tmp = minute(now());
  String minute = String(tmp, DEC);
  tmp = second(now());
  String second = String(tmp, DEC);
  Serial.println("Current Time: "+day+'-'+month+'-'+year+' '+hour+':'+minute+':'+second);

  sd.initialize(now(), Serial);

  //Xbee initialization
  xbee.initialize();
}

void loop(){
  //Serial.println(hour(now()));

  //Check the Main bus for messages
  if(CAN_bus_main.available()){
    CAN_bus_main.read(msg); //Read message into CAN_message_t buffer
    //sd.parse_message(msg); //send to SD manager for logging
    //data_parse_message(msg, &SD_Manager::writeData, sd);
    //TODO add xBee Parser
    //TODO add command Parser
    //TODO add transfer Parser
  }

  //Check the Sensor bus for messages
  if(CAN_bus_sensor.available()){
    CAN_bus_sensor.read(msg); //Read message into CAN_message_t buffer
    for(int i = 0; i<8; i++){
      Serial.print(char(msg.buf[i]));
    }
    Serial.println();
    int a = sd.write_raw_data(msg, Serial);
    Serial.println(a);
    //sd.parse_message(msg); //send to SD manager for logging
    //data_parse_message(msg, &SD_Manager::writeData, sd);
    //TODO add xBee Parser
    //TODO add command Parser
    //TODO add transfer Parser
  }

  int cmd = xbee.check_for_message(Serial);
  if(cmd == LS){
    sd.list_files(&XBEE);
    XBEE.write("end\n");
  }else if(cmd == OPEN){
    int path_len = xbee.read_file_name();
    char path[path_len];
    for(int i = 0; i < path_len-1; i++){
      path[i] = xbee.file_name_buf[i];
      // Serial.print(path[i]);
    }
    // Serial.println();
    sd.open_file(path);
    int bytes_read = sd.dump_file(Serial, path, path_len);

    Serial.println(bytes_read);
  }
}


time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}
