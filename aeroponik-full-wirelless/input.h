#ifndef INPUT_H
#define INPUT_H
#include <ArduinoJson.h>

//inisiasi input
int IN_1, IN_2;

// Checking Input From Serial Monitor
void cek_serial(){
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();  // Hilangkan spasi dan newline
    if (input == "IN_1HIGH") {
      Serial.println("IN_1 : HIGH");
      IN_1 = 1;
    } else if (input == "IN_1OFF") {
      Serial.println("IN_1 : OFF");
      IN_1 = 0;
    }else if (input == "IN_2HIGH") {
      Serial.println("IN2 : HIGH");
      IN_2 = 1;
    }else if (input == "IN_2LOW") {
      Serial.println("IN2 : OFF");
      IN_1 = 0;
    }
    Serial.println(input);
  }
}

void input_setup(){
  IN_1 = IN_2 = 0;
  pinMode(32, INPUT);
  pinMode(33, INPUT);
}
// Receive Data From Sensor
// Sensor ID

void read_data_sensor() {
  // Buat objek JSON
  StaticJsonDocument<256> doc;

  // Array untuk menampung data random
  JsonArray dataArray = doc.to<JsonArray>();

  // Generate 12 data random dan tambahkan ke array
  for (int i = 0; i < 12; i++) {
    float randomValue = random(20, 100); // Generate data random (1.0 - 10.0)
    dataArray.add(randomValue); // Tambahkan data ke JSON array
  }

  // Data ke-13 dan ke-14 diisi secara manual
  dataArray.add(IN_1);  // Data manual ke-13 #lv1
  dataArray.add(IN_2);  // Data manual ke-14 #lv2

  // Serialize JSON ke string
  String jsonString;
  serializeJson(doc, jsonString);

  // Konversi String ke const char*
  const char* payload = jsonString.c_str();
  
  // Periksa koneksi dan publish data
  if (!client.connected()) {
    reconnect();
  }
  client.publish("slave2/data", payload);  // Publikasi payload ke topik

  // Print JSON string ke Serial
  // Serial.println(jsonString);
}



#endif 