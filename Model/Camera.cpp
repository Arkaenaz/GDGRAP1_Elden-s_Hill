#include "Camera.hpp"

using namespace models;

Camera::Camera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp) {
	this->vecPosition = vecPosition;
	this->vecCenter = vecCenter;
	this->worldUp = worldUp;

	this->updateVectors();
}

Camera::~Camera() {}

/*
	Updates vectors.
*/
void Camera::updateVectors() {
	this->vecForward = glm::normalize(glm::vec3(this->vecCenter - this->vecPosition));
	this->vecRight = glm::normalize(glm::cross(this->vecForward, this->worldUp));
	this->vecUp = glm::normalize(glm::cross(this->vecRight, this->vecForward));
}

/*
	Moves itself
	@param vecMove movement speed values
*/
void Camera::move(glm::vec3 vecMove) {
	this->vecPosition += vecMove;
	this->updateVectors();
}

/*
	Rotates around a point
	@param vecPoint the point to rotate around
	@param vecRotate rotation angles
*/
void Camera::rotateAround(glm::vec3 vecPoint, glm::vec3 vecRotate) {
	glm::quat quatRotate = glm::quat(cos(glm::radians(vecRotate.z) / 2), glm::vec3(0.f, 0.f, (sin(glm::radians(vecRotate.z) / 2))));
	quatRotate *= glm::quat(cos(glm::radians(vecRotate.y) / 2), glm::vec3(0.f, (sin(glm::radians(vecRotate.y) / 2)), 0.f));
	quatRotate *= glm::quat(cos(glm::radians(vecRotate.x) / 2), glm::vec3((sin(glm::radians(vecRotate.x) / 2)), 0.f, 0.f));
	this->vecPosition = quatRotate * (this->vecPosition - vecPoint) + vecPoint;
	this->updateVectors();
}

glm::vec3 Camera::getPosition() {
	return this->vecPosition;
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 matPosition = glm::translate(glm::mat4(1.0f), this->vecPosition * -1.0f);
	glm::mat4 matOrientation = glm::mat4(1.f);
	glm::mat4 matView;

	matOrientation[0][0] = this->vecRight.x;
	matOrientation[1][0] = this->vecRight.y;
	matOrientation[2][0] = this->vecRight.z;

	matOrientation[0][1] = this->vecUp.x;
	matOrientation[1][1] = this->vecUp.y;
	matOrientation[2][1] = this->vecUp.z;

	matOrientation[0][2] = -this->vecForward.x;
	matOrientation[1][2] = -this->vecForward.y;
	matOrientation[2][2] = -this->vecForward.z;

	matView = matOrientation * matPosition;

	return matView;
}

glm::mat4 Camera::getProjection() {
	return glm::mat4(1.0f);
}