#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "../includes/_3dFunctions.h"

using namespace std;

class Sprite {
public:
	vector<vector<char>> datas;
	vector<char> data;
	int nWidth = 0;
	int nHeight = 0;
	short n_PerCol;

	Sprite(const std::string& filename, short nPerCol, bool invert);
	void LoadTextFile(const std::string& filename, short nPerCol, bool invert);
	char SampleGlyph(float x, float y, uint8_t face);
	char SampleGlyphQuad(float x, float y, uint8_t face);
};