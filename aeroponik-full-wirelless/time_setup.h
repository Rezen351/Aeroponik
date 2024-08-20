#ifndef TIME_SETUP_H
#define TIME_SETUP_H

#include "time.h"
#include "sntp.h"

// NTP servers and time zone settings
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600 * 6;

void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("No time available (yet)");
        return;
    }
    Serial.println(&timeinfo, "%A, %H:%M:%S");
}

void timeavailable(struct timeval *t) {
    Serial.println("Got time adjustment from NTP!");
    printLocalTime();
}

void setupTime() {
    sntp_set_time_sync_notification_cb(timeavailable);
    sntp_servermode_dhcp(1);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

#endif // TIME_SETUP_H
