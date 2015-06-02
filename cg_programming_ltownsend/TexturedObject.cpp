#include "TexturedObject.h"
#include "BitmapReader.h"

TexturedObject::TexturedObject(){
	objectState = NULL;
	renderMode = GL_TRIANGLES;
	vertexBufferID = 0;
	uvBufferID = 0;
	SetPosition(vec3(0));
	SetScale(vec3(1));
	numIndices = 6;
	LoadTriangles(2, 0, GL_TRIANGLES);
	textureID = BitmapReader::loadBMP_custom("smiley.bmp");
}

TexturedObject::~TexturedObject(){
	
}

void TexturedObject::SetPosition(vec3 position){
	this->position = position;
}

void TexturedObject::SetScale(vec3 scale){
	this->scale = scale;
}

void TexturedObject::SetTexture(GLuint unTextureID){
	this->textureID = unTextureID;
}

GLuint TexturedObject::GetTexture(){
	return textureID;
}

vec3 TexturedObject::GetPosition(){
	return position;
}

float TexturedObject::GetLeftX(){
	return leftX;
}

float TexturedObject::GetRightX(){
	return rightX;
}

float TexturedObject::GetTopY(){
	return topY;
}

float TexturedObject::GetBottomY(){
	return bottomY;
}

void TexturedObject::Update(const float& deltaTime){
	leftX = position.x - scale.x;
	rightX = position.x + scale.x;
	topY = position.y + scale.y;
	bottomY = position.y - scale.y;
}

void TexturedObject::Render(const Camera& camera){
	mat4 modelMatrix = Render();
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
}

void TexturedObject::LoadTriangles(const GLuint& perRow, const GLuint& perColumn, const GLenum& renderMode){

	static const GLfloat vertexBuffer[] = {
		-1.0f, 1.0f,  0.0f,
		-1.0f,-1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,  0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};
	
	numIndices = 6;

	this->renderMode = renderMode;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	static const GLfloat uvBuffer[] = {
		0, 1,
		0, 0,
		1, 1,
		1, 1,
		1, 0,
		0, 0
	};

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvBuffer), uvBuffer, GL_STATIC_DRAW);
}

void TexturedObject::SaveObjectState(char *message){
	if(objectState == NULL)
		objectState = (TexturedObject*)malloc(sizeof(*this));

	*objectState = *this;
	puts(message);
}

void TexturedObject::LoadObjectState(char *message){
	*this = *objectState;
	puts(message);
}

mat4 TexturedObject::Render(){
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
	
	glDrawArrays(renderMode, 0, numIndices);	//GL_TRIANGLE_STRIP or GL_TRIANGLES
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