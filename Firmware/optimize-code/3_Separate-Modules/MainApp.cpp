#include "MainApp.hpp"

uint8_t state;
bool flag = true;
unsigned long prevMillis = 0;
unsigned long currentMillis; 
const long pulse = 450; 
#define pin PA0

void MainApp::readEEPROM()
{
  char Buffer[69] = {};

  EEPROM.begin();

  // fetch button message
  for (int i = 0; i < 10; i++)
  {
    EEPROM.get(this->msg_addresses[i], Buffer);
    this->msgs[i] = String(Buffer);

    memset(Buffer, 0, 69);
  }

  // fetch default messages
  EEPROM.get(ADDR_MSG_def, Buffer);
  this->default_message = String(Buffer);
  memset(Buffer, 0, 69);

  EEPROM.end();
}
uint8_t MainApp::specialCondition()
{
  uint8_t ret=0;
  if (flag == 1)
  {
  state = !digitalRead(pin);
  prevMillis = millis();
  }
  while ( state )
  {
    flag = false;
    currentMillis = millis();
    if (currentMillis - prevMillis >= pulse) 
     {
         if(digitalRead(pin))
         {
            Serial.println("Wrong Pass");
            ret=2;
         }
         else
         {
            Serial.println("Correct Pass");
            ret=1;
         }
         
         state=0;
     }
  }
  if ( (millis() - prevMillis) > 5800 )
  {
      flag = 1;
  }
    return ret;
  } 
