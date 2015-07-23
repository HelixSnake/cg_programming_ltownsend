#include "MeshLoader.h"

bool MeshLoader::loadMesh(Mesh *mesh, const char* path)
{
	int numVerts = 0;
	int numUVs = 0;
	int numNormals = 0;
	int numFaces = 0;

	FILE *file = fopen(path, "r");
	if( file == NULL ){
    printf("Impossible to open the file !\n");
    return false;
	}
	while (true){
		char linebuffer[1024];
		char lineHeader[256];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) break;
		if ( strcmp( lineHeader, "v" ) == 0 ){
			++numVerts;
			fgets(linebuffer, 1024, file); 
		}
		if ( strcmp( lineHeader, "vt" ) == 0 ){
			++numUVs;
			fgets(linebuffer, 1024, file); 
		}
		if ( strcmp( lineHeader, "vn" ) == 0 ){
			++numNormals;
			fgets(linebuffer, 1024, file); 
		}
		if ( strcmp( lineHeader, "f" ) == 0 ){
			++numFaces;
			fgets(linebuffer, 1024, file); 
		}
	}
	rewind(file);
	vec3 *verts = new vec3[numVerts];
	vec2 *uvs = new vec2[numUVs];
	vec3 *normals = new vec3[numNormals];
	int **faces = new int*[numFaces];

	for (int i = 0; i < numFaces; i++) {
		faces[i] = new int[9];
	}

	int currentVert = 0;
	int currentUV = 0;
	int currentNormal = 0;
	int currentFace = 0;

	while (true){
		char lineHeader[256];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) break;
		if (strcmp(lineHeader, "v") == 0){
			vec3 *cVP = &verts[currentVert];
			fscanf(file, "%f %f %f\n", &cVP->x, &cVP->y, &cVP->z );
			++currentVert;
		}
		if (strcmp(lineHeader, "vt") == 0){
			vec2 *cUV = &uvs[currentUV];
			fscanf(file, "%f %f\n", &cUV->x, &cUV->y);
			++currentUV;
		}
		if (strcmp(lineHeader, "vn") == 0){
			vec3 *cNP = &normals[currentNormal];
			fscanf(file, "%f %f %f\n", &cNP->x, &cNP->y, &cNP->z );
			++currentNormal;
		}
		if (strcmp(lineHeader, "f") == 0){
			int **cFP = &faces[numFaces];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &(*cFP)[0], &(*cFP)[1], &(*cFP)[2], &(*cFP)[3], &(*cFP)[4], &(*cFP)[5], &(*cFP)[6], &(*cFP)[7], &(*cFP)[8]);
			if (matches != 9){
				printf("File can't be read by our simple parser :( Try exporting with other options\n");
				delete verts, uvs, normals;
				delete[] faces;
				return false;
			}
			++currentFace;
		}
	}
	
	mesh->_numTris = numFaces;
	mesh->_tris = new MeshTri[numFaces];

	for (int i = 0; i < numFaces; i++) {
		MeshTri *crntMeshTri = &mesh->_tris[i];
		for (int j = 0; j < 3; j++) {
			int vertIndex = faces[i][j * 3] - 1;
			if (vertIndex >= numVerts) {
				delete verts, uvs, normals;
				delete[] faces;
				return false;
			}
			crntMeshTri->vertices[j] = verts[vertIndex];

			int uvIndex = faces[i][j * 3 + 1] - 1;
			if (uvIndex >= numUVs) {
				delete verts, uvs, normals;
				delete[] faces;
				return false;
			}
			crntMeshTri->uvs[j] = uvs[uvIndex];

			int normalIndex = faces[i][j * 3 + 2] - 1;
			if (normalIndex >= numNormals) {
				delete verts, uvs, normals;
				delete[] faces;
				return false;
			}
			crntMeshTri->normals[j] = normals[normalIndex];
		}
	}
	delete verts, uvs, normals;
	delete[] faces;
	return true;
}