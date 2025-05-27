// C++ code
//
void setup()
{  
  // Set all pins as OUTPUT
  for (int pin = 5; pin <= 13; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void loop()
{
 // Signal 1 
  //red light wait time
  for (int i = 0; i < 3; i++){
    digitalWrite(10, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(500);
  }
  //yellow light wait time
  for (int i = 0; i < 3; i++){
    digitalWrite(12, HIGH);
    delay(1000);
    digitalWrite(12, LOW);
    delay(500);
  }
  //green light wait time
  for (int i = 0; i < 5; i++){
    digitalWrite(11, HIGH);
    delay(1000);
    digitalWrite(11, LOW);
    delay(500);
  }
  
  // Signal 2 
  //red light wait time
  for (int i = 0; i < 3; i++){
    digitalWrite(13, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(10, HIGH);
    delay(1000);
    digitalWrite(10, LOW);
    delay(500);
  }
  //yellow light wait time
  for (int i = 0; i < 3; i++){
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(500);
  }
  //green light wait time
  for (int i = 0; i < 5; i++){
    digitalWrite(8, HIGH);
    delay(1000);
    digitalWrite(8, LOW);
    delay(500);
  }
  // Signal 3 
  //red light wait time
  for (int i = 0; i < 3; i++){
    digitalWrite(13, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(7, HIGH);
    delay(1000);
    digitalWrite(7, LOW);
    delay(500);
  }
  //yellow light wait time
  for (int i = 0; i < 3; i++){
    digitalWrite(6, HIGH);
    delay(1000);
    digitalWrite(6, LOW);
    delay(500);
  }
  //green light wait time
  for (int i = 0; i < 5; i++){
    digitalWrite(5, HIGH);
    delay(1000);
    digitalWrite(5, LOW);
    delay(500);
  }
  }