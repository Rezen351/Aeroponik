#ifndef INPUT_H
#define INPUT_H
#include <ArduinoJson.h>


// Json
StaticJsonDocument<512> jsonDoc; // Adjust the size according to your needs

// Checking Input From Serial Monitor
void cek_serial(){
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();  // Hilangkan spasi dan newline

    // Kirim input ke Node-RED melalui MQTT
    client.publish("nodered/output", input.c_str());
    Serial.print("Pesan dikirim ke Node-RED: ");
    Serial.println(input);
  }
}

// Receive Data From Sensor
// Sensor ID

// Sensor Array




void read_data_sensor(){
  // Dummy sensor
  int val_1 = random(10,30);
  int val_2 = random(60,100);

  int val1_1 = random(10,30);
  int val1_2 = random(60,100);

  int val2_1 = random(10,30);
  int val2_2 = random(60,100);

  Serial.println("read data sensor");
  jsonDoc.clear(); // Clear previous data
  JsonArray dataArray = jsonDoc.to<JsonArray>();
  dataArray.add(val_1);
  dataArray.add(val_2);
  dataArray.add(val1_1);
  dataArray.add(val1_2);
  dataArray.add(val2_1);
  dataArray.add(val2_2);
  char jsonBuffer[512];
  serializeJson(dataArray, jsonBuffer);

  if (!client.connected()) {
    reconnect();
  }
  client.publish("sensor/data", jsonBuffer);

  Serial.println(jsonBuffer);
}

#endif 