#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

///
#include <WiFi.h>
// #include "esp_wpa2.h"

// WiFi credentials
// const char* ssid = "eduroam";
const char* ssid = "Hotspot1";
const char* password = "12345678";
// #define EAP_ANONYMOUS_IDENTITY "anonymous@tuke.sk"
// #define EAP_IDENTITY "13322006@mahasiswa.itb.ac.id"
// #define EAP_PASSWORD ""
// #define EAP_USERNAME "13322006@mahasiswa.itb.ac.id"

void wifi_setup(){
Serial.print(F("Connecting to network: "));
    Serial.println(ssid);
    WiFi.disconnect(true);

    // Begin WiFi connection
    // WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(F("."));
    }
    Serial.println("");
    Serial.println(F("WiFi is connected!"));
    Serial.println(F("IP address set: "));
    Serial.println(WiFi.localIP());


}

#endif // WIFI_SETUP_H