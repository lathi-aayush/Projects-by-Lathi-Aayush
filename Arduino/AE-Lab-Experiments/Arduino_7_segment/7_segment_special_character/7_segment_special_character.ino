int segmentPin[] = { 2, 3, 4, 5, 6, 7, 8 };

// a, b, c, d, e, f, g
// 0, 1, 2, 3, 4, 5, 6

void setup() {

  for (int i = 0; i <= 6; i++) {
    pinMode(segmentPin[i], OUTPUT);
  }
}

void loop() {

  for (int i = 0; i <= 6; i++) {
    digitalWrite(segmentPin[i], LOW);
  }

  // special character A
  for (int i = 0; i <= 6; i++) {
    if (i == 3) {
      digitalWrite(segmentPin[i], LOW);
    } else {
      digitalWrite(segmentPin[i], HIGH);
    }
  }

  delay(1000);
}
