#include "Level.h"

Level::Level(int width, int height)
{
	_Init(width, height);
}
Level::Level(const char *filename)
{
	ifstream file;
	file.open(filename);

	string instr;

	if (!getline(file, instr))
	{
		printf("Failed to load height");
		_Init(1,1);
		file.close();
		return;
	}
	_height = std::stoi(instr);
	if (!getline(file, instr))
	{
		printf("Failed to load width");
		_Init(1,1);
		file.close();
		return;
	}
	_width = std::stoi(instr);

	int arrLength = _height * _width;
	_Init(_height, _width);

	for (int i = 0; i < arrLength; i++)
	{
		file >> instr;
		_tiles[i] = std::stoi(instr);
	}

	file.close();
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

int Level::GetWidth()
{
	return _width;
}
int Level::GetHeight()
{
	return _height;
}

void Level::Print()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			cout << _tiles[i * _width + j] << " ";
		}
		cout << endl;
	}
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