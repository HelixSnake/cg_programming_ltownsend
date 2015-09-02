#include "ModeledObject.h"

ModeledObject::ModeledObject(Mesh* mesh, char* texture){
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
	textureID = TextureStore::AddTexture(texture);
	LoadTriangles();
	material = nullptr;
}

ModeledObject::~ModeledObject(){
	
}

void ModeledObject::SetShaderSetID(GLuint setID){
	if (material != nullptr)
	{
		delete material;
	}
	material = new Material(setID);
}

void ModeledObject::SendUniformVariable(GLuint* variableAddress, string uniformString){
	if (material != nullptr){
	material->AddUniformVariable(variableAddress, uniformString);
	}
}

void ModeledObject::SetPosition(vec3 position){
	this->position = position;
}

void ModeledObject::SetScale(vec3 scale){
	this->scale = scale;
}

void ModeledObject::SetRotationMatrix(mat4 rotation){
	this->rotMatrix = rotation;
}


void ModeledObject::AddRotation(vec3 axis, float angle){
	rotMatrix = glm::rotate(rotMatrix, angle, axis);
}

vec3 ModeledObject::GetPosition(){
	return position;
}

float ModeledObject::GetLeftX(){
	return leftX;
}

float ModeledObject::GetRightX(){
	return rightX;
}

float ModeledObject::GetTopY(){
	return topY;
}

float ModeledObject::GetBottomY(){
	return bottomY;
}

void ModeledObject::Update(const float& deltaTime){
	leftX = position.x - scale.x;
	rightX = position.x + scale.x;
	topY = position.y + scale.y;
	bottomY = position.y - scale.y;
}

void ModeledObject::LoadMaterial(){
	if (material != nullptr){
		material->LoadMaterial();
	}
}

void ModeledObject::Render(const Camera& camera){
	mat4 identityMatrix = mat4(1.0f);
	mat4 scaleMatrix = glm::scale(identityMatrix, scale);
	mat4 translationMatrix = translate(identityMatrix, position);
	mat4 modelMatrix = translationMatrix * rotMatrix * scaleMatrix;

	glUniformMatrix4fv(camera.MVMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
	Render();
}

void ModeledObject::LoadTriangles(){

	GLfloat *vertexBuffer = new GLfloat[numVertInds];
	for (int i = 0; i < loadedMesh->_numTris; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			vertexBuffer[i*9 + j*3 + 0] = loadedMesh->_tris[i].vertices[j].x;
			vertexBuffer[i*9 + j*3 + 1] = loadedMesh->_tris[i].vertices[j].y;
			vertexBuffer[i*9 + j*3 + 2] = loadedMesh->_tris[i].vertices[j].z;
		}
	}

	GLfloat *uvBuffer = new GLfloat[numUVInds];
	for (int i = 0; i < loadedMesh->_numTris; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			uvBuffer[i*6 + j*2 + 0] = loadedMesh->_tris[i].uvs[j].x;
			uvBuffer[i*6 + j*2 + 1] = loadedMesh->_tris[i].uvs[j].y;
		}
	}

	GLfloat *normalBuffer = new GLfloat[numNormalInds];
	for (int i = 0; i < loadedMesh->_numTris; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			normalBuffer[i*9 + j*3 + 0] = loadedMesh->_tris[i].normals[j].x;
			normalBuffer[i*9 + j*3 + 1] = loadedMesh->_tris[i].normals[j].y;
			normalBuffer[i*9 + j*3 + 2] = loadedMesh->_tris[i].normals[j].z;
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

	delete uvBuffer;
	delete vertexBuffer;
	delete normalBuffer;
}

void ModeledObject::SaveObjectState(char *message){
	if(objectState == NULL)
		objectState = (ModeledObject*)malloc(sizeof(*this));

	*objectState = *this;
	puts(message);
}

void ModeledObject::LoadObjectState(char *message){
	*this = *objectState;
	puts(message);
}

void ModeledObject::Render(){
	
	
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

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

	glDrawArrays(renderMode, 0, numVertInds);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	//Every object starts off with an identity matrix...
	/*mat4 objectMatrix = mat4(1.0f);
	mat4 identityMatrix = glm::scale(objectMatrix, scale);	
	mat4 modelMatrix = translate(identityMatrix, position);*/
}

//static mat4 Render(GLuint vertexBuffer, const vec3& position, const vec3& scale);