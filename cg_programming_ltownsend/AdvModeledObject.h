#include "Application.h"
#include "Mesh.h"
#include "TextureStore.h"
#include "Material.h"

#pragma once

class AdvModeledObject{

	public:
		AdvModeledObject(Mesh* mesh, char* texture, char* specMap, char* normMap);

		virtual ~AdvModeledObject();
		virtual void Update(const float& deltaTime);
		virtual void LoadMaterial();
		virtual void Render(const Camera& camera);

		void SetShaderSetID(GLuint setID);
		void SendUniformVariable(GLuint* variableAddress, string uniformString);
		void SetPosition(vec3 position);
		void SetScale(vec3 scale);
		void SetRotationMatrix(mat4 rotation);
		void AddRotation(vec3 axis, float angle);

		vec3 GetPosition();
		float GetLeftX();
		float GetRightX();
		float GetTopY();
		float GetBottomY();

		void SaveObjectState(char *message = "Saved Object State");
		void LoadObjectState(char *message = "Loaded Object State");
		void LoadTriangles();

	protected:
		AdvModeledObject* objectState;
		vec3 position, scale;
		mat4 rotMatrix;

	private:
		Mesh* loadedMesh;
		float leftX, rightX, topY, bottomY;
		void Render();
		GLuint numVertInds;
		GLuint numUVInds;
		GLuint uvBufferID;
		GLuint numNormalInds;
		GLuint vertexBufferID;
		GLuint normalBufferID;
		GLuint tangentBufferID;
		GLuint biTangentBufferID;
		GLenum renderMode;
		GLuint textureID;
		GLuint specMapID;
		GLuint normMapID;
		Material* material;
};