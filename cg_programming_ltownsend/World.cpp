#include "World.h"

World::World(){
	Mesh mesh;
	Mesh mesh2;
	MeshLoader::loadMesh(&mesh, "sphere.obj");
	MeshLoader::loadMesh(&mesh2, "sphere.obj");
	_modeledobject[0] = new AdvModeledObject(&mesh, "EarthTex.bmp", "EarthSpec.bmp", "EarthNormal.bmp");
	_modeledobject[1] = new AdvModeledObject(&mesh2, "MoonTex.bmp", "MoonSpec.bmp", "MoonNormal.bmp");
	_modeledobject[2] = new AdvModeledObject(&mesh, "EarthTex.bmp", "MoonSpec.bmp", "MoonNormal.bmp");
	_modeledobject[0]->AddFloatVar("objectSpecFalloff", 40.0);
	_modeledobject[0]->AddFloatVar("normalMapHeightMult", 2.0);
	_modeledobject[1]->AddFloatVar("normalMapHeightMult", 2.0);
	_modeledobject[0]->AddVec3Var("objectSpecColor", vec3(0.6, 0.6, 0.4));
	_modeledobject[2]->AddFloatVar("atmosphereThickness", 0.2);
	_modeledobject[2]->AddVec3Var("atmosphereColor", vec3(0, 0.7, 1));

	_modeledobject[1]->SetPosition(vec3(0, 3, 0));
	//TODO: fix normals for scaled objects
	_modeledobject[0]->SetScale(vec3(1,1,1));
	_modeledobject[2]->SetScale(vec3(1,1,1));
	_modeledobject[1]->SetScale(vec3(0.1,0.1,0.1));
	{
		mat4 identity = mat4(1.0);
		vec3 vec = vec3(0,0,1);
		float angle = -23;
		_modeledobject[0]->SetRotationMatrix(glm::rotate(identity, angle, vec));
	}
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
	_modeledobject[1]->SetShaderSetID(this->LoadShaderSet("advShader"));
	_modeledobject[2]->SetShaderSetID(this->LoadShaderSet("atmosphere"));
	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		_modeledobject[i]->SendUniformVariable(&_camera->projMatrixID, "Proj");
		_modeledobject[i]->SendUniformVariable(&_camera->MVMatrixID, "MV");
		_modeledobject[i]->SendUniformVariable(&_camera->MVPMatrixID, "MVP");
		_modeledobject[i]->SendUniformVariable(&_camera->viewMatrixID, "View");
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
	float angle = 23.0 / 180 * 3.14;
	vec3 axis = vec3(0,sin(angle),-cos(angle));
	_modeledobject[0]->AddRotation(vec3(0, 1, 0), deltaTime * 10);
	_modeledobject[1]->AddRotation(vec3(0, 1, 0), deltaTime * 10);
	ResetWorld();
}

void World::Render(const Camera& camera){
	for (int i = 0; i < NUM_OBJECTS; i++){
		_modeledobject[i]->LoadMaterial();
		glUniform3f(_light->directionID, _light->direction.x, _light->direction.y, _light->direction.z);
		glUniform3f(_camera->fwdVecID, _camera->fwdVec.x, _camera->fwdVec.y, _camera->fwdVec.z);
		glUniformMatrix4fv(_camera->projMatrixID, 1, GL_FALSE, &_camera->projectionMatrix[0][0]);
		glUniformMatrix4fv(_camera->viewMatrixID, 1, GL_FALSE, &_camera->viewMatrix[0][0]);
		_modeledobject[i]->Render(camera);
	}
	//_texturedobject->Render(camera);
}