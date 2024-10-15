# -*- coding: utf-8 -*-
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score
import pandas as pd

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
