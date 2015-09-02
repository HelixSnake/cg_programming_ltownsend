#include "Material.h"

Material::Material(GLuint shaderSetID){
	_shaderSetID = shaderSetID;
}

void Material::AddUniformVariable(GLuint* variableAddress, string uniformVar){
	_uniformVariables.insert(pair<GLuint*, string>(variableAddress, uniformVar));
}

void Material::RemoveUniformVariable(GLuint* variableAddress){
	_uniformVariables.erase(variableAddress);
}

void Material::LoadMaterial(){
	glUseProgram(_shaderSetID);
	for (map<GLuint*, string>::iterator iter = _uniformVariables.begin(); iter != _uniformVariables.end(); ++iter){
		*iter->first = glGetUniformLocation(_shaderSetID, iter->second.data());
	}
}