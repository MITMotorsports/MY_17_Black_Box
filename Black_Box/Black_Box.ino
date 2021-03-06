#include <TimeLib.h>
#include <FlexCAN.h>
#include <stdio.h>

#include "Xbee_Manager.h"
#include "SD_Manager.h"
#include "IMU_Manager.h"
#include "utils.h"
#include "pins.h"

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
  int serial_timeout_start = millis();
  while(!Serial && ((millis() - serial_timeout_start) < 1000));
  double serial_init_time = millis() - serial_timeout_start;
  Serial.printf("USB Serial started after %d ms\n", serial_init_time);

  //CAN initialization
  CAN_bus_main.begin();
  Serial.println("CAN Bus Main Started");
  // CAN_bus_sensor.begin();

  //RTC initialization
  setSyncProvider(getTeensy3Time);
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
  SPI.setSCK(LSM9DS1_SCK);
  pinMode(LED, OUTPUT);
  pinMode(VBATT, INPUT);
  pinMode(V3_3, INPUT);

}

int LED_timer = millis();
int LED_fliper = 1;

void loop(){
  if(millis() - LED_timer > 1000){
    digitalWrite(LED, LED_fliper);
    LED_fliper = !LED_fliper;
    LED_timer = millis();
  }

  //Check the Main bus for messages
  if(CAN_bus_main.available()){
    Serial.println("main_msg");
    CAN_bus_main.read(msg); //Read message into CAN_message_t buffer
    int a = sd.write_raw_data(msg);
    Serial.print("Logged ");
    Serial.print(a);
    Serial.println(" bytes");
    int b = xbee.write_raw_data(msg, Serial, sd.get_log_time());
    Serial.print("Sent ");
    Serial.print(b);
    Serial.println(" bytes");
    Serial.println();
    //sd.parse_message(msg); //send to SD manager for logging
    //data_parse_message(msg, &SD_Manager::writeData, sd);
    //TODO add xBee Parser
    //TODO add command Parser
    //TODO add transfer Parser
  }

  int cmd = xbee.check_for_message();
  if(cmd == LS){
    sd.list_files(&XBEE);
    XBEE.write("end\n");
  }else if(cmd == OPEN){
    int path_len = xbee.read_file_name();
    char path[path_len];
    // Serial.print("Path length: ");
    // Serial.println(path_len);
    // Serial.print("Pre Dumping File: ");
    for(int i = 0; i < path_len; i++){
      path[i] = xbee.file_name_buf[i];
      // Serial.println(path[i]);
    }
    path[path_len-1] = '\0';

    sd.open_file(path);
    int bytes_read = sd.dump_file(path, path_len, &xbee, Serial);
    Serial.print("bytes_sent from file: ");
    Serial.println(bytes_read);
  }

  //Check the Sensor bus for messages
  // if(CAN_bus_sensor.available()){
  //   Serial.println("sensor_msg");
  //   // CAN_bus_sensor.read(msg); //Read message into CAN_message_t buffer
  //   // Serial.println();
  //   // int a = sd.write_raw_data(msg, Serial);
  //   // int b = xbee.write_raw_data(msg, Serial);
  //   //sd.parse_message(msg); //send to SD manager for logging
  //   //data_parse_message(msg, &SD_Manager::writeData, sd);
  //   //TODO add xBee Parser
  //   //TODO add command Parser
  //   //TODO add transfer Parser
  // }
}


time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}
