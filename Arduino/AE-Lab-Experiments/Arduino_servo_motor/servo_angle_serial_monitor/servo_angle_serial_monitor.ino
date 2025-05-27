#include <Servo.h>

int servoPin = 3;
int angle = 0;

Servo servo;

void setup() {

  servo.attach(servoPin);

  Serial.begin(9600);
  Serial.println("Enter value bt/wn 1 to 180");
}

void loop() {
  if (Serial.available() > 0) {
    angle = Serial.parseInt();
  }
  servo.write(angle);

  delay(1000);
}
