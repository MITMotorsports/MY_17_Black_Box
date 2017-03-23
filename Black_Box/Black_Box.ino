#include <TimeLib.h>
#include <FlexCAN.h>

#include "SD_Manager.h"
#include "IMU_Manager.h"

//SD Card Variables
SD_Manager sd;

//CAN Variables
FlexCAN CAN_bus_main(500000, 0);
FlexCAN CAN_bus_sensor(500000, 1);
static CAN_message_t msg;

//IMU Variables
IMU_Manager imu;

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
  Serial.println(hour(now()));
  sd.initialize(now(), Serial);
}

void loop(){
  Serial.println(hour(now()));

  //Check the Main bus for messages
  if(CAN_bus_main.available()){
    CAN_bus_main.read(msg); //Read message into CAN_message_t buffer
    //sd.parse_message(msg); //send to SD manager for logging
    data_parse_message(msg, &SD_Manager::writeData, sd);
    //TODO add xBee Parser
    //TODO add command Parser
    //TODO add transfer Parser
  }

  //Check the Sensor bus for messages
  if(CAN_bus_sensor.available()){
    CAN_bus_sensor.read(msg); //Read message into CAN_message_t buffer
    //sd.parse_message(msg); //send to SD manager for logging
    data_parse_message(msg, &SD_Manager::writeData, sd);
    //TODO add xBee Parser
    //TODO add command Parser
    //TODO add transfer Parser
  }
  //int did = sd.writeData(5);
  //Serial.println(did);
  delay(1000);
}


time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}
