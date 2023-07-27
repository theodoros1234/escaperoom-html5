#ifndef ARDUINO_H
#define ARDUINO_H
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define LOW 0
#define HIGH 1
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

void delay(int ms);
void delayMicroseconds(int us);
void pinMode(int pin, int type);
bool digitalRead(int pin);
void digitalWrite(int pin, bool value);
void analogWrite(int pin, uint8_t value);
std::string getLEDs();
void keyDown(int key);
//void keyUp(int key);
bool getKey(int key);
void randomSeed(int seed);
int random(int max);
int random(int min, int max);
int millis();
void millisTick(int ms);
bool bitRead(uint8_t value, int bit_pos);
void bitClear(uint8_t &value, int bit_pos);
void bitSet(uint8_t &value, int bit_pos);
//void exit(int code);

class SerialSimulator {
public:
	void begin(int baud);
	void write(char c);
	void print(std::string str);
	void print(int c);
	void println(std::string str);
	void println(int c);
	void println();
	char read();
	bool available();
};

#endif
