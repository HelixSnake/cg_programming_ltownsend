#pragma once
#include "Application.h"
#include <map>

class Material{
public:
	Material(GLuint shaderSetID);
	void AddUniformVariable(GLuint* variableAddress, string uniformVar);
	void RemoveUniformVariable(GLuint* variableAddress);
	void LoadMaterial();
private:
	GLuint _shaderSetID;
	map<GLuint*, string> _uniformVariables;
};