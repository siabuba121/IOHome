/*
 IOHome.h library that allows to control your house by arduino module's
*/
#ifndef IOHome_h
#define IOHome_h

#include "Arduino.h"
#include <ESP8266WebServer.h>

class IOHome {
    public:
        IOHome(int *usedPorts,int countPorts, char* ssid, char* password, ESP8266WebServer server);
    private:
        int *usedPorts;
        int countPorts;
        void connectToWifi(char* ssid, char* password);
        void setPorts(int usedPorts[],int count);
        void notifyOfConnectionStatus();
        void changePinStatusService();
};

#endif