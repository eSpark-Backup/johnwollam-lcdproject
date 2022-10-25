#include "LiquidCrystal.h"
#include "EEPROM.h"

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

#define ADDR_MSG1 0
#define ADDR_MSG2 70
#define ADDR_MSG3 140
#define ADDR_MSG4 210
#define ADDR_MSG5 280
#define ADDR_MSG6 350
#define ADDR_MSG7 420
#define ADDR_MSG8 490
#define ADDR_MSG9 560
#define ADDR_MSG10 630
#define ADDR_MSG_def 700

#define PULSE_DURATION 3000

String msg1 = "hello";
String msg2 = "Good morning";
String msg3 = "Good evening";
String msg4 = "Good afternoon";
String msg5 = "Good night";
String msg6 = "Good time";
String msg7 = "Good food";
String msg8 = "bye";
String msg9 = "bye bye";
String msg10 = "Good bye";

String default_message = "eSpark Consulting Group";

String msgs[] = {msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10};
int msg_addresses[] = {ADDR_MSG1, ADDR_MSG2, ADDR_MSG3, ADDR_MSG4, ADDR_MSG5, ADDR_MSG6, ADDR_MSG7, ADDR_MSG8, ADDR_MSG9, ADDR_MSG10, ADDR_MSG_def};

uint8_t stringComplete = false;
uint8_t button1DelayEnabled = false;
uint8_t test = false;
uint8_t pulse_count = 0;
uint8_t a = 0;

uint32_t button_triggered_time = millis();
uint32_t pulse_count_timer =  millis();

String inputString = "";

const int rs = PB3, en = PA15, d4 = PB15, d5 = PA8, d6 = PA9, d7 = PA10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 4);

  Serial.begin(9600);

  pinMode(MSG1, INPUT);
  pinMode(MSG2, INPUT);
  pinMode(MSG3, INPUT);
  pinMode(MSG4, INPUT);
  pinMode(MSG5, INPUT);
  pinMode(MSG6, INPUT);
  pinMode(MSG7, INPUT);
  pinMode(MSG8, INPUT);

  inputString.reserve(69);

  read_EEPROM();

  Serial.println("Started");


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

  // Priority encoding with buttons, excluding button1 and button2
  uint16_t inputs =  (input3 << 2) | (input4 << 3) | (input5 << 4) | (input6 << 5) | (input7 << 6) | (input8 << 7) | (input9 << 8) | (input10 << 9);

  // Read button 1 status, button2 is same as button1
  if (!digitalRead(MSG1))
  {

    // Count pulses
    pulse_count = count_pulses();

    if (pulse_count == 0)
    {
      //Serial.println("Msg 1");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.show(0, 0, msgs[0].c_str());
    }
    else
    {
      //Serial.println("Msg 2");

      Serial.print("Pulse count: ");Serial.println(pulse_count);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.show(0, 0, msgs[1].c_str());

      pulse_count = 0;
    }


    pulse_count_timer = millis();

    //    Serial.println("Pulses count");
    //    Serial.println(pulse_count);
  }

  for (int i = 0; i < 10; i++)
  {
    if ((inputs >> i) & (0b1))
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.show(0, 0, msgs[i].c_str());
      //      Serial.println("Message");
      //      Serial.println(i);


      break;
    }
  }

  // atleast wait a second    triggered
  if ((millis() - pulse_count_timer) > PULSE_DURATION)
  {
    if (!inputs)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.show(0, 0, default_message.c_str());
      //Serial.println("Default message");
    }
  }

  // Parse input string
  if (stringComplete)
  {
    //Serial.println(inputString);

    String msg_id = String(inputString.charAt(0));
    int dataLen = (String(inputString.charAt(1)) + String(inputString.charAt(2))).toInt();

    String message = inputString.substring(3, 3 + dataLen + 1);
    //Serial.println(message);

    //    Serial.println("Message id");
    //    Serial.println(msg_id);
    //
    //    Serial.println("Data length");
    //    Serial.println(dataLen);
    //
    //    Serial.println("Message");
    //    Serial.println(message);

    char buffer_temp[69];
    message.toCharArray(buffer_temp, 69);

    if (msg_id != "#")
    {
      int msg_id_int = msg_id.toInt();
      msgs[msg_id_int] = message;

      EEPROM.begin();
      EEPROM.put(msg_addresses[msg_id_int], buffer_temp);
      EEPROM.end();

    } else {
      default_message = message;

      EEPROM.begin();
      EEPROM.put(msg_addresses[10], buffer_temp);
      EEPROM.end();
    }

    inputString = "";
    stringComplete = false;
  }

  if (test) {
    lcd.show(0, 3, "Hello");
    a++;
    test = false;
  }


  //
  //  Serial.println(inputs);
  //
  delay(50);

}

uint8_t count_pulses()
{
  uint8_t local_pulse_count = 0;
  static uint8_t current_state = LOW;
  static uint8_t past_state = HIGH;

  button_triggered_time = millis();

  while ((millis() - button_triggered_time) < PULSE_DURATION)
  {

    current_state = digitalRead(MSG1);

    if (current_state == LOW)
    {
      local_pulse_count += past_state;
    }

    past_state = current_state;
  }


  return local_pulse_count;
}

void read_EEPROM()
{
  char Buffer[69] = {};

  EEPROM.begin();

  // fetch button message
  for (int i = 0; i < 10; i++)
  {
    EEPROM.get(msg_addresses[i], Buffer);
    msgs[i] = String(Buffer);
    //Serial.print("EEPROM message "); Serial.println(i + 1);
    //Serial.println(Buffer);

    memset(Buffer, 0, 69);
  }

  // fetch default messages
  EEPROM.get(ADDR_MSG_def, Buffer);
  default_message = String(Buffer);
  //Serial.println("EEPROM message default");
  //Serial.println(Buffer);
  memset(Buffer, 0, 69);

  EEPROM.end();
}

void serialEvent() {
  if (Serial.available() > 0) {
    char a = Serial.read();
    if (a == '$') {
      // get the new byte:
      inputString = Serial.readStringUntil('%');
      stringComplete = true;
    }

    else if (a == '!') {
      char temp[2] = {'$', '#'};
      Serial.write(temp, 2);
      Serial.println("");
      test = true;
    }
  }
}
