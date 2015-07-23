#include "Mesh.h"

MeshTri::MeshTri()
{
}

Mesh::Mesh()
{
	_numTris = 0;
	_tris = nullptr;
}

Mesh::~Mesh()
{
	if (_tris != nullptr) delete _tris;
}
	

