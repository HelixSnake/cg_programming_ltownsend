#pragma once

//GLEW provides the function pointers we need t
#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>

// Standard headers...
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

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

#define GLFW_FAIL 0

// External Globals...
extern GLFWwindow* window;
extern GLint shaderStatus;
extern bool activeRef;