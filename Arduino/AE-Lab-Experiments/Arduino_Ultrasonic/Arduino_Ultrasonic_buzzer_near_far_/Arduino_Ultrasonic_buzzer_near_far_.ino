int trigPin = 9;
int echoPin = 10;
int red = 4;
int buzzer = 2;

int threshold = 60;

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(red, OUTPUT);

  pinMode(buzzer, OUTPUT);
}

void loop() {
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(20);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.0343) / 2;

  digitalWrite(red, LOW);
  digitalWrite(buzzer, LOW);

  if (distance < threshold) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(red, HIGH);
  } else {
  }

  delay(1000);
}
