// Amir hossein hajian nezahd
// 96440275

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

String userName = "";
ESP8266WebServer server(80);
const char* ssid = "amirwifi";
const char* password = "hidden";

void add2File(String text){
  File file = SPIFFS.open("/database.txt", "a");
  if(!file){
    Serial.println("File not open for writing");
  }
  file.println(text);
  file.close();
}

String readFromFile(){
  File file = SPIFFS.open("/database.txt", "r");
  if(!file){
    Serial.println("File not exist for reading");
  }
  String data = file.readString();
  file.close();
  return data;
}

void createFile(){
  SPIFFSConfig cfg;
  cfg.setAutoFormat(false);
  SPIFFS.setConfig(cfg);
  SPIFFS.begin();
}

void handleRoot(){
  server.send(200, "text/plain", "Messanger is on.");
}

void handleNotFound(){
  server.send(404, "text/plain", "Bad request!");
}

bool isConnected2WiFi(){
  return WiFi.status() == WL_CONNECTED;
}

void connecting2WiFi(){
  String message = "conecting..! Please wait.";
  Serial.println(message);
  int counter = 0;
  while(!isConnected2WiFi()){
    delay(1000);
    Serial.print("!");
    counter++;
  }
  message = "Connected after " + String(counter) + " secconds.";
  Serial.println(WiFi.localIP());
  Serial.println(message);
}

void manageAPI(){
  server.on("/", handleRoot);
  
  server.onNotFound(handleNotFound);
  
  server.on("/username", [](){
    server.send(200, "text/plain", "Succeed");
    userName = server.arg(0);
  });
  
  server.on("/send", []() {
    if(userName != ""){
      server.send(200, "text/plain", "Succeed");
      String message = "";
      message += userName + ": " + server.arg(0);
      Serial.println(message);
      add2File(message);
    }
  });

  server.on("/retreive", [](){
    String content = readFromFile();
    server.send(200, "text/plain", content);
  });

  server.on("/cleardata", [](){
    SPIFFS.remove("/database.txt");
    server.send(200, "text/plain", "Succeed");
  });
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  createFile();
  connecting2WiFi();
  manageAPI();
  server.begin();

}

void loop() {
  server.handleClient();
}
