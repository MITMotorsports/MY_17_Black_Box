#include "SD_Manager.h"
#include <TimeLib.h>

SD_Manager sd;

void setup(){
  Serial.begin(9600); //start Serial port, baud rate defaults to full USB speed

  setSyncProvider(getTeensy3Time);
  while(!Serial);
  delay(100);
  if (timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }

  Serial.println(hour(now()));
  sd.initialize(now(), Serial);
}

void loop(){
  Serial.println(hour(now()));
  //int did = sd.writeData(5);
  //Serial.println(did);
  delay(1000);
}


time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}
