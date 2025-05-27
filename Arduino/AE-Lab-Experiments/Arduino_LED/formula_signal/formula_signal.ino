void setup() {
  // put your setup code here, to run once:
  for (int pin = 5; pin <= 13; pin++) {
    pinMode(pin, OUTPUT);
  }
}

int PIN[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13 };
int red[] = { 11, 12, 13 };
int yellow[] = { 5, 6, 7 };
int green[] = { 8, 9, 10 };
const int d = 1000;
const int d2 = 300;

void loop() {
  // put your main code here, to run repeatedly:
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 9; i++) {
      digitalWrite(PIN[i], HIGH);
    }
    delay(d);
    for (int i = 0; i < 9; i++) {
      digitalWrite(PIN[i], LOW);
    }
    delay(d);
  }
  //only red blink
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(red[i], HIGH);
    }
    delay(d);
    for (int i = 0; i < 3; i++) {
      digitalWrite(red[i], LOW);
    }
    delay(d);
  }
    //only red and yellow blink faster
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(red[i], HIGH);
        digitalWrite(yellow[i], HIGH);
      }
      delay(d2);
      for (int i = 0; i < 9; i++) {
        digitalWrite(red[i], LOW);
        digitalWrite(yellow[i], LOW);
      }
      delay(d2);
    }

    // Green for go
    for (int i = 0; i < 3; i++) {
      digitalWrite(green[i], HIGH);
      digitalWrite(green[i], HIGH);
    }
    delay(5000);
  }
