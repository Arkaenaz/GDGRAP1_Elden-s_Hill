#ifndef PLANE_MODELS_H
#define PLANE_MODELS_H

#include "../Model3D.hpp"
// model 3d with normals
namespace models {
	class Plane : public Model3D {
		public:
			Plane(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale);

		private:
			void loadModel(const char *objectPath);
			void setupVAO();
			void setShaderValues(Shaders &CShaders);
	};
}

#endif
