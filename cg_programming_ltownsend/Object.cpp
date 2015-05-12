#include "Object.h"

Object::Object(){
	objectState = NULL;
	renderMode = GL_TRIANGLES;
	vertexBufferID = 0;
	SetPosition(vec3(0));
	SetScale(vec3(1));
	numIndices = 6;
	LoadTriangles(2, 0, GL_TRIANGLE_STRIP);
}

Object::~Object(){
	
}

void Object::SetPosition(vec3 position){
	this->position = position;
}

void Object::SetScale(vec3 scale){
	this->scale = scale;
}

vec3 Object::GetPosition(){
	return position;
}

float Object::GetLeftX(){
	return leftX;
}

float Object::GetRightX(){
	return rightX;
}

float Object::GetTopY(){
	return topY;
}

float Object::GetBottomY(){
	return bottomY;
}

void Object::Update(const float& deltaTime){
	leftX = position.x - scale.x;
	rightX = position.x + scale.x;
	topY = position.y + scale.y;
	bottomY = position.y - scale.y;
}

void Object::Render(const Camera& camera){
	mat4 modelMatrix = Render();
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
}

void Object::LoadTriangles(const GLuint& perRow, const GLuint& perColumn, const GLenum& renderMode){

	static const GLfloat vertexBuffer[] = {
		-1.0f, 1.0f,  0.0f,
		-1.0f,-1.0f, 0.0f,
		1.0f, 1.0f,  0.0f,
		1.0f, -1.0f, 0.0f
	};
	
	numIndices = 4;

	this->renderMode = renderMode;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
}

void Object::SaveObjectState(char *message){
	if(objectState == NULL)
		objectState = (Object*)malloc(sizeof(*this));

	*objectState = *this;
	puts(message);
}

void Object::LoadObjectState(char *message){
	*this = *objectState;
	puts(message);
}

mat4 Object::Render(){
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glVertexAttribPointer(
		0,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	glDrawArrays(renderMode, 0, numIndices);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
	glDisableVertexAttribArray(0);

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