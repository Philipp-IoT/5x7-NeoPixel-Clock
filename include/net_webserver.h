#ifndef __NET_WEBSERVER_H
#define __NET_WEBSERVER_H

#include "Arduino.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "settings.h"

class NetWebserver {
    private:
        AsyncWebServer _server = AsyncWebServer(80);

        static SettingsManager& _settings;

        static String _getPOSTParam(AsyncWebServerRequest *request, String param);

    public:
        NetWebserver();
        
        static NetWebserver& getInstance() {
            static NetWebserver _instance;
            return _instance;
        }
        
        void begin();
        
        static void onDefaultRequest(AsyncWebServerRequest *request);
        static void onPicnicRequest(AsyncWebServerRequest *request);
        static void onStateRequest(AsyncWebServerRequest *request);
        static void onSubmitRequest(AsyncWebServerRequest *request);

        static String StringProcessor(const String& var);
};

#endif