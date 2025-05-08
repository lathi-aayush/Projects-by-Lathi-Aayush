#include <DHT.h>

#define DHTPIN 2        // DHT22 data pin
#define DHTTYPE DHT22   // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

const int in1 = 8;
const int in2 = 9;
const int enA = 10;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);

  // Set direction forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  Serial.println("Fan control based on temperature");
}

void loop() {
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  if (temp < 29) {
    analogWrite(enA, 0);
    Serial.println("Fan OFF");
  }
  else if (temp < 30) {
    analogWrite(enA, 85);  // LOW
    Serial.println("Fan LOW");
  }
  else if (temp < 31) {
    analogWrite(enA, 115); // MEDIUM
    Serial.println("Fan MEDIUM");
  }
  else {
    analogWrite(enA, 255); // HIGH
    Serial.println("Fan HIGH");
  }

  delay(1000);  // Wait 2 seconds before next reading
}