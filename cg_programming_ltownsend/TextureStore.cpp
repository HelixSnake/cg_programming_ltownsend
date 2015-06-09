#include "TextureStore.h"

std::map<std::string, GLuint> TextureStore::_indexMap;

GLuint TextureStore::AddTexture(char* path)
{
	if (_indexMap.find(std::string(path)) == _indexMap.end())
	{
		GLuint imgindex = BitmapReader::loadBMP_alpha(path);
		_indexMap.insert(std::pair<std::string, GLuint>(std::string(path), imgindex));
		return imgindex;
	}
	else return _indexMap.at(std::string(path));
}

GLuint TextureStore::GetTexture(char* path)
{
	if (_indexMap.find(std::string(path)) == _indexMap.end()) return 0;
	return _indexMap.at(std::string(path));
}