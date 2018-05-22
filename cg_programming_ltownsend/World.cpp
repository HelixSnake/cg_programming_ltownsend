#include "World.h"

AdvModeledObject* _modeledobject[] = {nullptr};

World::World(){
	spaceEdgeDetect = false;
	renderToggle = false;
	_numDirLights = 0;
	_numPointLights = 0;
	//_dirlights.push_back(DirectionLight());
	//_dirlights[0].direction = vec3(0, 0, -1);
	//_dirlights[0].diffuseColor = vec3(0.9, 0.9, 0.9);
	_pointlights.push_back(PointLight());
	//_pointlights.push_back(PointLight());
	_pointlights[0].position = vec3(0, 0, 1000);
	//_pointlights[1].position = vec3(0, 2, 0);
	_pointlights[0].diffuseColor = vec3(1, 1, 0.9);
	_pointlights[0].specColor = vec3(1, 1, 0.7);
	_pointlights[0].intensity = 1.2;
	//_pointlights[1].diffuseColor = vec3(0, 1, 0);
	_pointlights[0].attenuation = vec3(1, 0, 0);
	//_pointlights[1].attenuation = vec3(1, 0, 1);
	MeshLoader::loadMesh(&_motifCubeMesh, "cube.obj");

	resetKey = GLFW_KEY_Q;
	_iterations = 0;
}

World::~World() {
	for (AdvModeledObject* obj : _motifCubes) {
		if (obj != nullptr)
		{
			delete obj;
		}
	}
}

void World::ApplyShader(AdvModeledObject* obj){
		if (obj == nullptr) return;
		obj->SetShaderSetID(this->LoadShaderSet("advShader"));
		obj->SendUniformVariable(&_camera->projMatrixID, "Proj");
		obj->SendUniformVariable(&_camera->MVMatrixID, "MV");
		obj->SendUniformVariable(&_camera->MVPMatrixID, "MVP");
		obj->SendUniformVariable(&_camera->viewMatrixID, "View");
		obj->SendUniformVariable(&_camera->fwdVecID, "cameraVec");
}

void World::ApplyShaders()
{
	for (AdvModeledObject* obj : _motifCubes)
	{
		ApplyShader(obj);
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

void World::CreateDefaultMotifCubes()
{
	AdvModeledObject* motifCube1 = CreateMotifCube();
	AdvModeledObject* motifCube2 = CreateMotifCube();
	motifCube1->SetScale(glm::vec3(0.7, 0.7, 0.7));
	motifCube2->SetScale(glm::vec3(0.7, 0.7, 0.7));
	motifCube2->SetPosition(glm::vec3(0, 0, 1));
	motifCube2->AddRotation(glm::vec3(0, 1, 1), 30);
}

AdvModeledObject* World::CreateMotifCube()
{
	AdvModeledObject* newMotifCube = new AdvModeledObject(&_motifCubeMesh);

	newMotifCube->SetTexture("SpecularMap", "white.bmp");
	newMotifCube->SetTexture("NormalMap", "blankNormal.bmp");
	newMotifCube->SetTexture("TransparencyMap", "white.bmp");

	newMotifCube->SetTexture("DiffuseMap", "black.bmp");
	newMotifCube->SetTexture("EmissiveMap", "Cubefaces.bmp");
	newMotifCube->SetVec3Var("objectEmissiveColor", vec3(1, 1, 1));
	newMotifCube->SetFloatVar("objectGlow", 1.0);
	newMotifCube->SetFloatVar("objectSpecIntensity", 0);

	newMotifCube->SetPosition(vec3(0, 0, 0));
	newMotifCube->SetScale(vec3(1, 1, 1));

	World::ApplyShader(newMotifCube);
	_motifCubes.push_back(newMotifCube);
	return newMotifCube;
}

void World::Update(const float& deltaTime){
	if (glfwGetKey(window, GLFW_KEY_SPACE) && !spaceEdgeDetect){
		renderToggle = !renderToggle;
	}
	spaceEdgeDetect = glfwGetKey(window, GLFW_KEY_SPACE);

	if (glfwGetKey(window, GLFW_KEY_INSERT) && !addMotifEdgeDetect) {
		//World::CreateMotifCube();
	}
	addMotifEdgeDetect = glfwGetKey(window, GLFW_KEY_INSERT);
	//_modeledobject[0]->Update(deltaTime);
	ResetWorld();
}

void World::Render(const Camera& camera){
	_numDirLights = _dirlights.size();
	_numPointLights = _pointlights.size();
	for (AdvModeledObject* obj : _motifCubes) {
		//if (!renderToggle) continue;
		if (obj == nullptr) continue;
		obj->LoadMaterial();
		_camera->setValuesIntoIDs();
		obj->SetUniformVariable("numdirlights", _numDirLights);
		obj->SetUniformVariable("numpointlights", _numPointLights);
		for (int j = 0; j < _numDirLights; j++) {
			string str1 = string("dirlights[");
			obj->SetUniformVariable(str1 + to_string(j) + "].direction", _dirlights[j].direction);
			obj->SetUniformVariable(str1 + to_string(j) + "].diffuse", _dirlights[j].diffuseColor);
			obj->SetUniformVariable(str1 + to_string(j) + "].specular", _dirlights[j].specColor);
			obj->SetUniformVariable(str1 + to_string(j) + "].intensity", _dirlights[j].intensity);
		}
		for (int j = 0; j < _numPointLights; j++) {
			string str1 = string("pointlights[");
			obj->SetUniformVariable(str1 + std::to_string(j) + "].position", _pointlights[j].position);
			obj->SetUniformVariable(str1 + std::to_string(j) + "].diffuse", _pointlights[j].diffuseColor);
			obj->SetUniformVariable(str1 + std::to_string(j) + "].specular", _pointlights[j].specColor);
			obj->SetUniformVariable(str1 + std::to_string(j) + "].intensity", _pointlights[j].intensity);
			obj->SetUniformVariable(str1 + std::to_string(j) + "].attenuation", _pointlights[j].attenuation);
		}
		obj->Render(camera);
	}
	//_texturedobject->Render(camera);
}