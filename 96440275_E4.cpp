#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <Ticker.h>

Ticker flipper;

ADC_MODE(ADC_VCC);

IPAddress IP(192, 168, 43, 200);
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);

String serverName = "https://192.168.150.1:2500";

ESP8266WebServer server(80);

String wifiSsid;
String wifiPassword;
byte magicIndex = 156;
byte magicNumber = 247;
byte ssidIndex = 10;
byte passwordIndex = 30;
bool flag = false;
int state = HIGH;

void send2Server(){
  float volt = ESP.getVcc();
  WiFiClient client;
  if (client.connect(serverName, 80)) {
    client.println(volt);
  }
  ESP.deepSleep(0);
}

void blinker(){
  state = !state;
  digitalWrite(LED_BUILTIN, state);
}

bool isConnected2WiFi(){
  return WiFi.status() == WL_CONNECTED;
}

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  int len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  EEPROM.commit();
}

String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1] = "";
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  return String(data);
}

void manageAPI(){
  server.on("/info", [](){
    server.send(200, "text/plain", "Succeed");
    String ssid = server.arg(0);
    String password = server.arg(1);
    writeStringToEEPROM(ssidIndex, ssid);
    writeStringToEEPROM(passwordIndex, password);
    wifiSsid = readStringFromEEPROM(ssidIndex);
    wifiPassword = readStringFromEEPROM(passwordIndex);
    Serial.println(wifiSsid);
    Serial.println(wifiPassword);
  });
  
  server.on("/start", [](){
    server.send(200, "text/plain", "Succeed");
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(wifiSsid, wifiPassword);
    String message = "conecting to WiFi..! Please wait...";
    Serial.println(message);
    delay(5000);
    if(isConnected2WiFi()){
      Serial.println("Connected!");
      Serial.println(WiFi.localIP());
      flipper.attach(5, send2Server);
    }
    else{
      Serial.println("Network connection failed.");
      flipper.attach(0.35, blinker);
    }
  });
}

void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  if(EEPROM.read(magicIndex) != magicNumber) {
    Serial.println("Get wifi information from browser.");
    WiFi.softAPConfig(IP, gateway, subnet);
    WiFi.softAP("amir2022");
    manageAPI();
    server.begin();
    EEPROM.write(magicIndex, magicNumber);
    EEPROM.commit();
    flag = true;
  }

}

void loop() {
  if(flag == true){
    server.handleClient();
  }

}
