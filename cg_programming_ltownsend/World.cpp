#include "World.h"

World::World(){
	Mesh mesh;
	Mesh mesh2;
	MeshLoader::loadMesh(&mesh, "head2.obj");
	MeshLoader::loadMesh(&mesh2, "stanford_bunny_original.obj", true);
	_modeledobject[0] = new AdvModeledObject(&mesh, "colors.bmp", "colors.bmp", "normal01.bmp");
	_modeledobject[1] = new AdvModeledObject(&mesh2, "colors.bmp", "grasstex.bmp", "normal01.bmp");

	_modeledobject[1]->SetPosition(vec3(0, 8, 0));
	//TODO: fix normals for scaled objects
	_modeledobject[0]->SetScale(vec3(1,1,1));
	_modeledobject[1]->SetScale(vec3(20,20,20));
	//_texturedobject = new TexturedObject();
	//SaveObjectStates();

	resetKey = GLFW_KEY_SPACE;
}

World::~World(){
	delete _modeledobject[0];
	delete _modeledobject[1];
}

void World::ApplyShaders(){
	_modeledobject[0]->SetShaderSetID(this->LoadShaderSet("advShader"));
	_modeledobject[1]->SetShaderSetID(this->LoadShaderSet("iridescent"));
	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		_modeledobject[i]->SendUniformVariable(&_camera->projMatrixID, "Proj");
		_modeledobject[i]->SendUniformVariable(&_camera->MVMatrixID, "MV");
		_modeledobject[i]->SendUniformVariable(&_camera->MVPMatrixID, "MVP");
		_modeledobject[i]->SendUniformVariable(&_light->directionID, "lightDir");
		_modeledobject[i]->SendUniformVariable(&_camera->fwdVecID, "cameraVec");
	}
}

void World::AddShaderSet(string name, GLuint programID){
	_shadersets.insert(pair<string, GLuint>(name, programID));
}

GLuint World::LoadShaderSet(string name){
	if (_shadersets.find(name) == _shadersets.end()) return -1;
	return _shadersets.at(name);
}

void World::SetDefaultShaderSet(GLuint shadersetID)
{
	_defaultShaderSet = shadersetID;
}

void World::LoadObjectStates(){
}

void World::SaveObjectStates(){
}

void World::ResetWorld(){
	static bool keyPressed = false;

	if(!keyPressed && glfwGetKey(window, resetKey) == GLFW_PRESS){
		LoadObjectStates();
		keyPressed = true;
	}
	else if(keyPressed && glfwGetKey(window, resetKey) == GLFW_RELEASE){
		keyPressed = false;
	}
}

void World::Update(const float& deltaTime){
	_modeledobject[0]->Update(deltaTime);
	_modeledobject[1]->Update(deltaTime);
	//_modeledobject[0]->AddRotation(vec3(0, 1, 0), deltaTime * 100);
	ResetWorld();
}

void World::Render(const Camera& camera){
	for (int i = 0; i < NUM_OBJECTS; i++){
		_modeledobject[i]->LoadMaterial();
		glUniform3f(_light->directionID, _light->direction.x, _light->direction.y, _light->direction.z);
		glUniform3f(_camera->fwdVecID, _camera->fwdVec.x, _camera->fwdVec.y, _camera->fwdVec.z);
		glUniformMatrix4fv(_camera->projMatrixID, 1, GL_FALSE, &_camera->projectionMatrix[0][0]);
		_modeledobject[i]->Render(camera);
	}
	//_texturedobject->Render(camera);
}