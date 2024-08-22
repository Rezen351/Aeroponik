#ifndef INPUT_H
#define INPUT_H

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



#endif 