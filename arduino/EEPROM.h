#ifndef EEPROM_H
#define EEPROM_H
#include <cstdint>

class EEPROMSimulator {
public:
	uint8_t value = 10;
	void write(int address, uint8_t value);
	uint8_t read(int address);
	int length();
};

#endif
