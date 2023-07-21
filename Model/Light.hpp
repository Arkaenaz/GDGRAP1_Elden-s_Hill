#ifndef MODELS_LIGHT_HPP
#define MODELS_LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace models {
	class Light {
		protected:
			float fIntensity;
			glm::vec3 vecPosition;
			glm::vec3 vecColor;
			float fAmbientStrength;
			glm::vec3 vecAmbientColor;
			float fSpecStrength;
			float fSpecPhong;

		public:
			Light(glm::vec3 vecPosition, glm::vec3 vecColor, float fAmbientStrength, glm::vec3 vecAmbientColor, float fSpecStrength, float fSpecPhong);
			~Light();

		public:
			void intensify(float fIntensify);

		public:
			glm::vec3 getPosition();
			void setPosition(glm::vec3 vecPosition);
			glm::vec3 getColor();
			void setColor(glm::vec3 vecColor);
			float getAmbientStrength();
			glm::vec3 getAmbientColor();
			float getSpecStrength();
			float getSpecPhong();
			float getIntensity();
	};
}

#endif