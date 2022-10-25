#include "MainApp.hpp"

/************ Main Application Pointer ************/
MainApp *app;
/************ Main Application Pointer ************/

LiquidCrystal lcd(_RS, _EN, _D4, _D5, _D6, _D7);

void setup() 
{
  app = new MainApp;
  lcd.begin(16, 4);
  Serial.begin(9600);
}

void loop()
{
  /************ Input Channels State Reading ************/
  uint16_t inputs = app->inputStateReading();

  /************ Input Channels (Checking Message Input & Displaying Respective Message On LCD) ************/
  app->displayMessage(inputs);

   /************ Input String Parsing ************/
  app->inputStringParsing();
  
  delay(50);
}

void serialEvent()
{
  app->serialEvent();
}
