#include "World.h"

World::World(){
	Mesh mesh;
	Mesh mesh2;
	MeshLoader::loadMesh(&mesh, "bunny.obj");
	MeshLoader::loadMesh(&mesh2, "cube_quads.obj");
	_modeledobject2 = new ModeledObject(&mesh2, "arceus.bmp");
	_modeledobject = new ModeledObject(&mesh, "grass.bmp");

	_modeledobject2->SetScale(vec3(5,5,-5));
	_modeledobject->SetPosition(vec3(0, -1, 0));
	//TODO: fix normals for scaled objects
	_modeledobject->SetScale(vec3(1,1,1));
	//_texturedobject = new TexturedObject();
	//SaveObjectStates();

	resetKey = GLFW_KEY_SPACE;
}

World::~World(){
	delete _modeledobject;
	delete _modeledobject2;
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
	_modeledobject2->Update(deltaTime);
	ResetWorld();
}

void World::Render(const Camera& camera){
	_modeledobject->Render(camera);
	_modeledobject2->Render(camera);
	//_texturedobject->Render(camera);
}