#include "PerspectiveCamera.hpp"

using namespace cameras;

PerspectiveCamera::PerspectiveCamera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp, float fFOV, float fZFar) :
	Camera(vecPosition, vecCenter, worldUp) {
	this->fFOV = fFOV;
	this->fZFar = fZFar;
}

PerspectiveCamera::~PerspectiveCamera() {}

glm::mat4 PerspectiveCamera::getProjection() {
	return glm::perspective(glm::radians(this->fFOV), SCREEN_WIDTH / SCREEN_HEIGHT, this->fZNear, this->fZFar);
}