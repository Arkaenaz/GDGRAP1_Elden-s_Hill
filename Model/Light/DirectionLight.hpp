#ifndef LIGHTS_DIRECTION_LIGHT_HPP
#define LIGHTS_DIRECTION_LIGHT_HPP

#include "../Light.hpp"

namespace lights {
	using namespace models;
	class DirectionLight : public Light {
		public:
			DirectionLight(glm::vec3 vecDirection, glm::vec3 vecColor, float fAmbientStrength, glm::vec3 vecAmbientColor, float fSpecStrength, float fSpecPhong);
			~DirectionLight();
	};
}

#endif