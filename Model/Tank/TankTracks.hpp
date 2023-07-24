#ifndef TANKTRACKS_MODELS_HPP
#define TANKTRACKS_MODELS_HPP

#include "../Model3D.hpp"


namespace models {
	class TankTracks : public Model3D {


		public:
		TankTracks(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale);

		private:
		void loadModel(const char *objectPath);
		void setupVAO();
		void setShaderValues(Shaders &CShaders);
	};
}
#endif 