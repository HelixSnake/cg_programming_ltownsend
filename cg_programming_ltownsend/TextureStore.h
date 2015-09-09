#include "Application.h"
#include "BitmapReader.h"
#include <map>

#pragma once

class TextureStore
{
public:
	static GLuint AddTexture(const char* path);
	static GLuint GetTexture(const char* path);
private:
	static std::map<std::string, GLuint> _indexMap;
};