/*
 IOHome.h library that allows to control your house by arduino module's
*/
#ifndef IOHome_h
#define IOHome_h

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class IOHome {
    public:
        IOHome();
        void handleClient();
        void init(int *usedPorts,int countPorts, const char* ssid, const char* password, LiquidCrystal_I2C *lcd);
    private:
        int *usedPorts;
        int countPorts;
        ESP8266WebServer server;
        LiquidCrystal_I2C *lcd;
        void initScreen();
        void writeToScreen(String message,int line);
        void connectToWifi(const char* ssid, const char* password);
        void setPorts(int usedPorts[],int count);
        void notifyOfConnectionStatus();
        void changePinStatusServiceInit();
        void changePortStatus();
        void initHTTPServer(int port);
        void getPortStatus();
};

#endif