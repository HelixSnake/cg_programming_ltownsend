#include "World.h"

World::World(){
	Mesh mesh;
	MeshLoader::loadMesh(&mesh, "arceus.obj");
	_modeledobject = new ModeledObject(&mesh);
	//_texturedobject = new TexturedObject();
	//SaveObjectStates();

	resetKey = GLFW_KEY_SPACE;
}

World::~World(){
	delete _modeledobject;
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
	_modeledobject->Update(deltaTime);
	ResetWorld();
}

void World::Render(const Camera& camera){
	_modeledobject->Render(camera);
	//_texturedobject->Render(camera);
}