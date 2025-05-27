int irSensorPin = 2;  
int sensorValue = 0;

void setup() {
  pinMode(irSensorPin, INPUT);
  Serial.begin(9600);
  Serial.println("IR Sensor Initialized...");
}

void loop() {
  sensorValue = digitalRead(irSensorPin);

  if (sensorValue == LOW) {
    Serial.println("Obstacle Detected!");
  } else {
    Serial.println("No Obstacle.");
  }
  delay(500);  
}