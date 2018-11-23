#include <string.h>
/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/
// подключите один мотор к клемме: M1+ и M1-
// а второй к клемме: M2+ и M2-
// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами 
// 4 и 7 — для направления, 5 и 6 — для скорости
#define SPEED_1      5  //правый мотор
#define DIR_1        4
 
#define SPEED_2      6   //левый мотор
#define DIR_2        7

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char buffer[200];
int value1, value2;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  // настраиваем выводы платы 4, 5, 6, 7 на вывод сигналов 
  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    int ind1 = inputString.indexOf(' ');
    value1 = inputString.substring(0, ind1).toInt();
    int ind2 = inputString.indexOf('\n');
    value2 = inputString.substring(ind1+1, ind2).toInt();
    if (value1 == 0 || value2 == 0){
      analogWrite(SPEED_1, 0);
      analogWrite(SPEED_2, 0);
      }
    // направление вперед
    if (value2 > 0 && value1 < 0 ){
      digitalWrite(DIR_1, LOW); //едет вперед
      digitalWrite(DIR_2, LOW);  //едет вперед
      analogWrite(SPEED_1, value2);
      analogWrite(SPEED_2, value1+255);
    } 
    if (value2 > 0 && value1 > 0 ){
      digitalWrite(DIR_1, LOW); //едет вперед
      digitalWrite(DIR_2, LOW);  //едет вперед
      analogWrite(SPEED_1, 255 - value1);
      analogWrite(SPEED_2, value2);
    }
    // направление назад
    if (value2 < 0 && value1 < 0 ){
      value2 = abs(value2);
      digitalWrite(DIR_1, HIGH); //едет вперед
      digitalWrite(DIR_2, HIGH);  //едет вперед
      analogWrite(SPEED_1, value2);
      analogWrite(SPEED_2, value1+255);
    } 
    if (value2 < 0 && value1 > 0 ){
      value2 = abs(value2);
      digitalWrite(DIR_1, HIGH); //едет вперед
      digitalWrite(DIR_2, HIGH);  //едет вперед
      analogWrite(SPEED_1, 255 - value1);
      analogWrite(SPEED_2, value2);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    int i = 0;
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  
}
