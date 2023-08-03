#ifndef TANKTURRET_MODELS_HPP
#define TANKTURRET_MODELS_HPP

#include "../Model3D.hpp"

// tank turret
namespace models {
	class TankTurret : public Model3D{


		public:
			TankTurret(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale);

		public:
			void rotateTurret(TankTurret trueTurretRotation,float deltaTime);
			void moveTurret(glm::vec3 vecPos);

		private:
			void loadModel(const char *objectPath);
			void setupVAO();
			void setShaderValues(Shaders& CShaders);
	};
}
#endif 