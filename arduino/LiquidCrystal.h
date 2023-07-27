#ifndef LIQUIDCRYSTAL_H
#define LIQUIDCRYSTAL_H
#include <cstdint>
#include <string>
#include <iostream>
#include <mutex>

class LiquidCrystal {
private:
	int numof_cols = 0, numof_rows = 0;
	int cursor_pos_col = 0, cursor_pos_row = 0;
	bool cursor_visible = true, blinking = true, blinking_inverted = false;
	bool font[256][8][5];
	char** display = nullptr;
	bool** pixels = nullptr;
	std::mutex lock, pixel_lock;
	
	void updatePixels(int row, int col);
	void updatePixels(int row, int col, bool ignoreCursor);
	
public:
	void setCursor(int col, int row);
	void print(std::string str);
	void print(int n);
	void print(char c);
	void write(char c);
	void clear();
	void cursor();
	void blink();
	void noCursor();
	void noBlink();
	void setBlinkInversion(bool s);
	void loadFont(uint8_t input[256][8]);
	void createChar(int pos, uint8_t bitmap[]);
	void begin(int cols, int rows);
	std::string dumpToStringASCII();
	void dumpToConsoleASCII();
	std::string dumpToStringBitmap();
	void dumpToConsoleBitmap();
};

#endif
