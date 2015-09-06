#pragma once

//GLEW provides the function pointers we need t
#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Standard headers...
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <string>

// Namespaces...
using namespace glm; // For OpenGL math
using namespace std;

// Defines...
#define GLFW_DLL
#define APP_NAME "Landon Townsend's Kickass OpenGL Project"
#define EXIT_WITH_ERROR -1
#define EXIT_WITH_SUCCESS 0
#define OPEN_GL_VERSION 3 // Specifies OpenGL 3.3
#define ANTIALIASING 4 // nx antialiasing
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define FIELD_OF_VIEW 45.0f
#define Z_NEAR 0.1f
#define Z_FAR 100.0f

#define GLFW_FAIL 0

// External Globals...
extern GLFWwindow* window;
extern GLint shaderStatus;
extern bool activeRef;

struct Camera{
	GLuint projMatrixID;
	GLuint MVPMatrixID;
	GLuint MVMatrixID;
	GLuint viewMatrixID;
	GLuint fwdVecID;
	vec3 fwdVec;
	mat4 projectionMatrix, viewMatrix, MVPMatrix;
};

struct DirectionLight{
	GLuint directionID;
	GLuint diffuseColorID;
	GLuint specColorID;
	vec3 diffuseColor, specColor, direction;
};
