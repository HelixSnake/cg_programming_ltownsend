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
	Mesh mesh2;
	Mesh mesh3;
	MeshLoader::loadMesh(&mesh, "sphere.obj");
	MeshLoader::loadMesh(&mesh2, "stanford_bunny.obj");
	MeshLoader::loadMesh(&mesh3, "stanford_bunny.obj", true);
	_modeledobject[0] = new AdvModeledObject(&mesh);
	_modeledobject[1] = new AdvModeledObject(&mesh);
	_modeledobject[2] = new AdvModeledObject(&mesh2);
	_modeledobject[3] = new AdvModeledObject(&mesh);
	_modeledobject[4] = new AdvModeledObject(&mesh3);
	_modeledobject[5] = new AdvModeledObject(&mesh);
	_modeledobject[6] = new AdvModeledObject(&mesh);
	_modeledobject[7] = new AdvModeledObject(&mesh);

	for (int i = 0; i < NUM_OBJECTS; i++){
		if (_modeledobject[i] == nullptr) continue;
		_modeledobject[i]->SetTexture("DiffuseMap", "white.bmp");
		_modeledobject[i]->SetTexture("SpecularMap", "white.bmp");
		_modeledobject[i]->SetTexture("NormalMap", "blankNormal.bmp");
		_modeledobject[i]->SetTexture("EmissiveMap", "white.bmp");
		_modeledobject[i]->SetTexture("TransparencyMap", "white.bmp");
	}

	_modeledobject[0]->SetTexture("DiffuseMap", "EarthTex.bmp");
	_modeledobject[0]->SetTexture("SpecularMap", "EarthSpec.bmp");
	_modeledobject[0]->SetTexture("NormalMap", "EarthNormal.bmp");
	//_modeledobject[0]->SetTexture("EmissiveMap", "EarthLum.bmp");

	_modeledobject[1]->SetTexture("DiffuseMap", "MoonTex.bmp");
	_modeledobject[1]->SetTexture("SpecularMap", "MoonSpec.bmp");
	_modeledobject[1]->SetTexture("NormalMap", "MoonNormal.bmp");
	
	_modeledobject[5]->SetTexture("DiffuseMap", "black.bmp");
	_modeledobject[5]->SetTexture("SpecularMap", "black.bmp");
	_modeledobject[5]->SetTexture("EmissiveMap", "sun.bmp");
	
	_modeledobject[3]->SetTexture("TransparencyMap", "EarthClouds.bmp");
	_modeledobject[3]->SetFloatVar("objectTransparency", 0.6);

	//_modeledobject[4]->SetTexture("DiffuseMap", "EarthTex.bmp");
	_modeledobject[2]->SetTexture("DiffuseMap", "grasstex.bmp");

	_modeledobject[0]->SetFloatVar("normalMapHeightMult", 5.0);
	_modeledobject[1]->SetFloatVar("normalMapHeightMult", 3.0);
	//_modeledobject[0]->SetVec3Var("objectSpecColor", vec3(0.6, 0.6, 0.4));
	_modeledobject[0]->SetVec3Var("objectEmissiveColor", vec3(1, 1, 1));
	_modeledobject[0]->SetFloatVar("objectSpecFalloff", 60.0);
	_modeledobject[0]->SetVec3Var("objectSpecColor", vec3(0.65, 0.65, 0.6));
	_modeledobject[0]->SetFloatVar("objectSpecIntensity", 1.9);
	//_modeledobject[0]->SetFloatVar("objectGlow", 0.3);
	_modeledobject[7]->SetFloatVar("atmosphereThickness", 0.2);
	_modeledobject[7]->SetFloatVar("atmosphereAlpha", 0.9);
	_modeledobject[7]->SetVec3Var("atmosphereColor", vec3(0, 0.7, 1));
	_modeledobject[5]->SetFloatVar("objectGlow", 1);
	_modeledobject[5]->SetVec3Var("objectEmissiveColor", vec3(1, 1, 1));
	_modeledobject[6]->SetFloatVar("atmosphereThickness", 15);
	_modeledobject[6]->SetFloatVar("atmosphereAlpha", 1.5);
	_modeledobject[6]->SetVec3Var("atmosphereColor", vec3(1, 1, 0.8));
	
	_modeledobject[0]->SetPosition(vec3(0, 0, 0));
	_modeledobject[1]->SetPosition(vec3(0, 0, 0));
	_modeledobject[3]->SetPosition(vec3(0, 0, 0));
	_modeledobject[7]->SetPosition(vec3(0, 0, 0));
	_modeledobject[4]->SetPosition(vec3(-1.6, -1, 0));
	_modeledobject[2]->SetPosition(vec3(1.6, -1, 0));
	_modeledobject[5]->SetPosition(vec3(0, 0, 1000));
	_modeledobject[6]->SetPosition(vec3(0, 0, 1000));
	//TODO: fix normals for scaled objects
	_modeledobject[0]->SetScale(vec3(1,1,1));
	_modeledobject[7]->SetScale(vec3(1,1,1));
	_modeledobject[5]->SetScale(vec3(30,30,30));
	_modeledobject[6]->SetScale(vec3(30,30,30));
	_modeledobject[3]->SetScale(vec3(1.005,1.005,1.005));
	_modeledobject[1]->SetScale(vec3(0.1,0.1,0.1));
	{
		mat4 identity = mat4(1.0);
		vec3 vec = vec3(0,0,1);
		float angle = -23;
		float angle2 = -27;
		_modeledobject[0]->SetRotationMatrix(glm::rotate(identity, angle, vec));
		_modeledobject[3]->SetRotationMatrix(glm::rotate(identity, angle2, vec));
		_modeledobject[1]->SetRotationMatrix(glm::rotate(identity, angle, vec));
	}
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
	_modeledobject[1]->SetShaderSetID(this->LoadShaderSet("advShader"));
	_modeledobject[2]->SetShaderSetID(this->LoadShaderSet("iridescent"));
	_modeledobject[3]->SetShaderSetID(this->LoadShaderSet("advShader"));
	_modeledobject[4]->SetShaderSetID(this->LoadShaderSet("toon"));
	_modeledobject[7]->SetShaderSetID(this->LoadShaderSet("atmosphere"));
	_modeledobject[5]->SetShaderSetID(this->LoadShaderSet("advShader"));
	_modeledobject[6]->SetShaderSetID(this->LoadShaderSet("atmosphere"));
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
	_modeledobject[1]->Update(deltaTime);
	float angle = 23.0 / 180 * 3.14;
	vec3 axis = vec3(0,sin(angle),-cos(angle));
	_modeledobject[0]->AddRotation(vec3(0, 1, 0), deltaTime * earthTurnSpeed);
	_modeledobject[1]->AddRotation(vec3(0, 1, 0), deltaTime * earthTurnSpeed/10);
	_modeledobject[2]->AddRotation(vec3(0, 1, 0), deltaTime * earthTurnSpeed/2);
	_modeledobject[4]->AddRotation(vec3(0, 1, 0), deltaTime * earthTurnSpeed/2);
	_modeledobject[3]->AddRotation(vec3(0, 1, 0), deltaTime * earthTurnSpeed*1.1);
	_modeledobject[1]->SetPosition(vec3(_modeledobject[1]->GetRotationMatrix() * vec4(3, 0, 0, 0)));
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