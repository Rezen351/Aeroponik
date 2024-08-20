#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <WiFi.h>
#include <PubSubClient.h>


// MQTT Broker
const char* mqtt_server = "broker.mqtt-dashboard.com";
const int mqtt_port = 1883;

// Client untuk Ethernet dan MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Fungsi untuk menangani pesan yang diterima dari MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan diterima di topic: ");
  Serial.println(topic);
  Serial.print("Pesan: ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Jika menerima perintah dari Node-RED
  if (String(topic) == "nodered/control") {
    if (message == "ON") {
      Serial.println("Tombol ON ditekan");
    } else if (message == "OFF") {
      Serial.println("Tombol OFF ditekan");
    }
  }

  // Jika menerima input teks dari Node-RED
  if (String(topic) == "nodered/input_text") {
    Serial.print("Teks diterima dari Node-RED: ");
    Serial.println(message);  // Tampilkan teks yang diterima dari Node-RED di Serial Monitor
  }
}

// Koneksi ke MQTT Broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("Terhubung");
      client.subscribe("nodered/control");  // Subscribe ke topic Node-RED
      client.subscribe("nodered/input_text");
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void mqtt_setup(){
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
void mqtt_loop(){
  // Jika belum terhubung ke MQTT, coba sambungkan kembali
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Mengatur loop MQTT
}


#endif // MQTT_SETUP_H
