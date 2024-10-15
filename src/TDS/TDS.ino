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
