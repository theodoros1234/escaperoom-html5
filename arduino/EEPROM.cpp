#include "EEPROM.h"

void EEPROMSimulator::write(int address, uint8_t value) {
	EEPROMSimulator::value = value;
}

uint8_t EEPROMSimulator::read(int address) {
	return EEPROMSimulator::value;
}

int EEPROMSimulator::length() {
	return 1;
}