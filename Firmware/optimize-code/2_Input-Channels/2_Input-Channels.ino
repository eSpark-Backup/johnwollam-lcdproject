 #include "LiquidCrystal.h"
#include "EEPROM.h"

#define MSG1 PA0

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

uint8_t serialInput = false;
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

  /************ Input Channels Setup ************/
  GPIOA->ODR &= (0xFF00);
  GPIOB->ODR &= (0xFFFC);
  /************ Input Channels Setup ************/
  
  inputString.reserve(69);

  read_EEPROM();

  Serial.println("Started");


}

void loop() {

  /************ Input Channels State Reading ************/
  uint32_t gpioPORTA = GPIOA->IDR;
  uint32_t gpioPORTB = GPIOB->IDR;
  
  uint8_t inputA = (gpioPORTA & 0xFF);
  uint8_t inputB = (gpioPORTB & 0x03);
  
  Serial.print("GPIOA :");
  Serial.println(inputA,BIN);
  Serial.print("GPIOB :");
  Serial.println(inputB,BIN);

  uint16_t inputs = ~( (inputA | ( inputB << 8)) | 0xFC01 );
  
  Serial.print("Inputs State:");
  Serial.println(inputs,BIN);
  /************ Input Channels State Reading ************/  

  for (int i = 1; i < 10; i++)
  {
    if ((inputs >> i) & (0b1))
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.show(0, 0, msgs[i].c_str());
      break;
    }
  }


  // atleast wait a second triggered
  if ((millis() - pulse_count_timer) > PULSE_DURATION)
  {
    Serial.println("Pulse Count Timer Triggered!");

    if (!inputs)
    {
      Serial.println("No Input Detected!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.show(0, 0, default_message.c_str());
      Serial.println("Default message");
    }
  }

  // Parse input string
  if (serialInput)
  {
    String msg_id = String(inputString.charAt(0));
    int dataLen = (String(inputString.charAt(1)) + String(inputString.charAt(2))).toInt();

    String message = inputString.substring(3, 3 + dataLen + 1);

    char buffer_temp[69];
    message.toCharArray(buffer_temp, 69);

    if (msg_id != "#")
    {
      int msg_id_int = msg_id.toInt();
      msgs[msg_id_int] = message;

      EEPROM.begin();
      EEPROM.put(msg_addresses[msg_id_int], buffer_temp);
      EEPROM.end();

    } 
    else 
    {
      default_message = message;
      EEPROM.begin();
      EEPROM.put(msg_addresses[10], buffer_temp);
      EEPROM.end();
    }

    inputString = "";
    serialInput = false;
  }

  if (test) 
  {
    lcd.show(0, 3, "Hello");
    a++;
    test = false;
  }
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

void serialEvent() 
{
  if (Serial.available() > 0) 
  {
    char a = Serial.read();
    if (a == '$') 
    {
      // get the new byte:
      inputString = Serial.readStringUntil('%');
      serialInput = true;
    }

    else if (a == '!') 
    {
      char temp[2] = {'$', '#'};
      Serial.write(temp, 2);
      Serial.println("");
      test = true;
    }
  }
}
