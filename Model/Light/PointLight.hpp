#ifndef LIGHTS_POINT_LIGHT_HPP
#define LIGHTS_POINT_LIGHT_HPP

#include "../Light.hpp"

namespace lights {
	using namespace models;
	class PointLight : public Light {
		public:
			PointLight(glm::vec3 vecPosition, glm::vec3 vecColor, float fAmbientStrength, glm::vec3 vecAmbientColor, float fSpecStrength, float fSpecPhong);
			~PointLight();
	};
}

#endif