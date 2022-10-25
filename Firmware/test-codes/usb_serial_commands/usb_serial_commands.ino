#include "LiquidCrystal.h"

#define MSG1 PA0
#define MSG2 PA1
#define MSG3 PA2
#define MSG4 PA3
#define MSG5 PA4
#define MSG6 PA5
#define MSG7 PA6
#define MSG8 PA7
#define MSG9 PB0
#define MSG10 PB1

const String msg1 = "hello";
const String msg2 = "Good morning";
const String msg3 = "Good evening";
const String msg4 = "Good afternoon";
const String msg5 = "Good night";
const String msg6 = "Good time";
const String msg7 = "Good food";
const String msg8 = "bye";
const String msg9 = "bye bye";
const String msg10 = "Good bye";

String default_message = "eSpark Consulting Group";

String msgs[] = {msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10};

uint8_t stringComplete = false;
String inputString = "";

const int rs = PB3, en = PA15, d4 = PB15, d5 = PA8, d6 = PA9, d7 = PA10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 4);

  Serial.begin(115200);

  pinMode(MSG1, INPUT);
  pinMode(MSG2, INPUT);
  pinMode(MSG3, INPUT);
  pinMode(MSG4, INPUT);
  pinMode(MSG5, INPUT);
  pinMode(MSG6, INPUT);
  pinMode(MSG7, INPUT);
  pinMode(MSG8, INPUT);

  inputString.reserve(69);


}


void loop() {

  uint8_t input1 = !digitalRead(MSG1);
  uint8_t input2 = !digitalRead(MSG2);
  uint8_t input3 = !digitalRead(MSG3);
  uint8_t input4 = !digitalRead(MSG4);
  uint8_t input5 = !digitalRead(MSG5);
  uint8_t input6 = !digitalRead(MSG6);
  uint8_t input7 = !digitalRead(MSG7);
  uint8_t input8 = !digitalRead(MSG8);
  uint8_t input9 = !digitalRead(MSG9);
  uint8_t input10 = !digitalRead(MSG10);

  // Priority encoding with buttons
  uint16_t inputs = (input1) | (input2 << 1) | (input3 << 2) | (input4 << 3) | (input5 << 4) | (input6 << 5) | (input7 << 6) | (input8 << 7) | (input9 << 8) | (input10 << 9);
  
  for (int i = 0; i < 10; i++)
  {
    if ((inputs >> i) & (0b1))
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(msgs[i]);
      break;
    }
  }

  if (!inputs)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(default_message);
  }

  if (stringComplete)
  {
    Serial.println(inputString);

    String msg_id = String(inputString.charAt(0));
    int dataLen = (String(inputString.charAt(1)) + String(inputString.charAt(2))).toInt();

    String message = inputString.substring(3, 3 + dataLen + 1);

//    Serial.println("Message id");
//    Serial.println(msg_id);
//
//    Serial.println("Data length");
//    Serial.println(dataLen);
//
//    Serial.println("Message");
//    Serial.println(message);

    if(msg_id != "#")
    {
      msgs[msg_id.toInt()] = message; 
    }else{
      default_message = message;
    }

    inputString = "";
    stringComplete = false;
  }

  //
  //  Serial.println(inputs);
  //
  delay(50);


}

void serialEvent() {
  if (Serial.read() == '$')
  {
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();

      // if the incoming character is a %, set a flag so the main loop can
      // do something about it:
      if (inChar == '%') {
        stringComplete = true;
        break;
      }

      // add it to the inputString:
      inputString += inChar;
    }

    // Clear serial buffer
    while (Serial.available())
    {
      Serial.read();
    }
  }
}
