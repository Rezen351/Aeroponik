#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "input.h"
void setup() {
  Serial.begin(9600);
  wifi_setup();
  mqtt_setup();
  
}

void loop() {
  mqtt_loop();

  //Input
  cek_serial();
}