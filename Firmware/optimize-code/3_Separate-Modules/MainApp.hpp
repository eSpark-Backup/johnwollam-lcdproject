#ifndef MAINAPP
#define MAINAPP

#include "Arduino.h"
#include "Headers.h"

class MainApp
{
  public:
  //Variables:
  int msg_addresses[11] = {ADDR_MSG1, ADDR_MSG2, ADDR_MSG3, ADDR_MSG4, ADDR_MSG5, ADDR_MSG6, ADDR_MSG7, ADDR_MSG8, ADDR_MSG9, ADDR_MSG10, ADDR_MSG_def};
  String default_message;
  String msgs[10];

  //Functions:
  void readEEPROM();
  uint8_t specialCondition();
};

#endif
