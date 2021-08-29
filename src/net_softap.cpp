#include "net_softap.h"
#include "WiFi.h"

NetSoftAP::NetSoftAP() {

}

void NetSoftAP::Start() {
    WiFi.mode(WIFI_AP);

    WiFi.softAPConfig(
        IPAddress(192, 168, 128, 1),
        IPAddress(192, 168, 128, 1),
        IPAddress(255, 255, 255, 0)
    );

    WiFi.softAP(SOFTAP_SSID, SOFTAP_PSK);
}
