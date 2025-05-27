int red[] = {5, 6, 7, 8, 9, 10};

const int d = 200;

void setup() {
  // put your setup code here, to run once:
  for (int pin = 5; pin <= 10; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 10; i >= 0; i--) {
    digitalWrite(red[i], HIGH);
    delay(d);
    digitalWrite(red[i], LOW);
    delay(d);
  }
  for (int i = 0; i <= 10; i++) {
    digitalWrite(red[i], HIGH);
    delay(d);
    digitalWrite(red[i], LOW);
    delay(d);
  }

}
