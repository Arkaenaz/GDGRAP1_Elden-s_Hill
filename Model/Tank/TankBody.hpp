#ifndef TANKBODY_MODELS_HPP
#define TANKBODY_MODELS_HPP

#include "../Model3D.hpp"

// tank body model
namespace models {
	class TankBody : public Model3D{


		public:
			TankBody(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale);

		private:
			void loadModel(const char *objectPath);
			void setupVAO();
			void setShaderValues(Shaders& CShaders);
	};
}
#endif 