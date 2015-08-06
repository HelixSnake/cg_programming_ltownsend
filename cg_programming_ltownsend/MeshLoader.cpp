#include "MeshLoader.h"

int FindFaceType(char* lineBuffer){
	int sBuff;
	int matches;
	matches = sscanf(lineBuffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff);
	if (matches == 12)return 12;
	matches = sscanf(lineBuffer, "%d/%d %d/%d %d/%d %d/%d\n", &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff);
	if (matches == 8)return 8;
	matches = sscanf(lineBuffer, "%d %d %d %d\n", &sBuff, &sBuff, &sBuff, &sBuff);
	if (matches == 4)return 4;
	matches = sscanf(lineBuffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff);
	if (matches == 9) return 9;
	matches = sscanf(lineBuffer, "%d/%d %d/%d %d/%d\n", &sBuff, &sBuff, &sBuff, &sBuff, &sBuff, &sBuff);
	if (matches == 6) return 6;
	matches = sscanf(lineBuffer, "%d %d %d\n", &sBuff, &sBuff, &sBuff);
	if (matches == 3) return 3;
	return -1;
}

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
		char faceFormatBuffer[1024];
		char lineHeader[1024];
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
			fgets(faceFormatBuffer, 1024, file);
			int faceformat = FindFaceType(faceFormatBuffer);
			if (faceformat == 4 || faceformat == 8 || faceformat == 12) ++numFaces;
			//fgets(linebuffer, 1024, file); 
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

	int faceType = 0;

	while (true){
		char lineHeader[256];
		char faceFormatBuffer[1024];
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
			int **cFP = &faces[currentFace];
			/*for (int i = 0; i < 9; i++){
				(*cFP)[i] = 0;
			}*/
			memset(*cFP, 0, 9*sizeof(int));
			fgets(faceFormatBuffer, 1024, file);
			faceType = FindFaceType(faceFormatBuffer);
			if (faceType == -1){
				printf("Faces formatting error");
				delete verts, uvs, normals;
				delete[] faces;
				return false;
			}

			if (faceType == 3){
				sscanf(faceFormatBuffer, "%d %d %d\n", &(*cFP)[0], &(*cFP)[3], &(*cFP)[6]);
			}
			else if (faceType == 6){
				sscanf(faceFormatBuffer, "%d/%d %d/%d %d/%d\n", &(*cFP)[0], &(*cFP)[1], &(*cFP)[3], &(*cFP)[4], &(*cFP)[6], &(*cFP)[7]);
			}
			else if (faceType == 9){
				sscanf(faceFormatBuffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &(*cFP)[0], &(*cFP)[1], &(*cFP)[2], &(*cFP)[3], &(*cFP)[4], &(*cFP)[5], &(*cFP)[6], &(*cFP)[7], &(*cFP)[8]);
			}
			else if (faceType == 4){
				sscanf(faceFormatBuffer, "%d %d %*d %d\n", &(*cFP)[0], &(*cFP)[3], &(*cFP)[6]);
				++currentFace;
				cFP = &faces[currentFace];
				memset(*cFP, 0, 9*sizeof(int));
				sscanf(faceFormatBuffer, "%*d %d %d %d\n", &(*cFP)[0], &(*cFP)[3], &(*cFP)[6]);
			}
			else if (faceType == 8){
				sscanf(faceFormatBuffer, "%d/%d %d/%d %*d/%*d %d/%d\n", &(*cFP)[0], &(*cFP)[1], &(*cFP)[3], &(*cFP)[4], &(*cFP)[6], &(*cFP)[7]);
				++currentFace;
				cFP = &faces[currentFace];
				memset(*cFP, 0, 9*sizeof(int));
				sscanf(faceFormatBuffer, "%*d/%*d %d/%d %d/%d %d/%d\n", &(*cFP)[0], &(*cFP)[1], &(*cFP)[3], &(*cFP)[4], &(*cFP)[6], &(*cFP)[7]);
			}
			else if (faceType == 12){
				sscanf(faceFormatBuffer, "%d/%d/%d %d/%d/%d %*d/%*d/%*d %d/%d/%d\n", &(*cFP)[0], &(*cFP)[1], &(*cFP)[2], &(*cFP)[3], &(*cFP)[4], &(*cFP)[5], &(*cFP)[6], &(*cFP)[7], &(*cFP)[8]);
				++currentFace;
				cFP = &faces[currentFace];
				memset(*cFP, 0, 9*sizeof(int));
				sscanf(faceFormatBuffer, "%*d/%*d/%*d %d/%d/%d %d/%d/%d %d/%d/%d\n", &(*cFP)[0], &(*cFP)[1], &(*cFP)[2], &(*cFP)[3], &(*cFP)[4], &(*cFP)[5], &(*cFP)[6], &(*cFP)[7], &(*cFP)[8]);
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
				delete mesh->_tris;
				return false;
			}
			crntMeshTri->vertices[j] = verts[vertIndex];

			int uvIndex = faces[i][j * 3 + 1] - 1;
			if (uvIndex >= numUVs) {
				delete verts, uvs, normals;
				delete[] faces;
				delete mesh->_tris;
				return false;
			}
			crntMeshTri->uvs[j] = uvs[uvIndex];

			int normalIndex = faces[i][j * 3 + 2] - 1;
			if (normalIndex >= numNormals) {
				delete verts, uvs, normals;
				delete[] faces;
				delete mesh->_tris;
				return false;
			}
			crntMeshTri->normals[j] = normals[normalIndex];
		}
	}
	delete verts, uvs, normals;
	delete[] faces;
	return true;
}
