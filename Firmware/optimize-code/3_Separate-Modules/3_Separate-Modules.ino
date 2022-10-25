#include "MainApp.hpp"

#define PERIOD         3000
#define PULSE_CHECK(x) x & ((0b1 << 1) | 0b1)
/************ Main Application Pointer ************/
MainApp *app;
/************ Main Application Pointer ************/

String inputString;

uint8_t serialInput = false;
uint8_t a = 0;

extern bool flag;
LiquidCrystal lcd(_RS, _EN, _D4, _D5, _D6, _D7);

uint8_t pulse_count(uint16_t message);

void setup() {

  app = new MainApp;

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 4);

  Serial.begin(9600);

  /************ Input Channels Setup ************/
  GPIOA->ODR &= (0xFF00);
  GPIOB->ODR &= (0xFFFC);

  pinMode(PA0, INPUT_PULLUP);
  /************ Input Channels Setup ************/

  inputString.reserve(69);

  app->readEEPROM();
}

void loop()
{

  /************ Input Channels State Reading ************/
  uint32_t gpioPORTA = GPIOA->IDR;
  uint32_t gpioPORTB = GPIOB->IDR;

  uint8_t inputA = (gpioPORTA & 0xFF);
  uint8_t inputB = (gpioPORTB & 0x03);

  //Serial.print("GPIOA :");
  //Serial.println(inputA, BIN);
  //Serial.print("GPIOB :");
  //Serial.println(inputB, BIN);

  //FC01 will be replace with FC00
  uint16_t inputs = ~( (inputA | ( inputB << 8)) | 0xFC00 );

  //Serial.print("Inputs State:");
  //Serial.println(inputs, BIN);
  /************ Input Channels State Reading ************/

  if (!inputs)
  {
    //Serial.println("No Input Detected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.show(0, 0, app->default_message.c_str());
    //Serial.println("Default message");
  }

  else
  {
    uint8_t i = 0;
    inputs >>   i & 0b1 ? pulse_count(inputs) :
           inputs >> ++i & 0b1 ? pulse_count(inputs) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, app->msgs[i].c_str()) : i;
  }

  /*    for (int i = 0; i < 10; i++)
      {
        if ((inputs >> i) & (0b1))
        {
          if(i == 0)
          {
            uint8_t var = app->specialCondition();
            if(var == 1)
            {
              Serial.println("correct");
            }
            else if(var == 2)
            {
              i = 1;
              Serial.println("un-correct");
            }
            else if(var == 0)
            {
              continue;
            }
            Serial.print("*****************  i = ");
            Serial.println(i);
          }
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.show(0, 0, app->msgs[i].c_str());
          break;
        }
      }*/

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
      app->msgs[msg_id_int] = message;

      EEPROM.begin();
      EEPROM.put(app->msg_addresses[msg_id_int], buffer_temp);
      EEPROM.end();

    }
    else
    {
      app->default_message = message;
      EEPROM.begin();
      EEPROM.put(app->msg_addresses[10], buffer_temp);
      EEPROM.end();
    }

    inputString = "";
    serialInput = false;
  }

  delay(50);
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

    else if (a == '!') {
      char temp[2] = {'$', '#'};
      Serial.write(temp, 2);
      Serial.println("");
    }
  }
}

uint8_t pulse_count(uint16_t message)
{
  uint8_t pulseCount = 0;
  bool prev_state = message;
  Serial.print("Prev: ");
  Serial.println(prev_state);

  unsigned long oldTime = millis();
  unsigned long Time = millis();

  do
  {
    uint8_t in = ~(GPIOA->IDR);
    bool cur_state = in  & 0x03;
    if (prev_state != cur_state)
    {
      prev_state = cur_state;
      if (cur_state)
      {
        pulseCount++;
        Serial.print("Count ");
        Serial.println(pulseCount);
      }
    }
    Time = millis();
  }
  while ((Time - oldTime) < PERIOD);

  if (pulseCount > 0)
  {
    Serial.println("Message02");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.show(0, 0, app->msgs[1].c_str());
    delay(5000);
  }

  else
  {
    Serial.println("Message01");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.show(0, 0, app->msgs[0].c_str());
    while (message);
  }
  pulseCount = 0;
  return 0;
}
