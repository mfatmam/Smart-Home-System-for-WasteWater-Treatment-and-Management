#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin definitions
#define ONE_WIRE_BUS 1 // Pin connected to DS18B20 Data pin
#define CONDUCTIVITY_PIN A0 // Pin connected to voltage divider

// Setup LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the I2C address if needed

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
 
  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);  // Fix: Pass the number of columns and rows to lcd.begin()
  lcd.backlight();   // Turn on the backlight
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
 
  // Read conductivity
  int sensorValue = analogRead(CONDUCTIVITY_PIN);
  float conductivityVoltage = sensorValue * (5.0 / 1023.0); // Convert to voltage

  // Display readings on LCD
  lcd.clear(); // Clear previous display
  lcd.setCursor(0, 0); // Set cursor to first row
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1); // Set cursor to second row
  lcd.print("Cond: ");
  lcd.print(conductivityVoltage);
  lcd.print(" V");

  // Display readings in Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Conductivity Voltage: ");
  Serial.println(conductivityVoltage);
 
  delay(1000); // Delay for readability
}
