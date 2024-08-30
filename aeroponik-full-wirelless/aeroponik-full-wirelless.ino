#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "input.h"

// int OUT_1, OUT_2, OUT_3, OUT_4;
// int IN_1, IN_2;

void setup() {
  Serial.begin(9600);
  wifi_setup();
  mqtt_setup();
  input_setup();
  
}

void loop() {
  mqtt_loop();

  //Input
  cek_serial();
  //
  read_data_sensor();
}