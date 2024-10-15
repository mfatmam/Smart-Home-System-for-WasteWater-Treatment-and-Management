#include <Wire.h>

#define SLAVE_ADDRESS 0x04

float ph;
float turbidity;
float tds;
float conductivity;

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData);
  Serial.begin(9600);
}

void loop() {
  ph = getPhReading();
  turbidity = getTurbidityReading();
  tds = getTDSReading();
  conductivity = getConductivityReading();

  delay(1000);
}

void sendData() {
  String data = String(ph) + "," + String(turbidity) + "," + String(tds) + "," + String(conductivity);
  Wire.write(data.c_str());
}

float getPhReading() {
  return 7.0;
}

float getTurbidityReading() {
  return 12.5;
}

float getTDSReading() {
  return 300.0;
}

float getConductivityReading() {
  return 500.0;
}
