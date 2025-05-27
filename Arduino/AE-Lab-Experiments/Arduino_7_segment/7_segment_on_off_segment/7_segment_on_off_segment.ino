int segmentA = 2;
int buttonPin = 7;

void setup() {
  pinMode(segmentA, OUTPUT);
  pinMode(buttonPin, INPUT);  
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);

  if (buttonState == LOW) {
    digitalWrite(segmentA, LOW); 
  } else {
    digitalWrite(segmentA, HIGH);    
  }

  delay(50); 
}
