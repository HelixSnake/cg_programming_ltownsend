#include "Level.h"

Level::Level(int width, int height)
{
	_Init(width, height);
}
Level::Level(const char* filename)
{
	FILE * file = fopen(filename,"r");

	if (!file) {
		printf("File could not be opened\n"); _Init(1, 1);
	}

	// File loading code here

	fclose(file);
}
Level::~Level()
{
	delete _tiles;
}

void Level::Clear()
{
	int arrayLength = _width * _height;
	for (int i = 0; i < arrayLength; i++)
	{
		_tiles[i] = 0;
	}
}

bool Level::SetTile(int x, int y, int value)
{
	if (x < 0 || y < 0 || x >= _width || y >= _height || value < 0) return false;
	_tiles[y*_width + x] = value;
}

int Level::GetTile(int x, int y)
{
	if (x < 0 || y < 0 || x >= _width || y >= _height) return -1;
	return _tiles[y*_width + x];
}

void Level::_Init(int width, int height)
{
	if (width <= 0 || height <= 0)
	{
		width = 1; height = 1;
	}
	_width = width;
	_height = height;
	int arrayLength = width * height;
	_tiles = new int[arrayLength];

	for (int i = 0; i < arrayLength; i++)
	{
		_tiles[i] = 0;
	}
}