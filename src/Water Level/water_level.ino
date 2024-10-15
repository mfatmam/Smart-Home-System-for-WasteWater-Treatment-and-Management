#include <LiquidCrystal.h>

// Sensor pins
#define sensorPower 7
#define sensorPin A0

// Value for storing water level
int val = 0;

// Initialize the library with the numbers of the interface pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Set D7 as an OUTPUT
  pinMode(sensorPower, OUTPUT);
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);
  
  // Initialize serial communication
  Serial.begin(9600);

  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);

  // Print a startup message on the LCD
  lcd.print("Water level:");
}

void loop() {
  // Get the reading from the function below and print it
  int level = readSensor();
  
  // Print the water level to the serial monitor
  Serial.print("Water level: ");
  Serial.println(level);
  
  // Display the water level on the LCD
  lcd.setCursor(0, 1); // Move cursor to the second line
  lcd.print("Level: ");
  lcd.print(level);
  lcd.print("    ");   // Add some spaces to clear out previous readings if shorter

  delay(1000);  // Wait for 1 second before the next reading
}

// This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);                        // Wait 10 milliseconds for stable reading
  val = analogRead(sensorPin);      // Read the analog value from sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;                       // Return the current reading
}
