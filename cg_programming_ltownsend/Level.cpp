#include "Level.h"

Level::Level(int width, int height, int numImages)
{
	_Init(width, height, numImages);
}
Level::Level(const char *filename)
{
	ifstream file;
	file.open(filename);

	string instr;

	if (!getline(file, instr))
	{
		printf("Failed to load height");
		_Init(1,1,1);
		file.close();
		return;
	}
	_height = std::stoi(instr);
	if (!getline(file, instr))
	{
		printf("Failed to load width");
		_Init(1,1,1);
		file.close();
		return;
	}
	_width = std::stoi(instr);

	if (!getline(file, instr))
	{
		printf("Failed to load image amount");
		_Init(1,1,1);
		file.close();
		return;
	}
	_numTileImages = std::stoi(instr);

	int arrLength = _height * _width;
	_Init(_height, _width, _numTileImages);

	for (int i = 0; i < _numTileImages; i++)
	{
		file >> instr;
		_tileImages[i] = new char[instr.length() + 1];
		strcpy(_tileImages[i], instr.data());
		file >> instr;
		_tileImageIndexes[i] = std::stoi(instr);
	}

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
	delete _tileImageIDs;
	delete _tileImageIndexes;
	delete[] _tileImages;
}

void Level::GenerateImageIDs()
{
	for (int i = 0; i < _numTileImages; i++)
	{
		_tileImageIDs[i] = TextureStore::AddTexture(_tileImages[i]);
	}
}
GLuint Level::GetImageID(int index)
{
	if (index < 0 || index > _numTileImages) return 0;
	else return _tileImageIDs[index];
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
	for (int i = 0; i < _numTileImages; i++)
	{
		cout << _tileImages[i] << " " << _tileImageIndexes[i] << endl;
	}
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			cout << GetTile(j, i) << " ";
		}
		cout << endl;
	}
}

void Level::_Init(int width, int height, int numImages)
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

	_numTileImages = numImages;
	_tileImageIndexes = new int[numImages];
	_tileImages = new char*[numImages];
	_tileImageIDs = new GLuint[numImages];
	for (int i = 0; i < numImages; i++)
	{
		_tileImageIndexes[i] = 0;
		_tileImages[i] = "";
		_tileImageIDs[i] = 0;
	}
}