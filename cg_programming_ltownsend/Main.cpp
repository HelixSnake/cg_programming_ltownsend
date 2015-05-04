#include "Application.h"

GLFWwindow* window = NULL;

int InitWindowFailed(){
	if(glfwInit() == GLFW_FAIL){
		fprintf(stderr, "Failed to initialize GLFW\n");
		return EXIT_WITH_ERROR;
	}

	glfwWindowHint(GLFW_SAMPLES, ANTIALIASING);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPEN_GL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPEN_GL_VERSION);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Gets working on mac...
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old open gl...

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME, NULL, NULL);
	
	if(window == NULL){
		fprintf(stderr, "Failed to create/open GLFW window.\n");
		glfwTerminate();
		return EXIT_WITH_ERROR;
	}

	//Initialize GLEW.
    glfwMakeContextCurrent(window);
    
    //Ensure we can capture the escape key being pressed below.
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	return EXIT_WITH_SUCCESS;
}

GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path){
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Read in shader code here...
	string vertexShaderCode = "";
	ifstream vertexShaderStream(vertex_file_path, ios::in);
	if(vertexShaderStream.is_open()){
		string line = "";
		while(getline(vertexShaderStream, line)){
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	}

	string fragmentShaderCode = "";
	ifstream fragmentShaderStream(fragment_file_path, ios::in);
	if(fragmentShaderStream.is_open()){
		string line = "";
		while(getline(fragmentShaderStream, line)){
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength = 0;

	//Compile shaders here...
	printf("Compiling vertex shader: %s\n", vertex_file_path);
	char const *vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	//Check Vertex Shader...
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	char* vertexShaderErrorMessage = new char[infoLogLength];
	glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &vertexShaderErrorMessage[0]);

	//Compile Fragment Shader...
	printf("Compiling fragment shader: %s\n", fragment_file_path);
	char const *fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	//Check Vertex Shader...
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	char* fragmentShaderErrorMessage = new char[infoLogLength];
	glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &fragmentShaderErrorMessage[0]);

	//Link Program...
	fprintf(stdout, "Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	//Check the progam...
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	char *programErrorMessage = new char[glm::max(infoLogLength, int(1))];
	glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
	fprintf(stdout, "%s\n", &programErrorMessage[0]);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	delete(programErrorMessage);
	programErrorMessage = NULL;

	delete(vertexShaderErrorMessage);
	vertexShaderErrorMessage = NULL;

	delete(fragmentShaderErrorMessage);
	fragmentShaderErrorMessage = NULL;

	return programID;
}

int InitGlewFailed(){
	glewExperimental = true;	//Has to do with core profile.
	int init = (int)glewInit();
	if(glewInit() != GLEW_OK){
		fprintf(stderr, "Failed to initialize GLEW\n");
		return EXIT_WITH_ERROR;
	}

	return EXIT_WITH_SUCCESS;
}

GLuint& LoadQuad(){
	static GLfloat g_vertex_buffer_data[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, -0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	for(int i = 0, size = 18; i < size; ++i){
		g_vertex_buffer_data[i] -= 0.5f;
	}

	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return vertexBuffer;
}
GLuint& LoadQuadColors(){
	static GLfloat g_vertex_buffer_data[] = {
		0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0,
		1.0f, 0.0f, 0.0f, 0.0, 1.0, 0.0,
		1.0f, 1.0f, 0.0f, 0.0, 0.0, 1.0,

		0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0,
		1.0f, 1.0f, 0.0f, 0.0, 0.0, 1.0,
		0.0f, 1.0f, 0.0f, 0.0, 1.0, 0.0
	};

	for(int i = 0, size = 36; i < size; ++i){
		if (i%6 < 3)
		{
			g_vertex_buffer_data[i] -= 0.5f;
		}
	}

	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return vertexBuffer;
}


GLuint& LoadTriangle(){
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return vertexBuffer;
}

mat4 RenderVertex(GLuint vertexBuffer){
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(
		0,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		0,			//Stride...
		(void*)0	//Array buffer offset...
	);

	mat4 positionMatrix = mat4(1.0f);
	return positionMatrix;
}

void RenderColoredVertex(GLuint vertexBuffer){
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(
		0,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		sizeof(GL_FLOAT)*6,			//Stride...
		(void*)0	//Array buffer offset...
	);
	glVertexAttribPointer(
		1,			//attribute layout
		3,			//Elements in array
		GL_FLOAT,	//Each element is of type float
		GL_FALSE,	//Normalized?
		sizeof(GL_FLOAT)*6,			//Stride...
		(void*)(sizeof(GL_FLOAT)*3)	//Array buffer offset...
	);
}

void RenderTriangle(GLuint vertexBuffer, const vec3 &position){
	mat4 positionMatrix = RenderVertex(vertexBuffer);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
}

mat4 RenderQuad(GLuint vertexBuffer, const vec3 &position, const vec3 &scale){
	mat4 finalMatrix = RenderVertex(vertexBuffer);
	finalMatrix = translate(finalMatrix, position);
	mat4 scaleMatrix = mat4(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
		);
	finalMatrix = finalMatrix * scaleMatrix;

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	return finalMatrix;
}

void RenderQuadColors(GLuint vertexBuffer){
	RenderColoredVertex(vertexBuffer);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

float& GetDeltaTime(){
	static float lastTime = glfwGetTime();
	float now = glfwGetTime();
	float deltaTime = now - lastTime;

	lastTime = now;
	return deltaTime;
}

int main(){
	if(InitWindowFailed() | InitGlewFailed()){
		return EXIT_WITH_ERROR;
	}

	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	//Create and compile glsl program from shaders...
	//GLuint programID = LoadShaders("ColoredVertexShader.vertexshader", "ColoredFragmentShader.fragmentshader");
	GLuint programID = LoadShaders("BasicVertexShader.vertexshader", "BasicFragmentShader.fragmentshader");


	GLuint MVPMatrixID = glGetUniformLocation(programID, "MVP");

	float aspectRatio = SCREEN_WIDTH/(float)SCREEN_HEIGHT;

	mat4 projectionMatrix = perspective(FIELD_OF_VIEW, aspectRatio, Z_NEAR, Z_FAR);

	GLuint triangleID = LoadTriangle();
	GLuint quadID = LoadQuad();
	GLuint quadcolorID = LoadQuadColors();

	glUseProgram(programID);
			
	float paddleLY = 0;
	float paddleRY = 0.5f;
	float ballX = 0;
	float ballY = 0;
	float paddleWidth = 0.1f;
	float paddleHeight = 0.5f;
	float paddleDistance = 1.5f;
	float paddleSpeed = 0.7f;
	float ballRadius = 0.1f;
	float ballVX = 1.0f;
	float ballVY = 1.0f;
	float arenaHeight = 1.0f;
	float arenaWidth = 2.0f;

	float paddleMaxY = arenaHeight - paddleHeight / 2;
	float realPaddleWidth = paddleWidth / 2;
	float realPaddleHeight = paddleHeight / 2;

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Game Update Code
		float deltaTime = GetDeltaTime();

		if (ballX > arenaWidth - ballRadius) {
			ballX = 0;
			ballY = 0;
		}
		if (ballX < -arenaWidth + ballRadius) {
			ballX = 0;
			ballY = 0;
			ballVX = -ballVX;
		}
		if (ballY > arenaHeight - ballRadius) {
			ballY = arenaHeight - ballRadius;
			ballVY = -ballVY;
		}
		if (ballY < -arenaHeight + ballRadius) {
			ballY = -arenaHeight + ballRadius;
			ballVY = -ballVY;
		}

		if (deltaTime > 1) deltaTime = 1;

		ballX += ballVX * deltaTime;
		ballY += ballVY * deltaTime;

		//check ball collision with paddles
		if (ballX < -paddleDistance + ballRadius + realPaddleWidth &&
			ballX > -paddleDistance - ballRadius - realPaddleWidth &&
			ballY < paddleLY + realPaddleHeight + ballRadius &&
			ballY > paddleLY - realPaddleHeight - ballRadius)
		{
			ballVX = abs(ballVX);
		}

		if (ballX < paddleDistance + ballRadius + realPaddleWidth &&
			ballX > paddleDistance - ballRadius - realPaddleWidth &&
			ballY < paddleRY + realPaddleHeight + ballRadius &&
			ballY > paddleRY - realPaddleHeight - ballRadius)
		{
			ballVX = -abs(ballVX);
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) paddleLY += paddleSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) paddleLY -= paddleSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) paddleRY += paddleSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) paddleRY -= paddleSpeed * deltaTime;

		if (paddleLY > paddleMaxY) paddleLY = paddleMaxY;
		if (paddleLY < -paddleMaxY) paddleLY = -paddleMaxY;
		if (paddleRY > paddleMaxY) paddleRY = paddleMaxY;
		if (paddleRY < -paddleMaxY) paddleRY = -paddleMaxY;

		// Rendering Code
		mat4 ViewMatrix = lookAt(
			vec3(0, 0, 3),
			vec3(0, 0, 0),
			vec3(0, 1, 0)
		);

		mat4 MVPMatrixBall = projectionMatrix * ViewMatrix * RenderQuad(quadID, vec3(ballX, ballY, 0), vec3(ballRadius, ballRadius, 1.0f));
		glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVPMatrixBall[0][0]);
		mat4 MVPMatrixPaddleL = projectionMatrix * ViewMatrix * RenderQuad(quadID, vec3(-paddleDistance, paddleLY, 0), vec3(paddleWidth, paddleHeight, 1.0f));
		glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVPMatrixPaddleL[0][0]);
		mat4 MVPMatrixPaddleR = projectionMatrix * ViewMatrix * RenderQuad(quadID, vec3(paddleDistance, paddleRY, 0), vec3(paddleWidth, paddleHeight, 1.0f));
		glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVPMatrixPaddleR[0][0]);
		//Render();
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	return EXIT_WITH_SUCCESS;
}