#include "World.h"

World::World(){

	level = new Level("Text.txt");
	//level->Print();
	level->GenerateImageIDs();

	int levelHeight = level->GetHeight();
	int levelWidth = level->GetWidth();
	float levelCenterX = levelWidth / 2;
	float levelCenterY = levelHeight / 2;
	objects = new Object*[numObjects = levelHeight * levelWidth];
	for (int y = 0; y < levelHeight; ++y)
	{
		for (int x = 0; x < levelWidth; ++x)
		{
			objects[y * levelWidth + x] = new TexturedObject();
			((TexturedObject*)objects[y * levelWidth + x])->SetPosition(vec3((x - levelCenterX)*2+1, (y - levelCenterY)*-2-1, 0));
			((TexturedObject*)objects[y * levelWidth + x])->SetScale(vec3(1, 1, 1));
			((TexturedObject*)objects[y * levelWidth + x])->SetTexture(level->GetImageID(level->GetTile(x,y)));
			cout << ((TexturedObject*)objects[y * levelWidth + x])->GetTexture() << " ";
		}
		cout << endl;
	}

	//SaveObjectStates();

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
		objects[i]->Update(deltaTime);
	}
	ResetWorld();
}

void World::Render(const Camera& camera){
	for(GLuint i = 0; i < numObjects; ++i){
		objects[i]->Render(camera);
	}
}