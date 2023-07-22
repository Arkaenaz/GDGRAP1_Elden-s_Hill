#ifndef TANKBODY_MODELS_HPP
#define TANKBODY_MODELS_HPP

#include "../Model3D.hpp"


namespace models {
	class TankBody : public Model3D{


		public:
		TankBody(std::string strObjectPath, std::string strTexturePath, glm::vec3 vecPosition, glm::vec3 vecScale);

		void loadModel(const char *objectPath);
	};
}
#endif 