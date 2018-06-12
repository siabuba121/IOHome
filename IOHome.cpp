/*
 IOHome.h library that allows to control your house by arduino module's
*/

#include "Arduino.h"
#include "IOHome.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

IOHome::IOHome(int *usedPorts,int countPorts, char* ssid, char* password, ESP8266WebServer server){
    this->countPorts = countPorts;
    this->usedPorts = usedPorts;
    this->server = server;
    Serial.begin(9600);
    this->setPorts(usedPorts,countPorts);
    this->connectToWifi(ssid, password);
}

void IOHome::connectToWifi(char* ssid, char* password){
    WiFi.begin(ssid, password);
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to "+String(ssid)+" IP:");
    Serial.print(WiFi.localIP());
    this->notifyOfConnectionStatus();
}

void IOHome::setPorts(int usedPorts[],int count){
    for(int i = 0;i<count;i++){
        pinMode(usedPorts[i],OUTPUT);
        Serial.println(usedPorts[i]);

    }
}

void IOHome::notifyOfConnectionStatus(){
    pinMode(2,OUTPUT);
    digitalWrite(2,LOW);
}

void IOHome::changePortStatus(){
    String pin = this->server.arg("pin");
    String status = this->server.arg("status");
    if(pin=="" || status==""){
      this->server.send(500, "text/plain","Bledne argumenty!");
    }else{
      digitalWrite(pin.toInt(),status.toInt());
      this->server.send(200, "text/plain","Zmieniono status "+pin+" "+status);
    }
}

void IOHome::changePinStatusServiceInit(){
    if(MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }
    //this bind...
    this->server.on("/changePort",std::bind(&IOHome::changePortStatus,this));
    //server.on("/getPortStatus",getPortStatus);
    this->server.begin();
    Serial.println("Serwer wystartował");
}