#include <Servo.h>

#define TRIG_PIN 3
#define ECHO_PIN 4
#define SERVO_PIN 5

#define FAN_IN1 8
#define FAN_IN2 9
#define FAN_ENA 10

Servo myServo;

int currentAngle = 90;
bool servoActive = true;
bool fanOn = false;

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(FAN_IN1, OUTPUT);
  pinMode(FAN_IN2, OUTPUT);
  pinMode(FAN_ENA, OUTPUT);

  // Set fan direction
  digitalWrite(FAN_IN1, HIGH);
  digitalWrite(FAN_IN2, LOW);
  analogWrite(FAN_ENA, 0);  // Fan initially OFF

  myServo.attach(SERVO_PIN);
  myServo.write(currentAngle);  // Center position

  Serial.println("Type 'on' or 'off' for servo, 'fanon' or 'fanoff' to control fan.");
}

void loop() {
  // Handle serial commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("on")) {
      servoActive = true;
      Serial.println("Servo is now ON");
    } else if (command.equalsIgnoreCase("off")) {
      servoActive = false;
      Serial.println("Servo is now OFF");
    } else if (command.equalsIgnoreCase("fanon")) {
      analogWrite(FAN_ENA, 255); // Full speed
      fanOn = true;
      Serial.println("Fan is now ON");
    } else if (command.equalsIgnoreCase("fanoff")) {
      analogWrite(FAN_ENA, 0);
      fanOn = false;
      Serial.println("Fan is now OFF");
    }
  }

  if (!servoActive) return;

  float distance = readDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  int targetAngle;

  if (distance < 60) {
    targetAngle = 30;
  } else if (distance > 60) {
    targetAngle = 150;
  } else {
    targetAngle = 90;
  }

  slowServoMove(currentAngle, targetAngle);
  currentAngle = targetAngle;

  delay(300);
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void slowServoMove(int fromAngle, int toAngle) {
  int step = (fromAngle < toAngle) ? 1 : -1;

  for (int pos = fromAngle; pos != toAngle; pos += step) {
    myServo.write(pos);
    delay(15);
  }

  myServo.write(toAngle);
}
