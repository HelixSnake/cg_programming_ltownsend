#include "Application.h"
#include "Mesh.h"
#include "TextureStore.h"
#include "Material.h"
#include <map>

#pragma once

class AdvModeledObject{

	public:
		AdvModeledObject(Mesh* mesh);

		virtual ~AdvModeledObject();
		virtual void Update(const float& deltaTime);
		virtual void LoadMaterial();
		virtual void Render(const Camera& camera);

		void SetFloatVar(string name, float value);
		void SetVec3Var(string name, vec3 value);
		void RemoveFloatVar(string name);
		void RemoveVec3Var(string name);
		
		void SetTexture(string name, string filename);
		void RemoveTexture(string name);

		void SetShaderSetID(GLuint setID);
		void SendUniformVariable(GLuint* variableAddress, string uniformString);
		void SetUniformVariable(string uniformString, vec3 value);
		void SetUniformVariable(string uniformString, float value);
		void SetUniformVariable(string uniformString, int value);
		void SetPosition(vec3 position);
		void SetScale(vec3 scale);
		void SetRotationMatrix(mat4 rotation);
		mat4 GetRotationMatrix();
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
		Material* material;

		map<string, GLuint> var_textures;
		map<string, pair<float, GLuint>> var_floats;
		map<string, pair<vec3, GLuint>> var_vec3s;
		map<string, float> var_floats_defaults;
		map<string, vec3> var_vec3s_defaults;
};