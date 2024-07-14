// Amirhossein hajian neZhad
// 96440275

#include <EEPROM.h>

byte magicNumber = 178;
byte magicIndex = 294;
byte hourIndex = 10;
byte minuteIndex = 11;
byte hour;
byte minute;
byte seccond = 0;

void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);
  if(EEPROM.read(magicIndex) != magicNumber) {
    while(Serial.available() == 0){};
    String inputTime = Serial.readString();
    
    hour = inputTime.charAt(1) - '0';
    minute = inputTime.charAt(4) - '0';

    switch (inputTime.charAt(0)){
      case '1':
        hour += 10;
        break;
      case '2':
        hour += 20;
        break;
    }

    switch (inputTime.charAt(3)){
      case '1':
        minute += 10;
        break;
      case '2':
        minute += 20;
        break;
      case '3':
        minute += 30;
        break;
      case '4':
        minute += 40;
        break;
      case '5':
        minute += 50;
        break;
      case '6':
        minute += 60;
        break;
    }
    
    EEPROM.write(hourIndex, hour);
    EEPROM.write(minuteIndex, minute);
    EEPROM.write(magicIndex, magicNumber);
    EEPROM.commit();
  }
  else{
  hour = EEPROM.read(hourIndex);
  minute = EEPROM.read(minuteIndex);
  }
}

void loop() {
  if (hour < 10){
    Serial.print("0");
  }
  Serial.print(hour);
  Serial.print(":");
  if (minute < 10){
    Serial.print("0");
  }
  Serial.print(minute);
  Serial.print(":");
  if (seccond < 10){
    Serial.print("0");
  }
  Serial.println(seccond++);
  Serial.println("--------");
  delay(1000);
  if(seccond > 59){
    minute += 1;
    seccond = 0;
    if(minute > 59){
      hour += 1;
      minute = 0;
    }
    if(hour > 23){
      hour = 0;
    }
    EEPROM.write(hourIndex, hour);
    EEPROM.write(minuteIndex, minute);
    EEPROM.commit();
  }
  
}
