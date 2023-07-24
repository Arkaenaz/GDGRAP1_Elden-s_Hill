#ifndef MODELS_OCTOPUS_HPP
#define MODELS_OCTOPUS_HPP

#include "../Model3D.hpp"


namespace models {
	class Octopus : public Model3D{
		public:
			Octopus(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale);

		private:
			void loadModel(const char *objectPath);
			void setupVAO();
			void setShaderValues(Shaders& CShaders);
	};
}
#endif 