#ifndef TANKTURRET_MODELS_HPP
#define TANKTURRET_MODELS_HPP

#include "../Model3D.hpp"


namespace models {
	class TankTurret : public Model3D{


		public:
			TankTurret(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale);

		private:
			void loadModel(const char *objectPath);
			void setupVAO();
			void setShaderValues(Shaders& CShaders);
	};
}
#endif 