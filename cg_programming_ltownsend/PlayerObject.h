#include "Application.h"
#include "Object.h"
#include "TextureStore.h"

#pragma once

class PlayerObject : public Object{

	public:
		PlayerObject();

		virtual ~PlayerObject();
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
		PlayerObject* objectState;

	private:
		static const int NUM_TEXTURE_IDS = 8;
		float leftX, rightX, topY, bottomY;
		mat4 Render();
		GLuint numIndices;
		GLuint vertexBufferID;
		GLuint uvBufferID;
		GLenum renderMode;
		GLuint currentTextureID;
		GLuint textureIDs[NUM_TEXTURE_IDS];
		float animTime;
};