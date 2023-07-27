#include "LiquidCrystal.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <mutex>

void LiquidCrystal::setCursor(int col, int row) {
	std::lock_guard<std::mutex> guard(lock);
	updatePixels(cursor_pos_row, cursor_pos_col, true);
	cursor_pos_col = col;
	cursor_pos_row = row;
}

void LiquidCrystal::print(std::string str) {
	//std::cout << str;
	for (auto i:str)
		LiquidCrystal::write(i);
}

void LiquidCrystal::print(int n) {
	//std::cout << n;
	std::string s = std::to_string(n);
	LiquidCrystal::print(s);
}

void LiquidCrystal::print(char c) {
	//std::cout << c;
	LiquidCrystal::write(c);
}

void LiquidCrystal::write(char c) {
	std::lock_guard<std::mutex> guard(lock);
	//std::cout << c;
	updatePixels(cursor_pos_row, cursor_pos_col, true);
	if (cursor_pos_row < numof_rows && cursor_pos_col < numof_cols)
		display[cursor_pos_row][cursor_pos_col] = c;
	cursor_pos_col++;
	updatePixels(cursor_pos_row, cursor_pos_col-1);
}

void LiquidCrystal::clear() {
	std::lock_guard<std::mutex> guard(lock);
	//std::cout << std::endl << std::endl << std::endl;
	
	for (int i=0; i<numof_rows; i++)
		for (int j=0; j<numof_cols; j++) {
			display[i][j] = ' ';
			updatePixels(i, j, true);
		}
	cursor_pos_row = 0;
	cursor_pos_col = 0;
}

void LiquidCrystal::cursor() {
	std::lock_guard<std::mutex> guard(lock);
	cursor_visible = true;
}

void LiquidCrystal::blink() {
	std::lock_guard<std::mutex> guard(lock);
	blinking = true;
}

void LiquidCrystal::noCursor() {
	std::lock_guard<std::mutex> guard(lock);
	cursor_visible = false;
}

void LiquidCrystal::noBlink() {
	std::lock_guard<std::mutex> guard(lock);
	blinking = false;
}

void LiquidCrystal::setBlinkInversion(bool s) {
	pixel_lock.lock();
	blinking_inverted = s;
	pixel_lock.unlock();
	updatePixels(cursor_pos_row, cursor_pos_col);
}

void LiquidCrystal::loadFont(uint8_t input[256][8]) {
	for (int i=0; i<256; i++)
		createChar(i, input[i]);
}

void LiquidCrystal::createChar(int pos, uint8_t bitmap[]) {
	std::lock_guard<std::mutex> pixel_guard(pixel_lock);
	
	for (int i=0; i<8; i++) {
		font[pos][i][0] = bitmap[i] & 0b10000;
		font[pos][i][1] = bitmap[i] & 0b01000;
		font[pos][i][2] = bitmap[i] & 0b00100;
		font[pos][i][3] = bitmap[i] & 0b00010;
		font[pos][i][4] = bitmap[i] & 0b00001;
		
		/*
		for (int j=0; j<5; j++)
			if (font[pos][i][j])
				std::cout << "X";
			else
				std::cout << " ";
		std::cout << std::endl;
		*/
	}
}

void LiquidCrystal::begin(int cols, int rows) {
	std::lock_guard<std::mutex> guard(lock);
	std::lock_guard<std::mutex> pixel_guard(pixel_lock);
	
	LiquidCrystal::numof_cols = cols;
	LiquidCrystal::numof_rows = rows;
	
	// Create character array
	display = new char*[rows];
	for (int i=0; i<rows; i++) {
		display[i] = new char[cols];
		// Fill array with spaces
		for (int j=0; j<cols; j++)
			display[i][j] = ' ';
	}
	
	// Create display bitmap
	pixels = new bool*[rows*8];
	for (int i=0; i<rows*8; i++) {
		pixels[i] = new bool[cols*5];
		// Fill array with spaces
		for (int j=0; j<cols*5; j++)
			pixels[i][j] = false;
	}
}

std::string LiquidCrystal::dumpToStringASCII() {
	std::lock_guard<std::mutex> guard(lock);

	if (display == nullptr)
		return "";

	std::string s;
	if (display != nullptr) {
		for (int i=0; i<numof_rows; i++) {
			for (int j=0; j<numof_cols; j++)
				s += display[i][j];
			s += "\n";
		}
	}
	return s;
}

void LiquidCrystal::dumpToConsoleASCII() {
	std::cout << dumpToStringASCII();
}

std::string LiquidCrystal::dumpToStringBitmap() {
	std::lock_guard<std::mutex> pixel_guard(pixel_lock);

	if (pixels == nullptr)
		return "";

	std::string s;
	for (int i=0; i<numof_rows*8; i++) {
		for (int j=0; j<numof_cols*5; j++) {
			if (pixels[i][j])
				s += "X";
			else
				s += " ";
		}
		s += "\n";
	}
	return s;
}

void LiquidCrystal::dumpToConsoleBitmap() {
	std::cout << dumpToStringBitmap();
}

void LiquidCrystal::updatePixels(int row, int col) {
	updatePixels(row, col, false);
}

void LiquidCrystal::updatePixels(int row, int col, bool ignoreCursor) {
	std::lock_guard<std::mutex> guard(lock);
	std::lock_guard<std::mutex> pixel_guard(pixel_lock);
	if (display == nullptr || pixels == nullptr)
		return;
	int offset_x = col * 5;
	int offset_y = row * 8;
	if (col >= numof_cols || row >= numof_rows || col < 0 || row < 0)
		return;
	char c = display[row][col];
	bool on_current_cell = row == cursor_pos_row && col == cursor_pos_col;
	//std::cout << c << int(c) << ", ";
	
	for (int i=0; i<8; i++)
		for (int j=0; j<5; j++) {
			if (!ignoreCursor && on_current_cell && blinking && blinking_inverted)
				pixels[offset_y+i][offset_x+j] = true;
			else if (!ignoreCursor && on_current_cell && cursor_visible && i>=6)
				pixels[offset_y+i][offset_x+j] = true;
			else
				pixels[offset_y+i][offset_x+j] = font[c][i][j];
		}
}
