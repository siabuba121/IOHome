/*
 IOHome.h library that allows to control your house by arduino module's
*/

#include "Arduino.h"
#include "IOHome.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

IOHome::IOHome(){
}

void IOHome::init(int *usedPorts,int countPorts, const char* ssid, const char* password, LiquidCrystal_I2C lcd){
    this->countPorts = countPorts;
    this->usedPorts = usedPorts;
    this->lcd = &lcd;
    //this->server = server;
    Serial.begin(9600);
    this->setPorts(usedPorts,countPorts);
    this->connectToWifi(ssid, password);
    this->initHTTPServer(80);
    this->changePinStatusServiceInit();
}
void IOHome::connectToWifi(const char* ssid, const char* password){
    Serial.println("trying to connect with "+String(ssid)+" pass "+String(password));
    WiFi.begin(ssid, password);
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to "+String(ssid)+" IP:");
    Serial.print(WiFi.localIP());
    this->writeToScreen(WiFi.localIP().toString(),0);
    this->notifyOfConnectionStatus();
}

void IOHome::initScreen(){
    this->lcd->begin(16,2);
    this->lcd->setBacklightPin(3,POSITIVE);
    this->lcd->setBacklight(HIGH);
    this->lcd->home();
    this->lcd->print("TEST");
}

void IOHome::writeToScreen(String message, int line){
    LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);
    lcd.begin(16,2);
    lcd.setBacklightPin(3,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,line);
    lcd.print(message);
    //this->lcd->setCursor(0,line);
    //this->lcd->print(message);
    //Serial.println("pisanie na ekran");

    //delete &lcd;
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
      //OBSLUGA URZADZENIA
      //TRZEBA TO TROCHE INACZEJ ROZWIAZAC
      if(pin.toInt() == 16){
          if(status.toInt() == 1){
              this->writeToScreen("Bojler wlaczony",1);
          }else{
              this->writeToScreen("Bojler wylaczony",1);
          }
      }
    }
}

void IOHome::changePinStatusServiceInit(){
    Serial.println("poczatek");
    if(MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }
    //this bind...
    Serial.println("dziwne bindy");
    this->server.on("/changePort",std::bind(&IOHome::changePortStatus,this));
    this->server.on("/getPortStatus",std::bind(&IOHome::getPortStatus,this));
    this->server.begin();
    Serial.println("Serwer wystartował");
}

void IOHome::initHTTPServer(int port){
    Serial.println("proboje");
    delay(2000);
    ESP8266WebServer serwer(port);
    this->server = serwer;
    Serial.println("wystartowal");
}

void IOHome::getPortStatus(){
    String pin = this->server.arg("pin");
    int status = digitalRead(pin.toInt());
    if(pin!=""){
        this->server.send(200, "application/json;charset=utf-8","{'port':'"+String(status)+"'}");
    }else{
        this->server.send(500, "text/plain","Bledne argumenty!");
    }
}

void IOHome::handleClient(){
    this->server.handleClient();
}