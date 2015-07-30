#include "Application.h"
#include "Mesh.h"
#include "TextureStore.h"

#pragma once

class ModeledObject{

	public:
		ModeledObject(Mesh* mesh);

		virtual ~ModeledObject();
		virtual void Update(const float& deltaTime);
		virtual void Render(const Camera& camera);

		void SetPosition(vec3 position);
		void SetScale(vec3 scale);

		vec3 GetPosition();
		float GetLeftX();
		float GetRightX();
		float GetTopY();
		float GetBottomY();

		void SaveObjectState(char *message = "Saved Object State");
		void LoadObjectState(char *message = "Loaded Object State");
		void LoadTriangles();

	protected:
		ModeledObject* objectState;
		vec3 position, scale;

	private:
		Mesh* loadedMesh;
		float leftX, rightX, topY, bottomY;
		mat4 Render();
		GLuint numVertInds;
		GLuint numUVInds;
		GLuint uvBufferID;
		GLuint numNormalInds;
		GLuint vertexBufferID;
		GLenum renderMode;
		GLuint textureID;
};