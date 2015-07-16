#include "Application.h"

#pragma once

class CubeObject{

	public:
		CubeObject();

		virtual ~CubeObject();
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
		void LoadTriangles(const GLuint& perRow, const GLuint& perColumn, const GLenum& renderMode);

	protected:
		CubeObject* objectState;
		vec3 position, scale;
		float rotatex;
		float rotatey;

	private:
		float leftX, rightX, topY, bottomY;
		mat4 Render();
		GLuint numIndices;
		GLuint vertexBufferID;
		GLuint colorBufferID;
		GLenum renderMode;
};