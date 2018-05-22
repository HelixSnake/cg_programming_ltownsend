#include "Object.h"
#include "CubeObject.h"
#include "TexturedObject.h"
#include "PlayerObject.h"
#include "Level.h"
#include "ModeledObject.h"
#include "AdvModeledObject.h"
#include "MeshLoader.h"
#include <vector>

class World{

	public:
		World();
		~World();

		void SaveObjectStates();
		void LoadObjectStates();
		void ResetWorld();
		void setCamera(Camera* camera) {_camera = camera;};
		void Update(const float& deltaTime);
		void Render(const Camera& camera);
		void ApplyShaders();
		void AddShaderSet(string name, GLuint programID);
		GLuint LoadShaderSet(string name);
		void SetDefaultShaderSet(GLuint shadersetID);
		
	private:
		int resetKey;
		static const int NUM_OBJECTS = 1;
		AdvModeledObject* _modeledobject[NUM_OBJECTS];
		TexturedObject* _texturedobject;
		map<string, GLuint> _shadersets;
		GLuint _defaultShaderSet;
		Camera* _camera;
		int _numDirLights;
		int _numPointLights;
		vector<DirectionLight> _dirlights;
		vector<PointLight> _pointlights;
		bool renderToggle;
		bool spaceEdgeDetect;
};