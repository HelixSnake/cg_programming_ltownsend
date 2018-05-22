#include "World.h"

AdvModeledObject* _modeledobject[] = {nullptr};

World::World(){
	spaceEdgeDetect = false;
	renderToggle = false;
	_numDirLights = 0;
	_numPointLights = 0;
	for (int i = 0; i < NUM_OBJECTS; i++){
		_modeledobject[i] = nullptr;
	}
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
	Mesh mesh;
	//Mesh mesh2;
	//Mesh mesh3;
	MeshLoader::loadMesh(&mesh, "cube.obj");
	_modeledobject[0] = new AdvModeledObject(&mesh);

	for (int i = 0; i < NUM_OBJECTS; i++){
		if (_modeledobject[i] == nullptr) continue;
		_modeledobject[i]->SetTexture("DiffuseMap", "white.bmp");
		_modeledobject[i]->SetTexture("SpecularMap", "white.bmp");
		_modeledobject[i]->SetTexture("NormalMap", "blankNormal.bmp");
		_modeledobject[i]->SetTexture("EmissiveMap", "white.bmp");
		_modeledobject[i]->SetTexture("TransparencyMap", "white.bmp");
	}

	_modeledobject[0]->SetTexture("DiffuseMap", "black.bmp");
	//_modeledobject[0]->SetTexture("SpecularMap", "EarthSpec.bmp");
	//_modeledobject[0]->SetTexture("NormalMap", "EarthNormal.bmp");
	_modeledobject[0]->SetTexture("EmissiveMap", "Cubefaces.bmp");

	_modeledobject[0]->SetFloatVar("normalMapHeightMult", 5.0);
	//_modeledobject[0]->SetVec3Var("objectSpecColor", vec3(0.6, 0.6, 0.4));
	_modeledobject[0]->SetVec3Var("objectEmissiveColor", vec3(1, 1, 1));
	_modeledobject[0]->SetFloatVar("objectGlow", 1.0);
	_modeledobject[0]->SetFloatVar("objectSpecFalloff", 60.0);
	_modeledobject[0]->SetVec3Var("objectSpecColor", vec3(0.0, 0.0, 0.0));
	_modeledobject[0]->SetFloatVar("objectSpecIntensity", 0);
	
	_modeledobject[0]->SetPosition(vec3(0, 0, 0));
	//TODO: fix normals for scaled objects
	_modeledobject[0]->SetScale(vec3(1,1,1));
	//SaveObjectStates();

	resetKey = GLFW_KEY_Q;
}

World::~World(){
	for (int i = 0; i < NUM_OBJECTS; i++){
		if (_modeledobject[i] != nullptr)
		{
			delete _modeledobject[i];
		}
	}
}

void World::ApplyShaders(){
	_modeledobject[0]->SetShaderSetID(this->LoadShaderSet("advShader"));
	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		if (_modeledobject[i] == nullptr) continue;
		_modeledobject[i]->SendUniformVariable(&_camera->projMatrixID, "Proj");
		_modeledobject[i]->SendUniformVariable(&_camera->MVMatrixID, "MV");
		_modeledobject[i]->SendUniformVariable(&_camera->MVPMatrixID, "MVP");
		_modeledobject[i]->SendUniformVariable(&_camera->viewMatrixID, "View");
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
	if (glfwGetKey(window, GLFW_KEY_SPACE) && !spaceEdgeDetect){
		renderToggle = !renderToggle;
	}
	spaceEdgeDetect = glfwGetKey(window, GLFW_KEY_SPACE);

	float earthTurnSpeed = 20;
	if (glfwGetKey(window, GLFW_KEY_KP_ADD)) earthTurnSpeed = 400;
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT)) earthTurnSpeed = 5;
	_modeledobject[0]->Update(deltaTime);
	ResetWorld();
}

void World::Render(const Camera& camera){
	_numDirLights = _dirlights.size();
	_numPointLights = _pointlights.size();
	for (int i = 0; i < NUM_OBJECTS; i++){
		if (i == 2 || i == 4){
			if (!renderToggle) continue;
		}
		else if (renderToggle) continue;
		if (_modeledobject[i] == nullptr) continue;
		_modeledobject[i]->LoadMaterial();
		_camera->setValuesIntoIDs();
		_modeledobject[i]->SetUniformVariable("numdirlights", _numDirLights);
		_modeledobject[i]->SetUniformVariable("numpointlights", _numPointLights);
		for (int j = 0; j < _numDirLights; j++){
			string str1 = string("dirlights[");
			_modeledobject[i]->SetUniformVariable(str1 + to_string(j) + "].direction", _dirlights[j].direction);
			_modeledobject[i]->SetUniformVariable(str1 + to_string(j) + "].diffuse", _dirlights[j].diffuseColor);
			_modeledobject[i]->SetUniformVariable(str1 + to_string(j) + "].specular", _dirlights[j].specColor);
			_modeledobject[i]->SetUniformVariable(str1 + to_string(j) + "].intensity", _dirlights[j].intensity);
		}
		for (int j = 0; j < _numPointLights; j++){
			string str1 = string("pointlights[");
			_modeledobject[i]->SetUniformVariable(str1 + std::to_string(j) + "].position", _pointlights[j].position);
			_modeledobject[i]->SetUniformVariable(str1 + std::to_string(j) + "].diffuse", _pointlights[j].diffuseColor);
			_modeledobject[i]->SetUniformVariable(str1 + std::to_string(j) + "].specular", _pointlights[j].specColor);
			_modeledobject[i]->SetUniformVariable(str1 + std::to_string(j) + "].intensity", _pointlights[j].intensity);
			_modeledobject[i]->SetUniformVariable(str1 + std::to_string(j) + "].attenuation", _pointlights[j].attenuation);
		}
		_modeledobject[i]->Render(camera);
	}
	//_texturedobject->Render(camera);
}