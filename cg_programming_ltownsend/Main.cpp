#include "Application.h"

GLFWwindow* window = NULL;

int InitWindowFailed() {
	if(glfwInit() == GLFW_FAIL) {
		fprintf(stderr, "Failed to initialize GLFW/n");
		return EXIT_WITH_ERROR;
	}
	glfwWindowHint(GLFW_SAMPLES, ANTIALIASING);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPEN_GL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPEN_GL_VERSION);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use new OpenGL version

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME, NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to create/open GLFW window/n");
		glfwTerminate();
		return EXIT_WITH_ERROR;
	}

	//Initialize GLEW.
	glfwMakeContextCurrent(window);

	//Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	return EXIT_WITH_SUCCESS;
}

int InitGlewFailed() {
	glewExperimental = true; // has to do with core profiling
	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW/n");
		return EXIT_WITH_ERROR;
	}

	return EXIT_WITH_SUCCESS;
}

int main() {
	if (InitWindowFailed() | InitGlewFailed()) {
		system("PAUSE");
		return EXIT_WITH_ERROR;
	}

	// Game Loop...
	do{
		//Update();
		//Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	return EXIT_WITH_SUCCESS;
}