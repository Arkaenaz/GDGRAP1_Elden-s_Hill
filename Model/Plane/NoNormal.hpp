#ifndef MODELS_BUS_HPP
#define MODELS_BUS_HPP

#include "../Model3D.hpp"
// model 3d but loads no normals
namespace models {
	class NoNormal : public Model3D {
		public:
			NoNormal(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale);

		private:
		void loadModel(const char *objectPath);
		void setupVAO();
		void setShaderValues(Shaders &CShaders);

	};
}

#endif

