#include "Application.h"
#include <fstream>
#include <iostream>
#pragma once

class Level {
public:
	Level(int width, int height, int numImages);
	Level(const char *filename);
	~Level();
	void Clear();
	bool SetTile(int x, int y, int value);
	int GetTile(int x, int y);
	int GetWidth();
	int GetHeight();
	void Print();
private:
	int* _tiles;
	int _width;
	int _height;
	void _Init(int width, int height, int numImages);
	int _numTileImages;
	char** _tileImages;
	int* _tileImageIndexes;
};