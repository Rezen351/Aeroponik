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

/*
// Jika memilih Opsi 1
//Menangani input nodered
void processCommand(const String& topic, const String& message, const String& device, const String& action) {
  if (topic == device) {
    if (message == "ON") {
      Serial.println(action + " ON");
    } else if (message == "OFF") {
      Serial.println(action + " OFF");
    }
  }
}
*/

// Fungsi untuk menangani pesan yang diterima dari MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  // Serial.print("Pesan diterima di topic: ");
  // Serial.println(topic);

  // Serial.print("Pesan: ");
  
  String message;

  String topicStr = String(topic);
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Serial.println(message);


//Opsi 1
//------------------SLAVE 1 -------------------------------------------
//POMPA1
 if (String(topic) == "slave1/pompa1") {
    if (message == "ON") {
      Serial.println("Jalankan apapun disini ON S1P1");
    } else if (message == "OFF") {
      Serial.println("Hentikan apapun disini OFF S1P1");
    }
  }
//POMPA2
 if (String(topic) == "slave1/pompa2") {
    if (message == "ON") {
      Serial.println("Jalankan apapun disini ON S1P2");
    } else if (message == "OFF") {
      Serial.println("Hentikan apapun disini OFF S1P2");
    }
  }
//ALARM
  if (String(topic) == "slave1/alarm") {
    if (message == "ALARM1_ON") {
      Serial.println("Jalankan apapun disini ON ALARM");
    } else if (message == "ALARM1_OFF") {
      Serial.println("Hentikan apapun disini OFF ALARM");
    }
  }

  //------------------SLAVE 2 -------------------------------------------



/*
  // Jika memilih Opsi 1
  // Slave 1
  processCommand(topicStr, message, "slave1/pompa1", "Pompa 1");
  processCommand(topicStr, message, "slave1/pompa2", "Pompa 2");
  processCommand(topicStr, message, "slave1/alarm1", "Alarm 1");
  processCommand(topicStr, message, "slave1/alarm2", "Alarm 2");

  // Slave 2
  processCommand(topicStr, message, "slave2/pompa1", "Pompa 1");
  processCommand(topicStr, message, "slave2/pompa2", "Pompa 2");
  processCommand(topicStr, message, "slave2/alarm1", "Alarm 1");
  processCommand(topicStr, message, "slave2/alarm2", "Alarm 2");
 */

/*
  //Jika memilih opsi 2
  // menerima data dari slave1
  if (String(topic) == "slave1/data") {
    Serial.print("Data dari slave 1");
    Serial.println(message);  // Tampilkan teks yang diterima dari Node-RED di Serial Monitor
  }
   // menerima data dari slave2
  if (String(topic) == "slave2/data") {
    Serial.print("Data dari slave 2 ");
    Serial.println(message);  // Tampilkan teks yang diterima dari Node-RED di Serial Monitor
  }

  // Jika menerima input teks dari Node-RED
  if (String(topic) == "nodered/input_text") {
    Serial.print("Teks diterima dari Node-RED: ");
    Serial.println(message);  // Tampilkan teks yang diterima dari Node-RED di Serial Monitor
  }

  */
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("Terhubung");

      // Subscribe ke topik yang diperlukan
      client.subscribe("slave1/pompa1");
      client.subscribe("slave1/pompa2");
      client.subscribe("slave1/alarm");
      client.subscribe("slave2/pompa1");
      client.subscribe("slave2/pompa2");
      client.subscribe("slave2/alarm");
      // client.subscribe("slave1/data");
      client.subscribe("nodered/input_text");  // Subscribe ke topik Node-RED
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
