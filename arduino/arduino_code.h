#ifndef ARDUINO_CODE_H
#define ARDUINO_CODE_H
#include "LiquidCrystal.h"
#include "EEPROM.h"

void ISR2();
void setup();
void loop();
void splashScreen(int pos);
void congratulations();
bool isCongratulationsEnabled();
LiquidCrystal* getLCD();
EEPROMSimulator* getEEPROM();

#endif
