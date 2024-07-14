// Amirhossein Hajiannezhad
// 96440275

#include <EEPROM.h>

#define SIZE 255

byte frontAddress = 401;
byte rearAddress = 402;
byte front, rear;
byte magicIndex = 375;
byte magicNumber = 502;
const byte D6_PIN = 12;
const byte D7_PIN = 13;

class CircularQueue{
  public:
  void enQueue(byte value){
    if((front == 0 && rear == SIZE - 1) || (rear == front - 1)){
      Serial.println("Queue is full");
      return;
    }
    else if(front == 255){
      front = rear = 0;
      EEPROM.write(rear, value);
    }
    else if(rear == SIZE - 1 && front != 0){
      rear = 0;
      EEPROM.write(rear, value);
    }
    else {
      EEPROM.write(rear++, value);
    }
    EEPROM.write(frontAddress, front);
    EEPROM.write(rearAddress, rear);
    EEPROM.commit();
  }

  byte deQueue(){
    byte data = EEPROM.read(front);
    EEPROM.write(front, 255);
    if(front == rear){
      front = 255;
      rear = 255;
    }
    else if(front == SIZE - 1){
      front = 0;
    }
    else{
      front++;
    }
    EEPROM.write(frontAddress, front);
    EEPROM.write(rearAddress, rear);
    EEPROM.commit();
    return data;
  }
  
  bool isEmpty(){
    if(front == 255){
    Serial.println("Queue is empty");
    return false;
    }
    return true;
  }
};
CircularQueue cq;

void ICACHE_RAM_ATTR pop(){
  if(cq.isEmpty()){
    byte val = cq.deQueue();
    if(val%2 == 0){
      digitalWrite(D7_PIN, HIGH);
    }
    else{
      digitalWrite(D7_PIN, LOW);
    }
  }
}

void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);
  pinMode(D6_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D6_PIN), pop, CHANGE);
  if(EEPROM.read(magicIndex) != magicNumber){
    front = 255;
    rear = 255;
    EEPROM.write(frontAddress, 255);
    EEPROM.write(rearAddress, 255);
    EEPROM.write(magicIndex, magicNumber);
    EEPROM.commit();
  }
  else{
    front = EEPROM.read(frontAddress);
    rear = EEPROM.read(rearAddress);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();
    if(input.substring(0) == "OddOrEven"){
      cq.enQueue(analogRead(A0));
  }
  }
}
