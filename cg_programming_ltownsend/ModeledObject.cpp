#include "ModeledObject.h"

ModeledObject::ModeledObject(Mesh* mesh){
	objectState = NULL;
	renderMode = GL_TRIANGLES;
	vertexBufferID = 0;
	SetPosition(vec3(0));
	SetScale(vec3(1));
	numVertInds = mesh->_numTris * 9;
	numUVInds = mesh->_numTris * 6;
	numNormalInds = mesh->_numTris * 9;
	loadedMesh = mesh;
	textureID = TextureStore::AddTexture("arceus.bmp");
	LoadTriangles();
}

ModeledObject::~ModeledObject(){
	
}

void ModeledObject::SetPosition(vec3 position){
	this->position = position;
}

void ModeledObject::SetScale(vec3 scale){
	this->scale = scale;
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

void ModeledObject::Render(const Camera& camera){
	mat4 modelMatrix = Render();
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
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

	this->renderMode = GL_TRIANGLES;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numVertInds, vertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numUVInds, uvBuffer, GL_STATIC_DRAW);
	delete uvBuffer;
	delete vertexBuffer;
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

mat4 ModeledObject::Render(){
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
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

	glDrawArrays(renderMode, 0, numVertInds);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	//Every object starts off with an identity matrix...
	/*mat4 objectMatrix = mat4(1.0f);
	mat4 identityMatrix = glm::scale(objectMatrix, scale);	
	mat4 modelMatrix = translate(identityMatrix, position);*/

	mat4 identityMatrix = mat4(1.0f);
	mat4 translateMatrix = translate(identityMatrix, position);
	mat4 modelMatrix = glm::scale(translateMatrix, scale);

	return modelMatrix;
}

//static mat4 Render(GLuint vertexBuffer, const vec3& position, const vec3& scale);