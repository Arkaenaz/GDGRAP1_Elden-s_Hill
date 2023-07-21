#include "OrthoCamera.hpp"

using namespace cameras;

OrthoCamera::OrthoCamera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp, glm::vec3 vecMin, glm::vec3 vecMax) :
	Camera(vecPosition, vecCenter, worldUp) {
	this->vecMin = vecMin;
	this->vecMax = vecMax;
}

OrthoCamera::~OrthoCamera() {}

glm::mat4 OrthoCamera::getProjection() {
	return glm::ortho(this->vecMin.x, this->vecMax.x, this->vecMin.y, this->vecMax.y, this->vecMin.z, this->vecMax.z);
}