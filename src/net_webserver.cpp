#include "net_webserver.h"

//static NetWebserver& _inst = NetWebserver::getInstance();

SettingsManager& NetWebserver::_settings = SettingsManager::getInstance();

String getCurrentTimeStr() {
  static struct tm timeinfo;
  static char buffer[64];

  if(WiFi.isConnected() != WL_CONNECTED) {
    return "--.--.-- --:--:--";
  }
  
  if( !getLocalTime(&timeinfo) ) {
    return "Error";
  } else {
    strftime(buffer, 64, "%d.%m.%Y %H:%M:%S", &timeinfo);
    return String(buffer);
  }
}

NetWebserver::NetWebserver() {

}

void NetWebserver::begin() {

  if(!SPIFFS.begin(true)) {
    Serial.printf("Could not start SPIFFS!");
  }
  
  _server.on("/", HTTP_GET, onDefaultRequest);
  _server.on("/index.html", HTTP_GET, onDefaultRequest);

  _server.on("/picnic.min.css", HTTP_GET, onPicnicRequest);

  _server.on("/state.json", HTTP_GET, onStateRequest);

  _server.on("/submit", HTTP_POST, onSubmitRequest);

  _server.on("/test", HTTP_GET, [] (AsyncWebServerRequest *request) {

      request->send(200, "text/plain", "Hello world!");
  });

  _server.begin();

}

String NetWebserver::StringProcessor(const String& var) {
  Serial.printf("STR_PROC: %s\n", var.c_str());
  
  if(var == "TIME") {
    return getCurrentTimeStr();
  }

  if(var == "WIFI_SSID") {
    return _settings.getString(SKEY_WIFI_SSID);
  }

  if(var == "WIFI_PSK") {
    return _settings.getString(SKEY_WIFI_PSK);
  }

  if(var == "NTP_SERVER") {
    return _settings.getString(SKEY_NTP_SERVER);
  }

  return F("ERROR");
}

void NetWebserver::onDefaultRequest(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", String(), false, StringProcessor);
}

void NetWebserver::onPicnicRequest(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/picnic.min.css");
}

void NetWebserver::onStateRequest(AsyncWebServerRequest *request) {

  //Serial.printf("onStateRequest ENTER\n");
  
  AsyncJsonResponse * response = new AsyncJsonResponse();
  response->addHeader("Server","ESP Async Web Server");
  JsonVariant& root = response->getRoot();
  root["heap"] = ESP.getFreeHeap();
  root["ssid"] = WiFi.SSID();
  root["sys_time_str"] = getCurrentTimeStr();
  response->setLength();
  request->send(response);

}

String NetWebserver::_getPOSTParam(AsyncWebServerRequest *request, String param) {
    AsyncWebParameter *p = request->getParam(param, true);
    const String &str = p->value();
    Serial.printf("%s : %s\n", param.c_str(), str.c_str());
    return str;
}

void NetWebserver::onSubmitRequest(AsyncWebServerRequest *request) {

  String val;

  if(request->hasParam("wifi_ssid", true)) {
    val = _getPOSTParam(request, "wifi_ssid");
    _settings.setString(SKEY_WIFI_SSID, val);
    _settings.setBool(SKEY_WIFI_CONF, true);
  }

  if(request->hasParam("wifi_psk", true)) {
    val = _getPOSTParam(request, "wifi_psk");
    _settings.setString(SKEY_WIFI_PSK, val);
  }

  if(request->hasParam("ntp_server", true)) {
    val = _getPOSTParam(request, "ntp_server");
    _settings.setString(SKEY_NTP_SERVER, val);
  }

  request->redirect("/");
}
