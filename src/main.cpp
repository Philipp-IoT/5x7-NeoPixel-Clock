//#define TEMPLATE_PLACEHOLDER "~"

#include <Arduino.h>

#include <WiFi.h>
#include <DNSServer.h>

#include "driver_led.h"
#include "settings.h"
#include "net_softap.h"
#include "net_webserver.h"

DNSServer dnsServer;

const char *ssid = "Hamm-Net";
const char *psk = "59494Soest";

DriverLED drvLED = DriverLED(2);
SettingsManager settings = SettingsManager();
//NetSoftAP softAP = NetSoftAP();
NetWebserver websrv = NetWebserver();                      

uint8_t chars[5];

bool timeSet = false;

bool soft_ap_used = false;

uint32_t webserver_start_countdown = 5000;

void setup() {
  
  Serial.begin(115200);

  Serial.printf("Booting...\n");

  /*if(settings.getWifiConfigured() == false) {
    Serial.printf("WiFi not configured... Starting Soft AP... \n");

    softAP.Start();
    delay(1000);
    server.begin();
    return;
    while(1) {
      delay(1000);
      Serial.printf("...\n");
    }

  } */


  settings.begin();

  bool is_con_ap = false;

  if(settings.getBool(SKEY_WIFI_CONF) == true) {
    WiFi.begin(ssid, psk);

    wl_status_t wlan_st = WL_IDLE_STATUS;

    while(wlan_st != WL_CONNECTED) {
      wlan_st = WiFi.status();

      if(wlan_st == WL_CONNECT_FAILED) {
        Serial.println("Could not connect!");
        break;
      }

      delay(100);
    }

    if(wlan_st == WL_CONNECTED) {
      is_con_ap = true;
      Serial.printf("Connected to AP: %s!\n", ssid);
      configTime(3600, 3600, "pool.ntp.org");
    }

  }

  if(is_con_ap != true) {
    Serial.printf("Not connected to AP! Starting soft AP with SSID %s\n", "5x7 Clock 012345");

    WiFi.disconnect();
    //delay(2000); /* experimentally determided - may be improved if required */

    bool soft_ap_st;

    WiFi.softAPConfig(IPAddress(192, 168, 5, 1), IPAddress(192, 168, 5, 1), IPAddress(255, 255, 255, 0));
    soft_ap_st = WiFi.softAP("5x7 Clock 012345", "12345678");

    if(soft_ap_st == false) {
      Serial.printf("Could not start soft AP!");
    } else {
      dnsServer.start(53, "*", WiFi.softAPIP());
      Serial.printf("IP Addr: %s\n", WiFi.softAPIP().toString().c_str());
      soft_ap_used = true;
    }
  }

  Serial.printf("Starting webserver... ");
  websrv.begin();
  Serial.printf("done!\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  static struct tm timeinfo;
  static char buffer[64];
  wl_status_t w_st;

  //Serial.printf("IP-Addr: %s\n", WiFi.localIP().toString().c_str());

  w_st = WiFi.status();

  if((w_st != WL_CONNECTED) || (soft_ap_used == true)) {

    chars[0] = 0;
    chars[1] = 0;
    chars[2] = 0;
    chars[3] = 0;
    chars[4] = 0;

  } else {

    if( !getLocalTime(&timeinfo, 100) ) {
      Serial.printf("Could not get time!\n");
    } else {
      strftime(buffer, 64, "%d.%m.%Y %H:%M:%S", &timeinfo);
      Serial.printf("Time: %s\n", buffer);
    }

    chars[0] = timeinfo.tm_hour / 10;
    chars[1] = timeinfo.tm_hour % 10;
    chars[2] = (timeinfo.tm_sec % 2 == 0) ? 10 : 11;
    chars[3] = timeinfo.tm_min / 10;
    chars[4] = timeinfo.tm_min % 10;

    
  }
  
  delay(50);

  drvLED.SetChars(chars);

  drvLED.UpdateDisplay();

  if(soft_ap_used == true) {
      dnsServer.processNextRequest();
  }

}