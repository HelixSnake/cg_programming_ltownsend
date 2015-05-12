#include "Object.h"

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
		GLuint numObjects;
		Object* objects;
};