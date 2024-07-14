//96440275
//Amir hossein hajian nezhad

ADC_MODE(ADC_VCC);
  
const byte LED_PIN = LED_BUILTIN;
const byte D3_PIN = 0;
const byte D6_PIN = 12;
const byte D5_PIN = 14;
bool flag5 = false;
bool flag6 = false;
bool flag3 = false;

void ICACHE_RAM_ATTR blink5() {
  flag5 = true;
}

void ICACHE_RAM_ATTR blink6() {
  flag6 = true;
}

void ICACHE_RAM_ATTR blink3() {
  flag3 = true;
}

void setup() {
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(D5_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D5_PIN), blink5, CHANGE);
  pinMode(D6_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D6_PIN), blink6, CHANGE);
  pinMode(D3_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D3_PIN), blink3, CHANGE);
}

void loop() {
  if(flag5 == true){
    for (int i = 0; i < 5; i++){
      digitalWrite(LED_PIN, LOW);
      delay(100);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
  }
  String input = "";
  while (Serial.available()) {
    input = Serial.readString();
    input.trim();
    if(input.substring(0) == "VCC"){
      Serial.print(ESP.getVcc());
      Serial.print('\n');
  }
  }
  flag5 = false;
  }

  if(flag6 == true){
    digitalWrite(LED_PIN, LOW);
    delay(300);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    for (int i = 0; i < 4; i++){
      digitalWrite(LED_PIN, LOW);
      delay(100);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
  }
  flag6 = false;
  }
  
  if(flag3 == true){
    for (int i = 0; i < 3; i++){
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
  }
    for (int i = 0; i < 2; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(300);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
    }
  flag3 = false;
  }
}
