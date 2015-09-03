#include "Application.h"
#include "BitmapReader.h"
#include <map>

#pragma once

class TextureStore
{
public:
	static GLuint AddTexture(char* path);
	static GLuint GetTexture(char* path);
private:
	static std::map<std::string, GLuint> _indexMap;
};