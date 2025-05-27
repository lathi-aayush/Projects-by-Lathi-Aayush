#include <Servo.h>

int servoPin = 3;
int angle = 0;

Servo servo;

void setup() {
  servo.attach(servoPin);
}

void loop() {
  for (angle = 0; angle < 180; angle++) {
    servo.write(angle);
    delay(15);
}
  for (angle = 180; angle > 0; angle--) {
    servo.write(angle);
    delay(15);
}
}