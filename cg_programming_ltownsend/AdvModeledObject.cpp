#include "AdvModeledObject.h"

static const int GLSL_NUM_TEXTURES = 32;
static const GLenum GLSL_TEXTURE_LOCATIONS[GLSL_NUM_TEXTURES] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8,
GL_TEXTURE9, GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20,
 GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23, GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31};

AdvModeledObject::AdvModeledObject(Mesh* mesh){
	objectState = nullptr;
	renderMode = GL_TRIANGLES;
	vertexBufferID = 0;
	SetPosition(vec3(0));
	SetScale(vec3(1));
	rotMatrix = mat4(1.0f);
	numVertInds = mesh->_numTris * 9;
	numUVInds = mesh->_numTris * 6;
	numNormalInds = mesh->_numTris * 9;
	loadedMesh = mesh;
	LoadTriangles();
	material = nullptr;
}

AdvModeledObject::~AdvModeledObject(){
	if (material != nullptr) delete material;
}

void AdvModeledObject::SetFloatVar(string name, float value){
	map<string, pair<float, GLuint>>::iterator current = var_floats.insert(var_floats.begin(), pair<string, pair<float, GLuint>>(name, pair<float, GLuint>(value, 0)));
	if (current != var_floats.end()){
		current->second.first = value;
	}
	if (material != nullptr){
		if (var_floats_defaults.find(name) == var_floats_defaults.end()){
			GLuint uniformLocation = glGetUniformLocation(material->GetShaderID(), name.data());
			float defaultValue; 
			glGetUniformfv(material->GetShaderID(), uniformLocation, &defaultValue);
			var_floats_defaults.insert(pair<string, float>(name, defaultValue));
		}
		GLuint* varID = &var_floats.at(name).second;
		material->AddUniformVariable(varID, name);
	}
}
void AdvModeledObject::SetVec3Var(string name, vec3 value){
	map<string, pair<vec3, GLuint>>::iterator current = var_vec3s.insert(var_vec3s.begin(), pair<string, pair<vec3, GLuint>>(name, pair<vec3, GLuint>(value, 0)));
	if (current != var_vec3s.end()){
		current->second.first = value;
	}
	if (material != nullptr){
		if (var_vec3s_defaults.find(name) == var_vec3s_defaults.end()){
			GLuint uniformLocation = glGetUniformLocation(material->GetShaderID(), name.data());
			float defaultValue[3]; 
			glGetUniformfv(material->GetShaderID(), uniformLocation, defaultValue);
			var_vec3s_defaults.insert(pair<string, vec3>(name, vec3(defaultValue[0], defaultValue[1], defaultValue[2])));
		}
		GLuint* varID = &var_floats.at(name).second;
		material->AddUniformVariable(varID, name);
	}
}
void AdvModeledObject::RemoveFloatVar(string name){
	var_floats.erase(name);
}
void AdvModeledObject::RemoveVec3Var(string name){
	var_vec3s.erase(name);
}

void AdvModeledObject::SetTexture(string name, string filename){
	if (var_textures.find(name) == var_textures.end() && var_textures.size() == GLSL_NUM_TEXTURES){
		printf("Error: max allowed textures on object already reached");
		return;
	}
	GLuint texture = TextureStore::AddTexture(filename.data());
	map<string, GLuint>::iterator current = var_textures.insert(var_textures.begin(), pair<string, GLuint>(name, texture));
	if (current != var_textures.end()){
		current->second = texture;
	}
}
void AdvModeledObject::RemoveTexture(string name){
	var_textures.erase(name);
}


void AdvModeledObject::SetShaderSetID(GLuint setID){
	if (material != nullptr){
		delete material;
		material = nullptr;
	}
	material = new Material(setID);
	for (map<string, pair<float, GLuint>>::iterator iter = var_floats.begin(); iter != var_floats.end(); ++iter){
		string name = iter->first;
		if (var_floats_defaults.find(name) == var_floats_defaults.end()){
			GLuint uniformLocation = glGetUniformLocation(material->GetShaderID(), name.data());
			float defaultValue; 
			glGetUniformfv(material->GetShaderID(), uniformLocation, &defaultValue);
			var_floats_defaults.insert(pair<string, float>(name, defaultValue));
		}
		material->AddUniformVariable(&iter->second.second, name);
	}
	for (map<string, pair<vec3, GLuint>>::iterator iter = var_vec3s.begin(); iter != var_vec3s.end(); ++iter){
		string name = iter->first;
		if (var_vec3s_defaults.find(name) == var_vec3s_defaults.end()){
			GLuint uniformLocation = glGetUniformLocation(material->GetShaderID(), name.data());
			float defaultValue[3]; 
			glGetUniformfv(material->GetShaderID(), uniformLocation, defaultValue);
			var_vec3s_defaults.insert(pair<string, vec3>(name, vec3(defaultValue[0], defaultValue[1], defaultValue[2])));
		}
		material->AddUniformVariable(&iter->second.second, name);
	}
}

void AdvModeledObject::SendUniformVariable(GLuint* variableAddress, string uniformString){
	if (material != nullptr){
		material->AddUniformVariable(variableAddress, uniformString);
	}
}
void AdvModeledObject::SetUniformVariable(string uniformString, vec3 value){
	if (material != nullptr){
		glUniform3f(glGetUniformLocation(material->GetShaderID(), uniformString.data()), value.x, value.y, value.z);
	}
}
void AdvModeledObject::SetUniformVariable(string uniformString, float value){
	if (material != nullptr){
		glUniform1f(glGetUniformLocation(material->GetShaderID(), uniformString.data()), value);
	}
}
void AdvModeledObject::SetUniformVariable(string uniformString, int value){
	if (material != nullptr){
		glUniform1i(glGetUniformLocation(material->GetShaderID(), uniformString.data()), value);
	}
}

void AdvModeledObject::SetPosition(vec3 position){
	this->position = position;
}

void AdvModeledObject::SetScale(vec3 scale){
	this->scale = scale;
}

void AdvModeledObject::SetRotationMatrix(mat4 rotation){
	this->rotMatrix = rotation;
}
mat4 AdvModeledObject::GetRotationMatrix(){
	return this->rotMatrix;
}

void AdvModeledObject::AddRotation(vec3 axis, float angle){
	rotMatrix = glm::rotate(rotMatrix, angle, axis);
}

vec3 AdvModeledObject::GetPosition(){
	return position;
}

float AdvModeledObject::GetLeftX(){
	return leftX;
}

float AdvModeledObject::GetRightX(){
	return rightX;
}

float AdvModeledObject::GetTopY(){
	return topY;
}

float AdvModeledObject::GetBottomY(){
	return bottomY;
}

void AdvModeledObject::Update(const float& deltaTime){
	leftX = position.x - scale.x;
	rightX = position.x + scale.x;
	topY = position.y + scale.y;
	bottomY = position.y - scale.y;
}

void AdvModeledObject::LoadMaterial(){
	if (material != nullptr){
		material->LoadMaterial();
	}
}

void AdvModeledObject::Render(const Camera& camera){
	if (material == nullptr) return;
	for (map<string, pair<float, GLuint>>::iterator iter = var_floats.begin(); iter != var_floats.end(); ++iter){
		glUniform1f(iter->second.second, iter->second.first);
	}
	for (map<string, pair<vec3, GLuint>>::iterator iter = var_vec3s.begin(); iter != var_vec3s.end(); ++iter){
		glUniform3f(iter->second.second, iter->second.first.x, iter->second.first.y, iter->second.first.z);
	}
	mat4 identityMatrix = mat4(1.0f);
	mat4 scaleMatrix = glm::scale(identityMatrix, scale);
	mat4 translationMatrix = translate(identityMatrix, position);
	mat4 modelMatrix = translationMatrix * rotMatrix * scaleMatrix;

	glUniformMatrix4fv(camera.MVMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
	Render();
	for (map<string, pair<float, GLuint>>::iterator iter = var_floats.begin(); iter != var_floats.end(); ++iter){
		glUniform1f(iter->second.second, var_floats_defaults.at(iter->first));
	}
	for (map<string, pair<vec3, GLuint>>::iterator iter = var_vec3s.begin(); iter != var_vec3s.end(); ++iter){
		vec3 defaultvec = var_vec3s_defaults.at(iter->first);
		glUniform3f(iter->second.second, defaultvec.x, defaultvec.y, defaultvec.z);
	}
}

void AdvModeledObject::LoadTriangles(){

	GLfloat *vertexBuffer = new GLfloat[numVertInds];
	for (int i = 0; i < loadedMesh->_numTris; ++i){
		for (int j = 0; j < 3; ++j){
			vertexBuffer[i*9 + j*3 + 0] = loadedMesh->_tris[i].vertices[j].x;
			vertexBuffer[i*9 + j*3 + 1] = loadedMesh->_tris[i].vertices[j].y;
			vertexBuffer[i*9 + j*3 + 2] = loadedMesh->_tris[i].vertices[j].z;
		}
	}

	GLfloat *uvBuffer = new GLfloat[numUVInds];
	for (int i = 0; i < loadedMesh->_numTris; ++i){
		for (int j = 0; j < 3; ++j){
			uvBuffer[i*6 + j*2 + 0] = loadedMesh->_tris[i].uvs[j].x;
			uvBuffer[i*6 + j*2 + 1] = loadedMesh->_tris[i].uvs[j].y;
		}
	}

	GLfloat *normalBuffer = new GLfloat[numNormalInds];
	for (int i = 0; i < loadedMesh->_numTris; ++i){
		for (int j = 0; j < 3; ++j){
			normalBuffer[i*9 + j*3 + 0] = loadedMesh->_tris[i].normals[j].x;
			normalBuffer[i*9 + j*3 + 1] = loadedMesh->_tris[i].normals[j].y;
			normalBuffer[i*9 + j*3 + 2] = loadedMesh->_tris[i].normals[j].z;
		}
	}

	GLfloat *tangentBuffer = new GLfloat[numNormalInds];
	GLfloat *biTangentBuffer = new GLfloat[numNormalInds];

	for (int i = 0; i < loadedMesh->_numTris; ++i){
		MeshTri currentTri = loadedMesh->_tris[i];
		mat2x3 tanBiTanMat;
		mat2 uvMat = mat2(currentTri.uvs[1]-currentTri.uvs[0], currentTri.uvs[2]-currentTri.uvs[0]);
		vec3 edge1 = currentTri.vertices[1]-currentTri.vertices[0];
		vec3 edge2 = currentTri.vertices[2]-currentTri.vertices[0];
		mat2x3 edgeMat = mat2x3(edge1, edge2);
		tanBiTanMat = edgeMat * inverse(uvMat);
		vec3 tangent = normalize(tanBiTanMat[0]);
		vec3 bitangent = -normalize(tanBiTanMat[1]);
		vec3 normal = loadedMesh->_tris[i].normals[0];

		tangent = tangent - dot(normal, tangent) * normal;
		bitangent = bitangent - dot(normal, bitangent) * normal - dot(tangent, bitangent) * tangent;
		tangent = normalize(tangent);
		bitangent = normalize(bitangent);
		for (int j = 0; j < 3; j++){
			tangentBuffer[i*9 + j*3 + 0] = tangent.x;
			tangentBuffer[i*9 + j*3 + 1] = tangent.y;
			tangentBuffer[i*9 + j*3 + 2] = tangent.z;
			biTangentBuffer[i*9 + j*3 + 0] = bitangent.x;
			biTangentBuffer[i*9 + j*3 + 1] = bitangent.y;
			biTangentBuffer[i*9 + j*3 + 2] = bitangent.z;
		}
	}

	this->renderMode = GL_TRIANGLES;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numVertInds, vertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numUVInds, uvBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numNormalInds, normalBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &tangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numNormalInds, tangentBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &biTangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, biTangentBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numNormalInds, biTangentBuffer, GL_STATIC_DRAW);

	delete uvBuffer;
	delete tangentBuffer;
	delete biTangentBuffer;
	delete vertexBuffer;
	delete normalBuffer;
}

void AdvModeledObject::SaveObjectState(char *message){
	if(objectState == NULL)
		objectState = (AdvModeledObject*)malloc(sizeof(*this));

	*objectState = *this;
	puts(message);
}

void AdvModeledObject::LoadObjectState(char *message){
	*this = *objectState;
	puts(message);
}

void AdvModeledObject::Render(){

	GLuint texLoc;
	map<string, GLuint>::iterator iter = var_textures.begin();
	for (int i = 0; i < var_textures.size(); ++i)
	{
		texLoc = glGetUniformLocation(material->GetShaderID(), iter->first.data());
		glUniform1i(texLoc, i);
		glActiveTexture(GLSL_TEXTURE_LOCATIONS[i]);
		glBindTexture(GL_TEXTURE_2D, iter->second);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		++iter;
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glVertexAttribPointer(
		0,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);

	glVertexAttribPointer(
		1,			//attribute layout
		2,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);

	glVertexAttribPointer(
		2,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferID);

	glVertexAttribPointer(
		3,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glBindBuffer(GL_ARRAY_BUFFER, biTangentBufferID);

	glVertexAttribPointer(
		4,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glDrawArrays(renderMode, 0, numVertInds);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	//Every object starts off with an identity matrix...
	/*mat4 objectMatrix = mat4(1.0f);
	mat4 identityMatrix = glm::scale(objectMatrix, scale);	
	mat4 modelMatrix = translate(identityMatrix, position);*/
}

//static mat4 Render(GLuint vertexBuffer, const vec3& position, const vec3& scale);