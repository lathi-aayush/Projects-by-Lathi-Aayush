#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === Pins ===
#define DHTPIN 2
#define DHTTYPE DHT22
#define TRIG_PIN 3
#define ECHO_PIN 4
#define SERVO_PIN 5

#define FAN_IN1 8
#define FAN_IN2 9
#define FAN_ENA 10

// === Objects ===
DHT dht(DHTPIN, DHTTYPE);
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 20, 4);

// === Variables ===
int currentAngle = 90;
bool servoActive = true;
bool fanAuto = true;
float triggerTemp = 31;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(FAN_IN1, OUTPUT);
  pinMode(FAN_IN2, OUTPUT);
  pinMode(FAN_ENA, OUTPUT);

  digitalWrite(FAN_IN1, HIGH);
  digitalWrite(FAN_IN2, LOW);
  analogWrite(FAN_ENA, 0);  // Fan OFF initially

  myServo.attach(SERVO_PIN);
  myServo.write(currentAngle);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("System Initializing");
  delay(1000);
  lcd.clear();

  Serial.println("Type: on/off (servo), fanon/fanoff (fan)");
}

void loop() {
  handleSerialCommands();

  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("DHT Error");
    return;
  }

  // === Fan control ===
  String fanStatus = "OFF";
  if (fanAuto) {
    if (temp >= triggerTemp) {
      analogWrite(FAN_ENA, 255);  // ON
      fanStatus = "ON";
    } else {
      analogWrite(FAN_ENA, 0);    // OFF
      fanStatus = "OFF";
    }
  } else {
    analogWrite(FAN_ENA, 0);      // Manual override: always off
    fanStatus = "MANUAL OFF";
  }

  // === Servo control ===
  float distance = readDistance();
  if (servoActive) {
    int targetAngle = 90;
    if (distance < 60) targetAngle = 75;
    else if (distance > 60) targetAngle = 180;

    slowServoMove(currentAngle, targetAngle);
    currentAngle = targetAngle;
  }

  // === LCD display ===
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Fan: ");
  lcd.print(fanStatus);

  lcd.setCursor(0, 2);
  lcd.print("Dist: ");
  lcd.print(distance, 1);
  lcd.print(" cm");

  lcd.setCursor(0, 3);
  lcd.print("Servo: ");
  lcd.print(servoActive ? "ON " : "OFF");

  // === Serial display ===
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C | Fan: ");
  Serial.print(fanStatus);
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);
}

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.equalsIgnoreCase("on")) {
      servoActive = true;
      Serial.println("Servo ON");
    } else if (cmd.equalsIgnoreCase("off")) {
      servoActive = false;
      Serial.println("Servo OFF");
    } else if (cmd.equalsIgnoreCase("fanon")) {
      fanAuto = true;
      Serial.println("Auto Fan Control ENABLED");
    } else if (cmd.equalsIgnoreCase("fanoff")) {
      fanAuto = false;
      Serial.println("Auto Fan Control DISABLED");
    }
  }
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
    delay(10);
  }
  myServo.write(toAngle);
}
