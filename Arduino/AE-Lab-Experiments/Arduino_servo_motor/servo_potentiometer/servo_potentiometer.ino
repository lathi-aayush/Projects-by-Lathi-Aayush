#include <Servo.h>

int servoPin = 3;
int potPin = 0;

Servo servo;

void setup() {
  servo.attach(servoPin);

  Serial.begin(9600);
}

void loop() {

int reading = analogRead(potPin);
Serial.write(reading);

int angle = map(reading, 0, 1023, 0, 180);

servo.write(angle);

}
