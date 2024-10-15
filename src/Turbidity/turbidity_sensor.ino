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
