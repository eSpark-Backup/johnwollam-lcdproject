#include "LiquidCrystal.h"
#include "EEPROM.h"

/************ EEPROM Addresses ************/
#define ADDR_MSG1     0
#define ADDR_MSG2     70
#define ADDR_MSG3     140
#define ADDR_MSG4     210
#define ADDR_MSG5     280
#define ADDR_MSG6     350
#define ADDR_MSG7     420
#define ADDR_MSG8     490
#define ADDR_MSG9     560
#define ADDR_MSG10    630
#define ADDR_MSG_def  700
/************ EEPROM Addresses ************/

/************ Pulse************/
#define PULSE_DURATION 3000
/************ Pulse************/

/************ LCD Pinouts ************/
#define _RS PB3
#define _EN PA15
#define _D4 PB15
#define _D5 PA8
#define _D6 PA9
#define _D7 PA10
/************ LCD Pinouts ************/
