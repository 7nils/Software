//Definitionen Variablen Bauteilabh.
const int impulsePerRotation = 12;    // Encoderabhängig NUR SPITZEN!
const float gearRatio = 9.7;   // oder 20.4 bei 380UpM
const float offsetCurrent = 2.5;  // Offset Stromsensor
const float sensitivityCurrent = 0.4; // Empfindlichkeit 400mV/A

// Definition der Pins
const int pwmPin = 5;        // PWM 
const int potPin = A0;       // Poti
const int encoderPinA = 2;   // Encoder A
const int encoderPinB = 3;   // Encoder B
const int forwardPin = 9;  // In1
const int reversePin = 10;  // In2
const int signalMotorIn = A2;  
const int signalMotorOut = A1;
const int currentSensor = A3;  // Strommessung
const int R7 = 30;  // Widerstand Spannungsteiler 30kOhm
const int R9 = 30;  // Widerstand Spannungsteiler 30kOhm
const int R8 = 10;  // Widerstand Spannungsteiler 10kOhm
const int R10 = 10;  // Widerstand Spannungsteiler 10kOhm


// Variablen
float supplyVoltage = 5.0;    // Versorgungsspannung Stromsensor
int potValue = 0;            // 0 bis 1023
int pwmValue = 0;            // 0 bis 255
int rpm = 0;                 // Drehzahl in UpM
float voltageMotorIn = 0;     
float voltageMotorOut = 0;  
float voltageMotor = 0;
float currentMotor = 0;
float rawValue= 0;      // Rohwert Stromsensor
float currentSensorVoltage = 0;   // Spannung am Stromsensor



volatile int impulseCount = 0; // An Encoder A
volatile int direction = 0;    // 1 = Vorwärts, -1 = Rückwärts, 0 = Keine Drehung
unsigned long lastTime = 0;   // Zeitpunkt der letzten Drehzahlberechnung

void setup() {
  // Initialisierung
  Serial.begin(9600);         
  pinMode(pwmPin, OUTPUT);    
  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT);
  pinMode(forwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);
  pinMode(signalMotorIn, INPUT);
  pinMode(signalMotorOut, OUTPUT);
  pinMode(currentSensor, INPUT);
  

  // Interrupt für Encoder A
  attachInterrupt(digitalPinToInterrupt(encoderPinA), countEncoderA, RISING);
  
}

void loop() {
  // Potiwert lesen und PWM berechnen
  potValue = analogRead(potPin);         
  pwmValue = map(potValue, 0, 1023, 0, 255);

  // Motorspannung berechnen
  calculateVoltageMotor();

  // Motorstrom berechnen
  calculateCurrentMotor();
  

  // Motordrehzahl per PWM
  analogWrite(pwmPin, pwmValue);  // 255 = 12V
  digitalWrite(forwardPin, HIGH);  // In1
  digitalWrite(reversePin, LOW);  //  In2

  // Berechnung der Drehzahl und Spannung  3mal/sek
  if (millis() - lastTime >= 1000) {
    rpm = calculateRPM();  // Drehzahl berechnen

  
    //current = calculateCurrent(); // Strom berechnen
    
    lastTime = millis();   
    impulseCount = 0;      // Zähler resetten
    
    
    // "Plotten" Werte
   Serial.print("   | RPM: ");
   Serial.print(rpm);
   Serial.print("   | Direction: ");
   Serial.print(direction == 1 ? "Forward" : (direction == -1 ? "Reverse" : "Stopped"));
   Serial.print("   | Voltage: ");
   Serial.print(voltageMotor, 2);  // Zwei Nachkommastellen
   Serial.print(" V");
   Serial.print("   | Current: ");
   Serial.print(currentMotor, 2);  // Zwei Nachkommastellen
   Serial.println(" A");
  }
}

// ISR an Encoder A
void countEncoderA() {
  
  if (digitalRead(encoderPinB) == LOW) {
  
    impulseCount++; // Vorwärts
    direction = 1;
    
  } 
  else {
    impulseCount--;  // Rückwärts
    direction = -1;
  }
}

// Funktion zur Berechnung der Drehzahl in UpM
int calculateRPM() {
  int rpm = ((impulseCount*60)/impulsePerRotation/gearRatio); // Umdrehungen pro Minute
  return rpm;
}


//Funktion Berechnung Spannungsteiler
float calculateVoltageMotor(){
//MotorIn
analogRead(signalMotorIn);
voltageMotorIn = signalMotorIn / (R8/(R7+R8));
//MotorOut
analogRead(signalMotorOut);
voltageMotorOut = signalMotorOut / (R10/(R9+R10));
//Differenz
voltageMotor = voltageMotorIn - voltageMotorOut;
return voltageMotor;
}

// Funktion Berechnung Strom
float calculateCurrentMotor(){

rawValue = analogRead(currentSensor);
currentSensorVoltage = rawValue * (supplyVoltage / 1023.0);  // Signalspannung Berechnung
currentMotor = (currentSensorVoltage - offsetCurrent) / sensitivityCurrent;  // Motorstromberechnung

return currentMotor;
}

