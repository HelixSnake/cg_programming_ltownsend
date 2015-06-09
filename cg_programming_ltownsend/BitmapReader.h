#include "Application.h"

#pragma once

class BitmapReader {
	public:
		static GLuint loadBMP_custom(const char * imagepath);
		static GLuint loadBMP_alpha(const char * imagepath);
};