#include "settings.h"
#include "nvs_flash.h"

Preferences _preferences;

SettingsManager::SettingsManager() {

    
}

void SettingsManager::begin() {
    if(_preferences.begin(SETTINGS_NS, false) == false) {
        /* Might need to add additional code to prevent further usage if returned false */
        Serial.printf("Error! Could not start preferences manager!\n");
    }
    Serial.printf("SM started!\n");
}

void SettingsManager::ResetFlash() {
    nvs_flash_erase();
    nvs_flash_init();
}

bool SettingsManager::getBool(const char* key) {
    /* return bool from NVS or false of error */
    return _preferences.getBool(key, false); 
}

String SettingsManager::getString(const char* key) {
   return _preferences.getString(key, "failed");
}

bool SettingsManager::setBool(const char* key, bool val) {
    return _preferences.putBool(key, val);
}

bool SettingsManager::setString(const char* key, String val) {
    Serial.printf("Setting %s to %s\n", key, val.c_str());
    int len = _preferences.putString(key, val.c_str());
    Serial.printf("Len %d \n", len);
    return true;
}
