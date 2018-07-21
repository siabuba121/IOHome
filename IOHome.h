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
        void initAsRoom(int *usedPorts,int countPorts, const char* ssid, const char* password, LiquidCrystal_I2C *lcd);
        
        //PLANT
        int signalMoisturePin;
        void initAsPlant(int signalMoisturePin,const char* ssid, const char* password);
        void waterPlant();
        void initPlantMoistureService();
        void getHumidity();
        //END PLANT
    private:
        String objectType;
        
        //PORTS
        int *usedPorts;
        int countPorts;
        void setPorts(int usedPorts[],int count);
        void changePinStatusServiceInit();
        void changePortStatus();
        void getPortStatus();
        //END PORTS
        
        //SCREEN
        LiquidCrystal_I2C *lcd;
        void initScreen();
        void writeToScreen(String message,int line);
        //END SCREEN

        //WIFI + SERVER
        void connectToWifi(const char* ssid, const char* password);
        void notifyOfConnectionStatus();
        void initHTTPServer(int port);
        ESP8266WebServer server;
        //END WIFI
};

#endif