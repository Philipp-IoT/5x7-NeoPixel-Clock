#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "Arduino.h"
#include "Preferences.h"

#define SETTINGS_NS "led-clock"

#define SKEY_WIFI_CONF "WIFI_CONF"
#define SKEY_WIFI_SSID "WIFI_SSID"
#define SKEY_WIFI_PSK "WIFI_PSK"
#define SKEY_NTP_SERVER "NTP_SERVER"

class SettingsManager {
    private:
        
    public:
        SettingsManager();

        static SettingsManager& getInstance() {
            static SettingsManager _instance;
            return _instance;
        }

        void begin();

        void ResetFlash();

        bool getBool(const char* key);
        String getString(const char* key);

        bool setBool(const char* key, bool val);
        bool setString(const char* key, String val);

};

#endif