#pragma once
#include "Application.h"

struct MeshTri{
	MeshTri();
	vec3 vertices[3];
	vec2 uvs[3];
	vec3 normals[3];
};

struct Mesh{
	Mesh();
	~Mesh();
	int _numTris;
	MeshTri *_tris;
};