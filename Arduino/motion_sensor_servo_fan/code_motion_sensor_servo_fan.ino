#include <Servo.h>

#define TRIG_PIN 3
#define ECHO_PIN 4
#define SERVO_PIN 5

Servo myServo;

int currentAngle = 90;     // Track current angle of the servo
bool servoActive = true;   // Manual ON/OFF toggle

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(currentAngle);  // Initialize to center

  Serial.println("Type 'on' or 'off' in Serial Monitor to control the servo.");
}

void loop() {
  // Check Serial input for manual control
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove whitespace

    if (command.equalsIgnoreCase("on")) {
      servoActive = true;
      Serial.println("Servo is now ON");
    } else if (command.equalsIgnoreCase("off")) {
      servoActive = false;
      Serial.println("Servo is now OFF");
    }
  }

  if (!servoActive) return;  // Exit early if OFF

  float distance = readDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  int targetAngle;

  if (distance < 60) {
    targetAngle = 30;  // Turn left
  } else if (distance > 60) {
    targetAngle = 150; // Turn right
  } else {
    targetAngle = 90;  // Return to center
  }

  slowServoMove(currentAngle, targetAngle);
  currentAngle = targetAngle;  // Update current angle

  delay(300);
}

// 📏 Read distance from ultrasonic sensor
float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;  // cm
}

// 🐢 Move servo gradually from one angle to another
void slowServoMove(int fromAngle, int toAngle) {
  int step = (fromAngle < toAngle) ? 1 : -1;

  for (int pos = fromAngle; pos != toAngle; pos += step) {
    myServo.write(pos);
    delay(15);  // Slow movement
  }

  myServo.write(toAngle);  // Final adjustment
}
