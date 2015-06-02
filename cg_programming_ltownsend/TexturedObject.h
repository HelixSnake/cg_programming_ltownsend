#include "Application.h"
#include "Object.h"

#pragma once

class TexturedObject : public Object{

	public:
		TexturedObject();

		virtual ~TexturedObject();
		virtual void Update(const float& deltaTime);
		virtual void Render(const Camera& camera);

		void SetPosition(vec3 position);
		void SetScale(vec3 scale);
		void SetTexture(GLuint unTextureID);

		GLuint GetTexture();

		vec3 GetPosition();
		float GetLeftX();
		float GetRightX();
		float GetTopY();
		float GetBottomY();

		void SaveObjectState(char *message = "Saved Object State");
		void LoadObjectState(char *message = "Loaded Object State");
		void LoadTriangles(const GLuint& perRow, const GLuint& perColumn, const GLenum& renderMode);

	protected:
		TexturedObject* objectState;

	private:
		float leftX, rightX, topY, bottomY;
		mat4 Render();
		GLuint numIndices;
		GLuint vertexBufferID;
		GLuint uvBufferID;
		GLenum renderMode;
		GLuint textureID;
};