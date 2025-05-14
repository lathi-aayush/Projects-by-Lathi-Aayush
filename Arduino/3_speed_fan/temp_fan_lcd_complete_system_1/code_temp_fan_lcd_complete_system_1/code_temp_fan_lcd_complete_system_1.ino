#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Servo.h>

// DHT22 settings
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LCD: 20x4 display at 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Motor control pins
const int in1 = 8;
const int in2 = 9;
const int enA = 10;
bool fanEnabled = true;  // Controlled via Serial

// Ultrasonic sensor and Servo
#define TRIG_PIN 3
#define ECHO_PIN 4
#define SERVO_PIN 5

Servo myServo;
int currentAngle = 90;
bool servoActive = true;

void setup() {
  Serial.begin(9600);

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting system...");

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  myServo.attach(SERVO_PIN);
  myServo.write(currentAngle);

  delay(2000);
  lcd.clear();
  Serial.println("Type 'fanon', 'fanoff', 'on', or 'off' to control.");
}

void loop() {
  // 🧠 Handle Serial input
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("fanon")) {
      fanEnabled = true;
      Serial.println("Fan control: ON");
    } else if (command.equalsIgnoreCase("fanoff")) {
      fanEnabled = false;
      Serial.println("Fan control: OFF");
    } else if (command.equalsIgnoreCase("on")) {
      servoActive = true;
      Serial.println("Servo is now ON");
    } else if (command.equalsIgnoreCase("off")) {
      servoActive = false;
      Serial.println("Servo is now OFF");
    }
  }

  // 🌡 DHT Reading
  float temp = dht.readTemperature();
  String fanSpeed = "OFF";

  if (isnan(temp)) {
    Serial.println("Sensor error. Check DHT.");
    lcd.setCursor(0, 0);
    lcd.print("Read Error: DHT");
    delay(1000);
    return;
  }

  if (fanEnabled) {
    if (temp < 30.7) {
      analogWrite(enA, 0);
      fanSpeed = "OFF";
    } else if (temp < 31) {
      analogWrite(enA, 85);
      fanSpeed = "LOW";
    } else if (temp < 31.5) {
      analogWrite(enA, 115);
      fanSpeed = "MEDIUM";
    } else {
      analogWrite(enA, 255);
      fanSpeed = "HIGH";
    }
  } else {
    analogWrite(enA, 0);
    fanSpeed = "MANUAL OFF";
  }

  // 🌬 Fan + Temp display
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" C | Fan: ");
  Serial.println(fanSpeed);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Fan: ");
  lcd.print(fanSpeed);

  // 🧭 Servo Logic (Distance Detection)
  if (servoActive) {
    float distance = readDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    int targetAngle = 90;
    if (distance < 60) {
      targetAngle = 30;
    } else if (distance > 60) {
      targetAngle = 150;
    }

    slowServoMove(currentAngle, targetAngle);
    currentAngle = targetAngle;
  }

  delay(1000);
}

// 📏 Ultrasonic distance
float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// 🔁 Smooth servo motion
void slowServoMove(int fromAngle, int toAngle) {
  int step = (fromAngle < toAngle) ? 1 : -1;
  for (int angle = fromAngle; angle != toAngle; angle += step) {
    myServo.write(angle);
    delay(10);
  }
  myServo.write(toAngle);  // Final position
}
