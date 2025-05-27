const int in1 = 8;   // A1 pin
const int in2 = 9;   // A2 pin
const int enA = 10;  // ENA pin

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter ON or OFF to control the fan:");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // remove trailing newline or spaces

    if (command == "ON") {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(enA, 255);  // full speed
      Serial.println("Fan turned ON");
    } else if (command == "OFF") {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      analogWrite(enA, 0);  // stop motor
      Serial.println("Fan turned OFF");
    } else {
      Serial.println("Invalid command. Use ON or OFF.");
    }
  }
}