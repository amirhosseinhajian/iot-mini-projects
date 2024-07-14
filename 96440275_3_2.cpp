#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

IPAddress IP(192, 168, 43, 200);
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void handleRoot(){
  server.send(200, "text/plain", "You are coneected to access point.");
}

void handleNotFound(){
  server.send(404, "text/plain", "Bad request!");
}

bool connect2WiFi(String pass, int count){
  Serial.println("--------------------------------------------------------");
  String ssid = "IOT";
  String password = pass;
  WiFi.begin(ssid, password);
  String message = "Conecting with password" + String(count+1) + "...! Please wait 5 seccends.";
  Serial.println(message);
  Serial.println("Password" + String(count+1) + ": " + pass);
  int counter = 0;
  while(counter < 5){
    delay(1000);
    Serial.print("!");
    counter++;
  }
  Serial.println();
  if(WiFi.status() == WL_CONNECTED){
    message = "Connected after " + String(counter) + " secconds.";
    Serial.println(WiFi.localIP());
    Serial.println(message);
    Serial.println();
    return true;
  }
  else{
    Serial.println("Cannot connect to the IOT network.");
    Serial.println();
    return false;
  }

}

void manageAPI(){
  server.on("/", handleRoot);
  
  server.onNotFound(handleNotFound);

   server.on("/connect", [](){
    server.send(200, "text/plain", "To see the result, Please look at the Serial.");
    for(int i =0; i<4; i++){
      String s = "";
      for(int j=0; j<8; j++){
        char c = random(33, 127);
        s += c;
      }
      bool flag = connect2WiFi(s, i);
      if (flag == true){
        break;
      }
    }
  });

  server.on("/deepSleep", [](){
    String message = "I'm awake, but I'm going into deep sleep mode for 30 seconds.";
    Serial.println(message);
    server.send(200, "text/plain", message);
    delay(1000);
    ESP.deepSleep(30e6);
  });
}

void setup() {
  Serial.begin(9600);
  WiFi.softAPConfig(IP, gateway, subnet);
  WiFi.softAP("Amir2022");
  manageAPI();
  server.begin();

}

void loop() {
  server.handleClient();

}
