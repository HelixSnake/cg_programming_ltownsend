#pragma once
#include "Application.h"
#include "Mesh.h"

using namespace glm;

class MeshLoader{
public:
	static bool loadMesh(Mesh *mesh, const char* path, bool smoothNormals = false);
};