#include "Object.h"
#include "CubeObject.h"
#include "TexturedObject.h"
#include "PlayerObject.h"
#include "Level.h"
#include "ModeledObject.h"
#include "MeshLoader.h"

class World{

	public:
		World();
		~World();

		void SaveObjectStates();
		void LoadObjectStates();
		void ResetWorld();
		void Update(const float& deltaTime);
		void Render(const Camera& camera);
		
	private:
		int resetKey;
		ModeledObject* _modeledobject;
		TexturedObject* _texturedobject;
};