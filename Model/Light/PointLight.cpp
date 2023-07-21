#include "PointLight.hpp"

using namespace lights;

PointLight::PointLight(glm::vec3 vecPosition, glm::vec3 vecColor, float fAmbientStrength, glm::vec3 vecAmbientColor, float fSpecStrength, float fSpecPhong) :
	Light(vecPosition, vecColor, fAmbientStrength, vecAmbientColor, fSpecStrength, fSpecPhong) {
	this->fIntensity = 1.f;
}

PointLight::~PointLight() {}

