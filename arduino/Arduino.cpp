#include "Arduino.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <stdlib.h>
#define r1 8
#define r2 7
#define r3 6
#define r4 5
#define c1 4
#define c2 3
#define c3 2
#define rled 11
#define gled 10
#define bled 9

namespace ArduinoData {
	std::mutex lock;
	bool outputs[20];
	bool keys[12];
	bool exited = false;
	int led_r=0, led_g=0, led_b=0;
	int millis_timer = 0;
}

void delay(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void delayMicroseconds(int us) {
	std::this_thread::sleep_for(std::chrono::microseconds(us));
}

void pinMode(int pin, int type) {
	
}

bool digitalRead(int pin) {
	std::lock_guard<std::mutex> guard(ArduinoData::lock);
	int offset = -1;

	switch (pin) {
		case c1:
			offset = 0;
			break;
		case c2:
			offset = 1;
			break;
		case c3:
			offset = 2;
			break;
		default:
			return ArduinoData::outputs[pin];
	}

	if (offset != -1) {
		if (!ArduinoData::outputs[r1]) // 1 2 3
			return !ArduinoData::keys[1+offset];
		else if (!ArduinoData::outputs[r2]) // 4 5 6
			return !ArduinoData::keys[4+offset];
		else if (!ArduinoData::outputs[r3]) // 7 8 9
			return !ArduinoData::keys[7+offset];
		else if (!ArduinoData::outputs[r4]) {// Back 0 Enter
			switch (offset) {
				case 0:
					return !ArduinoData::keys[0];
				case 1:
					return !ArduinoData::keys[10];
				case 2:
					return !ArduinoData::keys[11];
				default:
					return true;
			}
		} else
			return true;
	}
	return true;
}

void digitalWrite(int pin, bool value) {
	std::lock_guard<std::mutex> guard(ArduinoData::lock);
	ArduinoData::outputs[pin] = value;

	int led_value = value ? 0 : 255;

	switch (pin) {
		case rled:
			ArduinoData::led_r = led_value;
			break;
		case gled:
			ArduinoData::led_g = led_value;
			break;
		case bled:
			ArduinoData::led_b = led_value;
			break;
	}
}

void analogWrite(int pin, uint8_t value) {
	std::lock_guard<std::mutex> guard(ArduinoData::lock);
	switch (pin) {
		case rled:
			ArduinoData::led_r = value;
			break;
		case gled:
			ArduinoData::led_g = value;
			break;
		case bled:
			ArduinoData::led_b = value;
			break;
	}
}

std::string getLEDs() {
	std::lock_guard<std::mutex> guard(ArduinoData::lock);
	return std::to_string(int(ArduinoData::led_r)) + "," + std::to_string(int(ArduinoData::led_g)) + "," + std::to_string(int(ArduinoData::led_b));
}

void keyDown(int key) {
	std::lock_guard<std::mutex> guard(ArduinoData::lock);
	ArduinoData::keys[key] = true;
}
/*
void keyUp(int key) {
	std::lock_guard<std::mutex> guard(ArduinoData::lock);
	ArduinoData::keys[key] = false;
}
*/
bool getKey(int key) {
	std::lock_guard<std::mutex> guard(ArduinoData::lock);
	bool pressed = ArduinoData::keys[key];
	ArduinoData::keys[key] = false;
	return pressed;
}

void randomSeed(int seed) {
	srand(seed);
}

int random(int max) {
	return rand()%max;
}

int random(int min, int max) {
	return rand()%(max-min) + min;
}

int millis() {
	return ArduinoData::millis_timer;
	//return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).count();
}

void millisTick(int ms) {
	ArduinoData::millis_timer += ms;
}

bool bitRead(uint8_t value, int bit_pos) {
	return (value & (1 << bit_pos)) != 0;
}

void bitClear(uint8_t &value, int bit_pos) {
	value = value & ~(1 << bit_pos);
}

void bitSet(uint8_t &value, int bit_pos) {
	value = value | (1 << bit_pos);
}

/*void exit(int code) {
	while (true);
}*/

void SerialSimulator::begin(int baud) {
	
}

void SerialSimulator::write(char c) {
	
}

void SerialSimulator::print(std::string str) {
	
}

void SerialSimulator::print(int c) {
	
}

void SerialSimulator::println(std::string str) {
	
}

void SerialSimulator::println(int c) {
	
}

void SerialSimulator::println() {
	
}

char SerialSimulator::read() {
	return 0;
}

bool SerialSimulator::available() {
	return false;
}
