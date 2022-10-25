#ifndef MAINAPP
#define MAINAPP

#include "Arduino.h"
#include "Headers.h"

class MainApp
{
  public:
  MainApp()
  {
    inputSetup();
    readEEPROM();
    inputString.reserve(69);
  }
  
  //Variables:
  int     msgAddresses[11] = {ADDR_MSG1, ADDR_MSG2, ADDR_MSG3, ADDR_MSG4, ADDR_MSG5, ADDR_MSG6, ADDR_MSG7, ADDR_MSG8, ADDR_MSG9, ADDR_MSG10, ADDR_MSG_def};
  bool    serialInput = false;
  char    inChar;
  String  defaultMessage;
  String  messages[10];
  String  inputString;

  //Functions:
  void      inputSetup();
  uint16_t  inputStateReading();
  void      inputStringParsing();
  void      serialEvent();
  void      readEEPROM();
  void      displayMessage(uint16_t inputs);
  uint8_t   specialCondition(uint16_t message);
  
};

#endif
