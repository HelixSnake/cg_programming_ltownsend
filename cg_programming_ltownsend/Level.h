#include "Application.h"
#pragma once

class Level {
public:
	Level(int width, int height);
	Level(const char *filename);
	~Level();
	void Clear();
	bool SetTile(int x, int y, int value);
	int GetTile(int x, int y);
private:
	int* _tiles;
	int _width;
	int _height;
	void _Init(int width, int height);
};