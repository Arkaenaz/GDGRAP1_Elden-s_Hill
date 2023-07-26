#include "PerspectiveCamera.hpp"

using namespace cameras;

PerspectiveCamera::PerspectiveCamera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp, float fFOV, float fZFar) :
	Camera(vecPosition, vecCenter, worldUp) {
	this->fFOV = fFOV;
	this->fZFar = fZFar;
}

PerspectiveCamera::~PerspectiveCamera() {}

void PerspectiveCamera::updatePosition(glm::vec3 vecRotate, glm::vec3 vecPoint) {
	float fX = this->fZoom * cos(glm::radians(this->fPitch));
	float fY = this->fZoom * sin(glm::radians(this->fPitch));

	float fAngle = vecRotate.y + this->fYaw;
	float fXOffset = fX * sin(glm::radians(fAngle));
	float fZOffset = fX * cos(glm::radians(fAngle));

	this->vecPosition.x = vecPoint.x - fXOffset;
	this->vecPosition.y = vecPoint.y + fY;
	this->vecPosition.z = vecPoint.z - fZOffset;

	//this->fYaw = 180 - vecRotate.y + this->fPitch;
	glm::vec3 direction;
	direction.x = cos(glm::radians(this->fYaw)) * cos(glm::radians(this->fPitch));
	direction.y = sin(glm::radians(this->fPitch));
	direction.z = sin(glm::radians(this->fYaw)) * cos(glm::radians(this->fPitch));

	//this->vecCenter = this->vecPosition + glm::normalize(direction);
}

void PerspectiveCamera::addYaw(float fYaw) {
	this->fYaw += fYaw;
}

void PerspectiveCamera::addPitch(float fPitch) {
	this->fPitch += fPitch;
}

float PerspectiveCamera::getYaw() {
	return this->fYaw;
}

float PerspectiveCamera::getPitch() {
	return this->fPitch;
}

void PerspectiveCamera::setPitch(float fPitch) {
	this->fPitch = fPitch;
}

glm::mat4 PerspectiveCamera::getProjection() {
	return glm::perspective(glm::radians(this->fFOV), SCREEN_WIDTH / SCREEN_HEIGHT, this->fZNear, this->fZFar);
}

void PerspectiveCamera::setZoom(float fZoom) {
	this->fZoom = fZoom;
}