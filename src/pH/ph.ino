#include <Wire.h>
#include <LiquidCrystal.h>

float calibration_value = 21.34;  // Valeur de calibration
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10], temp;

// Initialisation de l'écran LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600); // Démarrer la communication série
  lcd.begin(16, 2);   // Initialiser l'écran LCD avec 16 colonnes et 2 lignes
  lcd.print("pH Meter Ready");  // Afficher un message de bienvenue
  delay(2000);
  lcd.clear();  // Effacer l'écran pour afficher les données de pH par la suite
}

void loop() {
  // Lire 10 fois la valeur du capteur et stocker les résultats dans le tableau
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(A2);
    delay(30);
  }

  // Trier les valeurs lues pour éliminer les anomalies
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }

  // Calculer la moyenne des valeurs lues, en ignorant les deux valeurs extrêmes
  avgval = 0;
  for (int i = 2; i < 8; i++) {
    avgval += buffer_arr[i];
  }

  // Calculer la tension et la valeur de pH
  float volt = (float)avgval * 5.0 / 1024 / 6;
  float ph_act = -5.70 * volt + calibration_value;

  // Afficher la valeur du pH dans le moniteur série
  Serial.print("pH Value: ");
  Serial.println(ph_act);

  // Afficher la valeur du pH sur l'écran LCD
  lcd.clear();
  lcd.setCursor(0, 0);  // Placer le curseur à la première ligne
  lcd.print("pH Value:");
  lcd.setCursor(0, 1);  // Placer le curseur à la deuxième ligne
  lcd.print(ph_act, 2);  // Afficher la valeur du pH avec deux décimales

  // Pause avant la prochaine lecture
  delay(1000);
}
#include <LiquidCrystal.h>

#define TdsSensorPin A0
#define VREF 5.0              // Référence de tension (Volt) de l'ADC
#define SCOUNT  30            // Nombre d'échantillons

int analogBuffer[SCOUNT];     // Stocke les valeurs analogiques dans un tableau
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float temperature = 16;       // Température actuelle pour la compensation

// Initialisation de l'écran LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Algorithme de filtrage médian
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}

void setup() {
  Serial.begin(115200);
  pinMode(TdsSensorPin, INPUT);
  
  // Initialisation de l'écran LCD
  lcd.begin(16, 2);
  lcd.print("TDS Meter Ready"); // Message d'accueil
  delay(2000);
  lcd.clear(); // Effacer l'écran avant de commencer l'affichage des mesures
}

void loop() {
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) {  // Tous les 40 ms, lire la valeur analogique
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);  // Lire et stocker la valeur analogique dans le buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT) {
      analogBufferIndex = 0;
    }
  }

  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {  // Mettre à jour l'affichage toutes les 800 ms
    printTimepoint = millis();

    // Appliquer le filtrage médian et calculer la moyenne des valeurs
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++) {
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    }
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;

    // Compensation de la température
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = averageVoltage / compensationCoefficient;

    // Conversion de la tension en valeur TDS
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
                - 255.86 * compensationVoltage * compensationVoltage
                + 857.39 * compensationVoltage) * 0.5;

    // Afficher les résultats sur le moniteur série
    Serial.print("TDS Value: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");

    // Afficher la valeur TDS sur l'écran LCD
    lcd.setCursor(0, 0);  // Première ligne
    lcd.print("TDS: ");
    lcd.print(tdsValue, 0);  // Afficher TDS avec 0 décimale
    lcd.print(" ppm   ");    // Ajoute des espaces pour effacer les anciennes valeurs plus longues

    lcd.setCursor(0, 1);  // Deuxième ligne
    lcd.print("Volt: ");
    lcd.print(averageVoltage, 2);  // Afficher la tension avec 2 décimales
    lcd.print(" V   ");  // Ajouter des espaces pour effacer les anciennes valeurs plus longues
  }
}
#include <LiquidCrystal.h>

// Pins configuration
int sensorPin = A0;  
int digitalPin = 8;  
int sensorValue = 0;  
int digitalValue = 0;  
float voltage = 0;
float turbidity = 0;

float previousTurbidity = -1;  
int previousDigitalValue = -1;  

// Initialize the LCD with the pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(digitalPin, INPUT);  
  Serial.begin(9600);  

  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
  lcd.print("Turbidity Meter");
  delay(2000);  // Let the user see the welcome message
  lcd.clear();  // Clear the welcome message before starting
}

void loop() {
  
  // Read analog sensor value
  sensorValue = analogRead(sensorPin);
  voltage = sensorValue * (5.0 / 1023.0);  
  turbidity = -1333.33 * voltage + 5333.33;

  // Ensure turbidity does not go below zero
  if (turbidity < 0) {
    turbidity = 0;
  }

  // Update and display the turbidity only if it changes
  if (turbidity != previousTurbidity) {
    // Serial output for debugging/monitoring
    Serial.print("Analog Voltage: ");
    Serial.print(voltage);
    Serial.print(" V, ");
    Serial.print("Turbidity: ");
    Serial.print(turbidity);
    Serial.println(" NTU");

    // Display on the LCD without clearing the entire screen
    lcd.setCursor(0, 0);  // First row
    lcd.print("Voltage: ");
    lcd.print(voltage, 2);  // Display voltage with 2 decimal places
    lcd.print(" V   ");  // Ensure no old data remains

    lcd.setCursor(0, 1);  // Second row
    lcd.print("Turbidity: ");
    lcd.print(turbidity, 1);  // Display turbidity with 1 decimal place
    lcd.print(" NTU   ");  // Ensure no old data remains

    previousTurbidity = turbidity;
  }

  // Read and display the digital pin value for threshold monitoring
  digitalValue = digitalRead(digitalPin);

  if (digitalValue != previousDigitalValue) {
    Serial.print("Digital Output: ");
    if (digitalValue == HIGH) {
      Serial.println("HIGH (Turbidity above threshold)");
    } else {
      Serial.println("LOW (Turbidity below threshold)");
    }

    previousDigitalValue = digitalValue;
  }

  delay(1000);  // Wait 1 second before next reading
}
