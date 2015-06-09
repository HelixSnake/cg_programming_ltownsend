#include "PlayerObject.h"

PlayerObject::PlayerObject(){
	animTime = 0;
	objectState = NULL;
	renderMode = GL_TRIANGLES;
	vertexBufferID = 0;
	uvBufferID = 0;
	SetPosition(vec3(0));
	SetScale(vec3(1));
	numIndices = 6;
	LoadTriangles(2, 0, GL_TRIANGLES);
	textureIDs[0] = TextureStore::AddTexture("char1a.bmp");
	textureIDs[1] = TextureStore::AddTexture("char1b.bmp");
	textureIDs[2] = TextureStore::AddTexture("char2a.bmp");
	textureIDs[3] = TextureStore::AddTexture("char2b.bmp");
	textureIDs[4] = TextureStore::AddTexture("char3a.bmp");
	textureIDs[5] = TextureStore::AddTexture("char3b.bmp");
	textureIDs[6] = TextureStore::AddTexture("char4a.bmp");
	textureIDs[7] = TextureStore::AddTexture("char4b.bmp");
	currentTextureID = textureIDs[0];
}

PlayerObject::~PlayerObject(){
	
}

void PlayerObject::SetPosition(vec3 position){
	this->position = position;
}

void PlayerObject::SetScale(vec3 scale){
	this->scale = scale;
}

vec3 PlayerObject::GetPosition(){
	return position;
}

float PlayerObject::GetLeftX(){
	return leftX;
}

float PlayerObject::GetRightX(){
	return rightX;
}

float PlayerObject::GetTopY(){
	return topY;
}

float PlayerObject::GetBottomY(){
	return bottomY;
}

void PlayerObject::Update(const float& deltaTime){
	float animLength = 0.1;
	animTime += deltaTime;
	//if (animTime > animLength * 2) animTime -= animLength * 2;
	int animFrame = (int)floor(animTime / animLength) % 2;
	if (animFrame > 1)
	{
		int asdfas = 2;
	}
	leftX = position.x - scale.x;
	rightX = position.x + scale.x;
	topY = position.y + scale.y;
	bottomY = position.y - scale.y;
	float move_speed = 5;
	vec3 movevec;
	movevec.x = 0;
	movevec.y = 0;
	movevec.z = 0;
	if (glfwGetKey(window, GLFW_KEY_LEFT))
	{
		currentTextureID = textureIDs[2 + animFrame];
		movevec.x -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		currentTextureID = textureIDs[6 + animFrame];
		movevec.x += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_UP))
	{
		currentTextureID = textureIDs[4 + animFrame];
		movevec.y += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN))
	{
		currentTextureID = textureIDs[0 + animFrame];
		movevec.y -= 1;
	}
	if (movevec.x != 0 || movevec.y != 0)
	{
		movevec = glm::normalize(movevec);
	}

	position += movevec * move_speed * deltaTime;
}

void PlayerObject::Render(const Camera& camera){
	mat4 modelMatrix = Render();
	mat4 MVPMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

	glUniformMatrix4fv(camera.MVPMatrixID, 1, GL_FALSE, &MVPMatrix[0][0]);
}

void PlayerObject::LoadTriangles(const GLuint& perRow, const GLuint& perColumn, const GLenum& renderMode){

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

void PlayerObject::SaveObjectState(char *message){
	if(objectState == NULL)
		objectState = (PlayerObject*)malloc(sizeof(*this));

	*objectState = *this;
	puts(message);
}

void PlayerObject::LoadObjectState(char *message){
	*this = *objectState;
	puts(message);
}

mat4 PlayerObject::Render(){
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
	
	glBindTexture(GL_TEXTURE_2D, this->currentTextureID);

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