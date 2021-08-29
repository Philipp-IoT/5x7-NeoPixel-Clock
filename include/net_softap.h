#ifndef __NET_SOFTAP_H
#define __NET_SOFTAP_H

#include "Arduino.h"

#define SOFTAP_SSID "5x7 LED Clock"
#define SOFTAP_PSK "12345678"

class NetSoftAP {
    private:
        bool dummy;
    public:
        NetSoftAP();
        void Start();
};

#endif