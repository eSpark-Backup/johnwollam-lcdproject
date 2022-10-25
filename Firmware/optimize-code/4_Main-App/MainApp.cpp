#include "MainApp.hpp"

extern LiquidCrystal lcd;

void MainApp::inputSetup()
{
  /************ Input Channels Setup ************/
  GPIOA->ODR &= (0xFF00);
  GPIOB->ODR &= (0xFFFC);

  pinMode(PA0, INPUT_PULLUP);
  /************ Input Channels Setup ************/
}

uint16_t MainApp::inputStateReading()
{
  /************ Input Channels State Reading ************/
  uint32_t gpioPORTA = GPIOA->IDR;
  uint32_t gpioPORTB = GPIOB->IDR;

  uint8_t inputA = (gpioPORTA & 0xFF);
  uint8_t inputB = (gpioPORTB & 0x03);

  uint16_t inputs = ~( (inputA | ( inputB << 8)) | 0xFC00 );

  return inputs;
  /************ Input Channels State Reading ************/
}

void MainApp::inputStringParsing()
{
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
      messages[msg_id_int] = message;

      EEPROM.begin();
      EEPROM.put(msgAddresses[msg_id_int], buffer_temp);
      EEPROM.end();

    }
    else
    {
      defaultMessage = message;
      EEPROM.begin();
      EEPROM.put(msgAddresses[10], buffer_temp);
      EEPROM.end();
    }

    inputString = "";
    serialInput = false;
  }
}

void MainApp::serialEvent()
{
  if (Serial.available() > 0)
  {
    inChar = Serial.read();
    if (inChar == '$')
    {
      inputString = Serial.readStringUntil('%');
      serialInput = true;
    }

    else if (inChar == '!') 
    {
      char temp[2] = {'$', '#'};
      Serial.write(temp, 2);
      Serial.println("");
    }
  }
}

void MainApp::readEEPROM()
{
  char Buffer[69] = {};

  EEPROM.begin();

  // fetch button message
  for (int i = 0; i < 10; i++)
  {
    EEPROM.get(msgAddresses[i], Buffer);
    messages[i] = String(Buffer);

    memset(Buffer, 0, 69);
  }

  // fetch default messages
  EEPROM.get(ADDR_MSG_def, Buffer);
  defaultMessage = String(Buffer);
  memset(Buffer, 0, 69);

  EEPROM.end();
}
void MainApp::displayMessage(uint16_t inputs)
{
    /************ Input Channels (Checking Message Input & Displaying Respective Message On LCD) ************/
  if (!inputs)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.show(0, 0, defaultMessage.c_str());
  }

  else
  {
    uint8_t i = 0;
           inputs >>   i & 0b1 ? specialCondition(inputs) :
           inputs >> ++i & 0b1 ? specialCondition(inputs) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) :
           inputs >> ++i & 0b1 ? lcd.clear(), lcd.setCursor(0, 0), lcd.show(0, 0, messages[i].c_str()) : i;
  }
  /************ Input Channels (Checking Message Input & Displaying Respective Message On LCD) ************/
}
uint8_t MainApp::specialCondition(uint16_t message)
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
        cur_state = NULL;
      }
    }
    Time = millis();
  }
  while ((Time - oldTime) < PERIOD);

  if (pulseCount > 0)
  {
    Serial.println("Message-02");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.show(0, 0, messages[1].c_str());
    delay(2000);
  }

  else
  {
    Serial.println("Message-01");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.show(0, 0, messages[0].c_str());
    while( (~(GPIOA->IDR)) & 0x03 );
  }
  pulseCount = 0;
  return 0;
} 
