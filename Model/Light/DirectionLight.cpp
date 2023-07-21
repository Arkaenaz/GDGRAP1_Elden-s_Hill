#include "DirectionLight.hpp"

using namespace lights;

DirectionLight::DirectionLight(glm::vec3 vecDirection, glm::vec3 vecColor, float fAmbientStrength, glm::vec3 vecAmbientColor, float fSpecStrength, float fSpecPhong) :
	Light(vecDirection, vecColor, fAmbientStrength, vecAmbientColor, fSpecStrength, fSpecPhong) {
	this->fIntensity = 1.f;
}

DirectionLight::~DirectionLight() {}

