void setup() {
  Serial.begin(9600);
  delay(1000);

  // Header für SerialPlot – optional, aber hilfreich für Legende
  Serial.println("RPM Voltage Current");
}

void loop() {
  // Beispielwerte (ersetze sie durch deine Sensorwerte)
  int rpm = getRPM();                 // z.B. 0–1000
  float voltageMotor = getVoltage(); // z.B. 0–15 V
  float currentMotor = getCurrent(); // z.B. 0–1.2 A

  // Daten tab-getrennt ausgeben – so erkennt SerialPlot 3 Kurven
  Serial.print(rpm);
  Serial.print("\t");
  Serial.print(voltageMotor, 2);
  Serial.print("\t");
  Serial.println(currentMotor, 2);

  delay(100);  // 10 Hz Abtastrate
}

// Dummy-Funktionen: Ersetze diese durch deinen Sensor-Code!
int getRPM() {
  return random(0, 1000);
}

float getVoltage() {
  return random(0, 150) / 10.0;  // Simuliert 0.0 bis 15.0 V
}

float getCurrent() {
  return random(0, 120) / 100.0; // Simuliert 0.00 bis 1.20 A
}