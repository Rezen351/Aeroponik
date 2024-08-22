#include <PinoutOutsealAtmega128.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>

ModbusMaster nodes[6];
const int sensorIDs[6] = {1, 2, 3, 4, 5, 6};

int trig[2];

int OUT_1, OUT_2, OUT_3, OUT_4;
int IN_1, IN_2;

uint16_t holdingRegisterOutput[2];
uint16_t holdingRegisterInput[2];
uint16_t holdingRegister[20];

StaticJsonDocument<512> jsonDoc; // Adjust the size according to your needs

void preTransmission() {
  digitalWrite(DM, HIGH);
}

void postTransmission() {
  digitalWrite(DM, LOW);
}

void setup() {
  pinMode(DM, OUTPUT);
  digitalWrite(DM, LOW);

  Serial.begin(9600);
  Serial1.begin(9600);

  for (int i = 0; i < 6; i++) {
    nodes[i].begin(sensorIDs[i], Serial1);
    nodes[i].preTransmission(preTransmission);
    nodes[i].postTransmission(postTransmission);
  }

  OUT_1 = OUT_2 = OUT_3 = OUT_4 = 0;
  IN_1 = IN_2 = 0;

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
}

void updateTriggers() {
  trig[0] = digitalRead(S1) == LOW;
  trig[1] = digitalRead(S2) == LOW;

  holdingRegisterInput[0] = trig[0];
  holdingRegisterInput[1] = trig[1];

  holdingRegister[2] = holdingRegisterInput[0];
  holdingRegister[3] = holdingRegisterInput[1];
}

void readSendDatasensor() {
  jsonDoc.clear(); // Clear previous data
  JsonArray dataArray = jsonDoc.to<JsonArray>(); // Create a new JSON array

  for (int i = 0; i < 6; i++) {
    uint8_t result;

    if (sensorIDs[i] == 3 || sensorIDs[i] == 6) {
      result = nodes[i].readInputRegisters(0x0001, sensorIDs[i]);
      if (result == nodes[i].ku8MBSuccess) {
        float sensorValue = float(nodes[i].getResponseBuffer(0x00) / 10.0F);
        if (sensorValue == 0.0) {
          dataArray.add(0); // If sensor value is 0, add 0 to the array
        } else {
          dataArray.add(sensorValue);
        }
      }
      result = nodes[i].readInputRegisters(0x0003, sensorIDs[i]);
      if (result == nodes[i].ku8MBSuccess) {
        float sensorValue = float(nodes[i].getResponseBuffer(0x00) / 10.0F);
        if (sensorValue == 0.0) {
          dataArray.add(0); // If sensor value is 0, add 0 to the array
        } else {
          dataArray.add(sensorValue);
        }
      }
      result = nodes[i].readInputRegisters(0x0008, sensorIDs[i]);
      if (result == nodes[i].ku8MBSuccess) {
        float sensorValue = float(nodes[i].getResponseBuffer(0x00) / 10.0F);
        if (sensorValue == 0.0) {
          dataArray.add(0); // If sensor value is 0, add 0 to the array
        } else {
          dataArray.add(sensorValue);
        }
      }
    } else {
      result = nodes[i].readInputRegisters(0x0000, sensorIDs[i]);
      if (result == nodes[i].ku8MBSuccess) {
        float tempValue = float(nodes[i].getResponseBuffer(1) / 10.0F);
        float humidValue = float(nodes[i].getResponseBuffer(0) / 10.0F);
        
        if (tempValue == 0.0) {
          dataArray.add(0); // If temperature value is 0, add 0 to the array
        } else {
          dataArray.add(tempValue);
        }
        
        if (humidValue == 0.0) {
          dataArray.add(0); // If humidity value is 0, add 0 to the array
        } else {
          dataArray.add(humidValue);
        }
      }
    }

    updateTriggers(); // Update triggers dynamically during sensor read
    delay(50); // Adjust delay time between requests to different sensors if needed
  
  }

  // Add trigger values to the dataArray
  dataArray.add(trig[0]);
  dataArray.add(trig[1]);

  serializeJson(dataArray, Serial);
  Serial.println(); // New line for better readability
}

void loop() {
  IN_1 = digitalRead(S1) == LOW;
  IN_2 = digitalRead(S2) == LOW;

  trig[0] = IN_1;
  trig[1] = IN_2;

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    switch (input.charAt(0)) {
      case 'P':
        if (input == "PUMP1_ON") {
          OUT_1 = 1;
        } else if (input == "PUMP1_OFF") {
          OUT_1 = 0;
        } else if (input == "PUMP2_ON") {
          OUT_2 = 1;
        } else if (input == "PUMP2_OFF") {
          OUT_2 = 0;
        } else {
          Serial.println("Unknown command: " + input);
        }
        break;
      case 'R':
        if (input == "READ_DATA") {
          for (int i = 0; i < 5; i++) {
            readSendDatasensor();
          }
        } else {
          Serial.println("Unknown command: " + input);
        }
        break;
      case 'A':
        if (input == "ALARM1_ON"){
          digitalWrite(R3, 1);
        } else if (input == "ALARM1_OFF"){
          digitalWrite(R3, 0);
        } else {
          Serial.println("Unknown command: " + input);
        }
        break;  
      default:
        Serial.println("Unknow Command: " + input);
        break;
      }
  }

  holdingRegisterOutput[0] = OUT_1;
  holdingRegisterOutput[1] = OUT_2;
  holdingRegisterInput[0] = IN_1;
  holdingRegisterInput[1] = IN_2;

  holdingRegister[0] = holdingRegisterOutput[0];
  holdingRegister[1] = holdingRegisterOutput[1];
  holdingRegister[2] = holdingRegisterInput[0];
  holdingRegister[3] = holdingRegisterInput[1];

  digitalWrite(R1, holdingRegister[0]);
  digitalWrite(R2, holdingRegister[1]);

  updateTriggers();

  delay(50); // Delay to approximate the original intervalModbus time (50ms)
}
