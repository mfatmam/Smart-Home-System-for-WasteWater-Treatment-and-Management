#include <LiquidCrystal.h>
#include <math.h>

#define B 3950             // Constante B de la thermistance
#define RESISTOR 10000     // Résistance fixe en série avec la thermistance, 10 kOhm
#define THERMISTOR 10000   // Résistance nominale de la thermistance à 25°C, 10 kOhm
#define NOMINAL 25         // Température nominale en °C

#define sensor A1          // Broche du capteur analogique

// Initialisation de l'écran LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
    Serial.begin(9600);  // Initialisation de la communication série
    pinMode(sensor, INPUT);  // Le capteur est une entrée

    // Initialisation de l'écran LCD 16x2
    lcd.begin(16, 2);
    lcd.print("Thermistor Temp");
}

void loop() {
    int t = analogRead(sensor);     // Lecture de la valeur analogique du capteur
    float tr = 1023.0 / t - 1;      // Calcul de la résistance de la thermistance
    tr = RESISTOR / tr;

    // Affichage de la résistance dans le moniteur série
    Serial.print("R = ");
    Serial.print(tr);
    Serial.print(" ohms, T = ");

    // Calcul de la température en utilisant la formule Steinhart-Hart
    float steinhart;
    steinhart = tr / THERMISTOR;              // R/Ro
    steinhart = log(steinhart);               // ln(R/Ro)
    steinhart /= B;                           // 1/B * ln(R/Ro)
    steinhart += 1.0 / (NOMINAL + 273.15);    // + (1/To)
    steinhart = 1.0 / steinhart;              // Inversion
    steinhart -= 273.15;                      // Convertir de K à °C

    // Affichage de la température dans le moniteur série
    Serial.println(steinhart);

    // Affichage sur l'écran LCD
    lcd.setCursor(0, 1);  // Déplacer le curseur à la deuxième ligne
    lcd.print("Temp: ");
    lcd.print(steinhart, 1);  // Afficher la température avec une décimale
    lcd.print(" C");

    delay(1000);  // Attendre 1 seconde avant la prochaine lecture
}
