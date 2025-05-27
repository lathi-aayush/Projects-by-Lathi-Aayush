// METHOD 1 : SHORT

int segmentPins[] = {2, 3, 4, 5, 6, 7, 8}; // a, b, c, d, e, f, g

byte digits[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void displayDigit(int digit) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[digit][i]);
  }
}

void loop() {
  for (int i = 0; i < 10; i++) {
    displayDigit(i);
    delay(1000);
  }
}


// METHOD 2 : LENGTHY

// int segmentPin[] = { 2, 3, 4, 5, 6, 7, 8 };

// // 2, 3, 4, 5, 6, 7, 8
// // a, b, c, d, e, f, g
// // 0, 1, 2, 3, 4, 5, 6

// void setup() {
//   for (int i = 0; i < 7; i++) {
//     pinMode(segmentPin[i], OUTPUT);
//   }
// }

// void loop() {

//   number0();
//   delay(500);
//   alloff();

//   number1();
//   delay(500);
//   alloff();

//   number2();
//   delay(500);
//   alloff();

//   number3();
//   delay(500);
//   alloff();

//   number4();
//   delay(500);
//   alloff();

//   number5();
//   delay(500);
//   alloff();

//   number6();
//   delay(500);
//   alloff();

//   number7();
//   delay(500);
//   alloff();

//   number8();
//   delay(500);
//   alloff();

//   number9();
//   delay(500);
//   alloff();
// }

// void alloff() {
//   for (int i = 0; i <= 6; i++) {
//     digitalWrite(segmentPin[i], LOW);
//   }
// }

// void number0() {
//   // Number = 0
//   for (int i = 0; i <= 5; i++) {
//     digitalWrite(segmentPin[i], HIGH);
//   }
// }

// void number1() {
//   // b, c
//   // 1, 2
//   digitalWrite(segmentPin[1], HIGH);
//   digitalWrite(segmentPin[2], HIGH);
// }

// void number2() {
//   // a, b, g, e, d
//   // 0, 1, 3, 4, 6
//   digitalWrite(segmentPin[0], HIGH);
//   digitalWrite(segmentPin[1], HIGH);
//   digitalWrite(segmentPin[3], HIGH);
//   digitalWrite(segmentPin[4], HIGH);
//   digitalWrite(segmentPin[6], HIGH);
// }

// void number3() {
//   // a, b, g, c, d
//   // 0, 1, 3, 2, 6
//   digitalWrite(segmentPin[0], HIGH);
//   digitalWrite(segmentPin[1], HIGH);
//   digitalWrite(segmentPin[2], HIGH);
//   digitalWrite(segmentPin[3], HIGH);
//   digitalWrite(segmentPin[6], HIGH);
// }

// void number4() {
//   // f, g, b, c
//   // 5, 6, 1, 2
//   digitalWrite(segmentPin[5], HIGH);
//   digitalWrite(segmentPin[6], HIGH);
//   digitalWrite(segmentPin[1], HIGH);
//   digitalWrite(segmentPin[2], HIGH);
// }

// void number5() {
//   // a, f, g, c, d
//   // 0, 5, 6, 2, 3
//   digitalWrite(segmentPin[0], HIGH);
//   digitalWrite(segmentPin[5], HIGH);
//   digitalWrite(segmentPin[6], HIGH);
//   digitalWrite(segmentPin[2], HIGH);
//   digitalWrite(segmentPin[3], HIGH);
// }

// void number6() {
//   // a, f, g, c, d, e
//   // 0, 5, 6, 2, 3, 4
//   digitalWrite(segmentPin[0], HIGH);
//   digitalWrite(segmentPin[5], HIGH);
//   digitalWrite(segmentPin[6], HIGH);
//   digitalWrite(segmentPin[2], HIGH);
//   digitalWrite(segmentPin[3], HIGH);
//   digitalWrite(segmentPin[4], HIGH);
// }

// void number7() {
//   // a, b, c
//   // 0, 1, 2
//   digitalWrite(segmentPin[0], HIGH);
//   digitalWrite(segmentPin[1], HIGH);
//   digitalWrite(segmentPin[2], HIGH);
// }

// void number8() {
//   for (int i = 0; i <= 7; i++) {
//     digitalWrite(segmentPin[i], HIGH);
//   }
// }

// void number9() {
//   // all except e = 4
//   for (int i = 0; i <= 7; i++) {
//     if (i == 4) {
//       digitalWrite(segmentPin[i], LOW);
//     } 
//     else {
//       digitalWrite(segmentPin[i], HIGH);
//     }
//   }
// }
