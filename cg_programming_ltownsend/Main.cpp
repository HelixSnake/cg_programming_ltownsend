#include "Application.h"
#include "World.h"

GLFWwindow* window = NULL;


struct vector2
{
	float x;
	float y;
};

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

GLuint AddGeometryShader(GLuint programID, const char *geometry_file_path){
	GLuint geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

	//Read in shader code here...
	string geometryShaderCode = "";
	ifstream geometryShaderStream(geometry_file_path, ios::in);
	if(geometryShaderStream.is_open()){
		string line = "";
		while(getline(geometryShaderStream, line)){
			geometryShaderCode += "\n" + line;
		}
		geometryShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength = 0;

	//Compile shaders here...
	//Compile Geometry Shader...
	printf("Compiling geometry shader: %s\n", geometry_file_path);
	char const *geometrySourcePointer = geometryShaderCode.c_str();
	glShaderSource(geometryShaderID, 1, &geometrySourcePointer, NULL);
	glCompileShader(geometryShaderID);

	//Check Geometry Shader...
	glGetShaderiv(geometryShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(geometryShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	char* geometryShaderErrorMessage = new char[infoLogLength];
	glGetShaderInfoLog(geometryShaderID, infoLogLength, NULL, &geometryShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &geometryShaderErrorMessage[0]);

	//Link Program...
	fprintf(stdout, "Linking program\n");
	glAttachShader(programID, geometryShaderID);
	glLinkProgram(programID);

	//Check the progam...
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	char *programErrorMessage = new char[glm::max(infoLogLength, int(1))];
	glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
	fprintf(stdout, "%s\n", &programErrorMessage[0]);

	glDeleteShader(geometryShaderID);
	
	delete(programErrorMessage);
	programErrorMessage = NULL;
	
	delete(geometryShaderErrorMessage);
	geometryShaderErrorMessage = NULL;

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

float DotProduct(vector2 v1, vector2 v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

int main(){
	if(InitWindowFailed() | InitGlewFailed()){
		return EXIT_WITH_ERROR;
	}
		
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	//Create and compile glsl program from shaders...
	//GLuint programID = LoadShaders("ColoredVertexShader.vertexshader", "ColoredFragmentShader.fragmentshader");
	GLuint toonShaderID = LoadShaders("ToonTexturedOutlinedVertexShader.vertexshader", "ToonTexturedOutlinedFragmentShader.fragmentshader");
	AddGeometryShader(toonShaderID, "ToonTexturedOutlinedGeometryShader.geomshader");
	GLuint iridescentShaderID = LoadShaders("IridescentVertexShader.vertexshader", "IridescentFragmentShader.fragmentshader");

	GLuint programID = LoadShaders("LitTexturedVertexShader.vertexshader", "LitTexturedFragmentShader.fragmentshader");
	GLuint advShaderID = LoadShaders("LitTexturedNormalSpec.vertexshader", "LitTexturedNormalSpec.fragmentshader");

	GLuint MVPMatrixID = glGetUniformLocation(programID, "MVP");

	GLuint triangleID = LoadTriangle();
	GLuint quadID = LoadQuad();
	GLuint quadcolorID = LoadQuadColors();


	glUseProgram(programID);

	Camera camera;
	World world;
	DirectionLight light;
	world.setCamera(&camera);
	world.setLight(&light);
	world.SetDefaultShaderSet(toonShaderID);
	world.AddShaderSet("default", programID);
	world.AddShaderSet("toon", toonShaderID);
	world.AddShaderSet("iridescent", iridescentShaderID);
	world.AddShaderSet("advShader", advShaderID);
	world.ApplyShaders();
	float aspectRatio = SCREEN_WIDTH/(float)SCREEN_HEIGHT;
	camera.MVPMatrixID = glGetUniformLocation(programID, "MVP");
	camera.MVMatrixID = glGetUniformLocation(programID, "MV");
	camera.projectionMatrix = perspective(FIELD_OF_VIEW, aspectRatio, Z_NEAR, Z_FAR);

	light.directionID = glGetUniformLocation(programID, "lightDir");
	camera.fwdVecID = glGetUniformLocation(programID, "cameraVec");

	glm::vec3 position = glm::vec3( 0, 0, 5 );
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;
 
	float speed = 5.0f; // 3 units / second
	float mouseSpeed = 0.05f;
	float elapsedTime = 0;

	light.diffuseColor = vec3(1,1,1);
	light.specColor = vec3(1,1,1);
	light.direction = vec3(0,0,-1);

	do{
		glClearColor(0.0, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Game Update Code
		float deltaTime = GetDeltaTime();
		elapsedTime += deltaTime;

		GLuint loc = glGetUniformLocationARB(programID,"time");
		glUniform1fARB(loc, elapsedTime);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		// Reset mouse position for next frame
		glfwSetCursorPos(window, 1024/2, 768/2);
		horizontalAngle += mouseSpeed * deltaTime * float(1024/2 - xpos);
		verticalAngle   += mouseSpeed * deltaTime * float( 768/2 - ypos);

		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f/2.0f),
			0,
			cos(horizontalAngle - 3.14f/2.0f)
		);	

		glm::vec3 up = glm::cross(right, direction);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			position -= right * deltaTime * speed;
		}

		// Rendering Code
		camera.viewMatrix = lookAt(
			position,
			position+direction,
			up
		);
		
		//light.direction = direction;
		glUniform3f(light.directionID, light.direction.x, light.direction.y, light.direction.z);
		glUniform3f(camera.fwdVecID, direction.x, direction.y, direction.z);
		camera.fwdVec = direction;

		world.Update(deltaTime);
		world.Render(camera);

		//Render();
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	return EXIT_WITH_SUCCESS;
}