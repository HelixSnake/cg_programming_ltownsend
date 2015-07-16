#include "World.h"

World::World(){

	_numCubes = 5;
	_cubes = new CubeObject*[_numCubes];
	for (int i = 0; i < _numCubes; i++)
	{
		_cubes[i] = new CubeObject();
		_cubes[i]->SetPosition(vec3(i*3 - (_numCubes / 5 * 3) - 3, 0, 0));
	}
	//SaveObjectStates();

	resetKey = GLFW_KEY_SPACE;
}

World::~World(){
	delete[] _cubes;
	_cubes = NULL;
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
	for (int i = 0; i < _numCubes; i++)
	{
		_cubes[i]->Update(deltaTime);
	}
	ResetWorld();
}

void World::Render(const Camera& camera){
	for (int i = 0; i < _numCubes; i++)
	{
		_cubes[i]->Render(camera);
	}
}