# -*- coding: utf-8 -*-
"""
Created on Tue Oct 15 17:41:05 2024

@author: fatma
"""

import serial
import json
import RPi.GPIO as GPIO
import time
from azure.iot.device import IoTHubDeviceClient, Message
import pandas as pd
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score

connection_string = "Smart_Home_System"
client = IoTHubDeviceClient.create_from_connection_string(connection_string)

GPIO.setmode(GPIO.BCM)
HOUSE_VALVE_PIN = 17
ONAS_VALVE_PIN = 27
GPIO.setup(HOUSE_VALVE_PIN, GPIO.OUT)
GPIO.setup(ONAS_VALVE_PIN, GPIO.OUT)

df = pd.read_excel("C:/Users/fatma/Downloads/archive/water_potability.xlsx")
df.dropna(inplace=True)

y = LabelEncoder().fit_transform(df['Potability'])
X = df.drop('Potability', axis=1)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

model = SVC(probability=True, random_state=42)
model.fit(X_train, y_train)

y_pred_train = model.predict(X_train)
accuracy_train = accuracy_score(y_train, y_pred_train)
y_pred = model.predict(X_test)
accuracy_test = accuracy_score(y_test, y_pred)
print(f'Accuracy (Train Set SVM): {accuracy_train * 100:.2f}%')
print(f'Accuracy (Test Set SVM): {accuracy_test * 100:.2f}%')

def control_valves(prediction):
    if prediction == 1:
        GPIO.output(HOUSE_VALVE_PIN, GPIO.HIGH)
        GPIO.output(ONAS_VALVE_PIN, GPIO.LOW)
        decision = "house"
    else:
        GPIO.output(HOUSE_VALVE_PIN, GPIO.LOW)
        GPIO.output(ONAS_VALVE_PIN, GPIO.HIGH)
        decision = "ONAS"
    return decision

def send_to_azure(turbidity, conductivity, pH, tds, water_level, decision):
    data = {
        'turbidity': turbidity,
        'conductivity': conductivity,
        'pH': pH,
        'tds': tds,
        'water_level': water_level,
        'decision': decision
    }
    message = Message(json.dumps(data))
    client.send_message(message)
    print("Data sent to Azure:", data)

def get_sensor_data():
    turbidity = 10.5
    conductivity = 450
    pH = 7.2
    tds = 300
    water_level = 80
    return turbidity, conductivity, pH, tds, water_level

try:
    while True:
        turbidity, conductivity, pH, tds, water_level = get_sensor_data()
        sensor_data = [[turbidity, conductivity, pH, tds, water_level]]
        sensor_data_scaled = scaler.transform(sensor_data)
        prediction = model.predict(sensor_data_scaled)[0]
        decision = control_valves(prediction)
        send_to_azure(turbidity, conductivity, pH, tds, water_level, decision)
        time.sleep(5)

except KeyboardInterrupt:
    print("Program stopped by user")

finally:
    GPIO.cleanup()
