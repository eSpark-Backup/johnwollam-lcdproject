#include "EEPROM.h"

int addr = 0;

char message_o[69];

struct messageObject {
  char message[69];
};

void setup() {
  Serial.begin(115200);

  EEPROM.begin();

  messageObject putObject = {"hello"};

  for (int i = 0; i < 100; i += 10)
  {
    EEPROM.put(i, putObject);
  }

  Serial.println("EEPROM length");
  Serial.println(EEPROM.length());


}

void loop() {
  // put your main code here, to run repeatedly:

  messageObject getObject;
  messageObject putObject = {"hello"};

  Serial.print("Addr "); Serial.print(addr); Serial.println(" before writing");
  EEPROM.get(addr, message_o);
  Serial.println(message_o);

  addr += 10;

  delay(2000);
}
