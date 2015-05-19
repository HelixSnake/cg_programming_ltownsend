#include "World.h"

World::World(){

	objects = new TexturedObject[numObjects = 1];
	objects[0].SetPosition(vec3(0, 0, 0));
	objects[0].SetScale(vec3(1, 1, 1));

	level = new Level("Text.txt");
	level->Print();

	SaveObjectStates();

	resetKey = GLFW_KEY_SPACE;
}

World::~World(){
	delete[] objects;
	objects = NULL;
	delete level;
	level = NULL;
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
	for(GLuint i = 0; i < numObjects; ++i){
		objects[i].Update(deltaTime);
	}
	ResetWorld();
}

void World::Render(const Camera& camera){
	for(GLuint i = 0; i < numObjects; ++i){
		objects[i].Render(camera);
	}
}