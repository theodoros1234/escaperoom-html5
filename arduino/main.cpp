#include "arduino_code.h"
#include "Arduino.h"
#include "font.h"
#include <thread>
#include <string>
#include <emscripten/bind.h>

LiquidCrystal* lcd_object = getLCD();

void monitorDisplay() {
	while (true) {
		lcd_object->dumpToConsoleBitmap();
		delay(50);
	}
}

std::string getDisplayContentsASCII() {
	return lcd_object->dumpToStringASCII();
}

std::string getDisplayContentsBitmap() {
	return lcd_object->dumpToStringBitmap();
}

void loadDisplayFont() {
	lcd_object->loadFont(default_font);
}

void lcdSetBlinkInversion(bool s) {
	lcd_object->setBlinkInversion(s);
}

EMSCRIPTEN_BINDINGS(my_module) {
	emscripten::function("getDisplayContentsASCII", &getDisplayContentsASCII);
	emscripten::function("getDisplayContentsBitmap", &getDisplayContentsBitmap);
	emscripten::function("loadDisplayFont", &loadDisplayFont);
	emscripten::function("setup", &setup);
	emscripten::function("loop", &loop);
	emscripten::function("splashScreen", &splashScreen);
	emscripten::function("lcdSetBlinkInversion", &lcdSetBlinkInversion);
	emscripten::function("digitalWrite", &digitalWrite);
	emscripten::function("keyDown", &keyDown);
	//emscripten::function("keyUp", &keyUp);
	emscripten::function("ISR2", &ISR2);
	emscripten::function("millisTick", &millisTick);
	emscripten::function("getLEDs", &getLEDs);
	emscripten::function("congratulations", &congratulations);
	emscripten::function("isCongratulationsEnabled", &isCongratulationsEnabled);
}
/*
int main() {
	loadDisplayFont();
	
	std::thread monitorDisplayThread(monitorDisplay);
	
	setup();
	while (true)
		loop();
	
}*/
