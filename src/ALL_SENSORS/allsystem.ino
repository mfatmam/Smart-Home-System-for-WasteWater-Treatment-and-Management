#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// LCD initialization
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // RS, E, D4, D5, D6, D7

// Conductivity sensor and temperature sensor pins
#define ONE_WIRE_BUS 7          // Pin connected to DS18B20 data pin
#define CONDUCTIVITY_PIN A1      // Pin connected to voltage divider for conductivity

// Variables for pH, TDS, and Turbidity readings
int phBuffer[10], avgval;
float calibration_value = 0.00;
float tdsValue = 0, tdsVoltage;
int tdsBuffer[30], tdsBufferTemp[30];
int tdsBufferIndex = 0;
float turbidityVoltage = 0, turbidity = 0;
#define turbidityPin A2
#define TdsSensorPin A3
#define SCOUNT 30
#define VREF 5.0

// OneWire and DallasTemperature setup for temperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temperature = 25.0;  // Default temperature value

// --- Setup ---
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // LCD with 16 columns and 2 rows
  sensors.begin();   // Start temperature sensor

  delay(2000);
  lcd.clear();
}

// --- Main Loop ---
void loop() {
  // Reading values from sensors
  readPH();
  readTDS();
  readTurbidity();
  readTemperature();
  readConductivity();

  // Serial Monitor display
  Serial.print("pH: ");
  Serial.print(getPH(), 2);
  Serial.print(" | TDS: ");
  Serial.print(tdsValue, 0);
  Serial.print(" ppm | Turbidity: ");
  Serial.print(turbidity, 1);
  Serial.print(" NTU | Temp: ");
  Serial.print(temperature, 1);
  Serial.println(" °C");

  // LCD display
  lcd.setCursor(0, 0);
  lcd.print("pH: ");
  lcd.print(getPH(), 2);  // Display pH with 2 decimal places
  lcd.print(" TDS: ");
  lcd.print(tdsValue, 0);  // Display TDS with no decimal places

  lcd.setCursor(0, 1);
  lcd.print("Turb: ");
  lcd.print(turbidity, 1);  // Display turbidity with 1 decimal place
  lcd.print(" NTU");

  delay(1000);
}

// --- Reading pH ---
void readPH() {
  for (int i = 0; i < 10; i++) {
    phBuffer[i] = analogRead(A0);
    delay(30);
  }
  avgval = 0;
  for (int i = 2; i < 8; i++) {
    avgval += phBuffer[i];
  }
  avgval /= 6;
}

float getPH() {
  float voltage = avgval * 5.0 / 1024;
  return -5.70 * voltage + calibration_value;
}

// --- Reading TDS ---
void readTDS() {
  tdsBuffer[tdsBufferIndex] = analogRead(TdsSensorPin);
  tdsBufferIndex++;
  if (tdsBufferIndex == SCOUNT) {
    tdsBufferIndex = 0;
  }

  // Median filtering
  for (int i = 0; i < SCOUNT; i++) {
    tdsBufferTemp[i] = tdsBuffer[i];
  }
  tdsVoltage = getMedianNum(tdsBufferTemp, SCOUNT) * VREF / 1024.0;

  // Temperature compensation
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
  float compensationVoltage = tdsVoltage / compensationCoefficient;

  // TDS calculation
  tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
              - 255.86 * compensationVoltage * compensationVoltage
              + 857.39 * compensationVoltage) * 0.5;
}

int getMedianNum(int bArray[], int iFilterLen) {
  int bTemp[iFilterLen];
  for (int i = 0; i < iFilterLen; i++) {
    bTemp[i] = bArray[i];
  }
  for (int j = 0; j < iFilterLen - 1; j++) {
    for (int i = 0; i < iFilterLen - j - 1; i++) {
      if (bTemp[i] > bTemp[i + 1]) {
        int temp = bTemp[i];
        bTemp[i] = bTemp[i + 1];
        bTemp[i + 1] = temp;
      }
    }
  }
  return (iFilterLen & 1) ? bTemp[(iFilterLen - 1) / 2] : (bTemp[iFilterLen / 2] + bTemp[iFilterLen / 2 - 1]) / 2;
}

// --- Reading Turbidity ---
void readTurbidity() {
  int sensorValue = analogRead(turbidityPin);
  turbidityVoltage = sensorValue * (5.0 / 1023.0);
  turbidity = -133.42 * turbidityVoltage + 533.42;
  if (turbidity < 0) turbidity = 0;
}

// --- Reading Temperature ---
void readTemperature() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
}

// --- Reading Conductivity ---
void readConductivity() {
  int sensorValue = analogRead(CONDUCTIVITY_PIN);
  float conductivityVoltage = sensorValue * (5.0 / 1023.0);  // Convert to voltage

  Serial.print(" | Conductivity Voltage: ");
  Serial.print(conductivityVoltage);
  Serial.println(" V");

  lcd.setCursor(8, 1);
  lcd.print("Cond: ");
  lcd.print(conductivityVoltage, 2);  // Display conductivity voltage
}

