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