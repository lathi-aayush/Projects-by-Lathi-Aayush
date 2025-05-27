int PIN[] = {13, 12, 11};  // This is an array of 3 pins

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(PIN[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN[i], HIGH);
    delay(1000);
    digitalWrite(PIN[i], LOW);
    delay(1000);
  }
}
