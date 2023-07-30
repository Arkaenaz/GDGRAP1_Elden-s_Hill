#include "Light.hpp"

using namespace models;

Light::Light(glm::vec3 vecPosition, glm::vec3 vecColor, float fAmbientStrength, glm::vec3 vecAmbientColor, float fSpecStrength, float fSpecPhong) {
	this->vecPosition = vecPosition;
	this->vecColor = vecColor;
	this->fAmbientStrength = fAmbientStrength;
	this->vecAmbientColor = vecAmbientColor;
	this->fSpecStrength = fSpecStrength;
	this->fSpecPhong = fSpecPhong;
	this->fIntensity = 1.f;
}

Light::~Light() {}

/*	
	Increases or decreases intensity
	@param fIntensify value to add onto its intensity
*/
void Light::intensify(float fIntensify) {
	this->fIntensity += fIntensify;
}

glm::vec3 Light::getPosition() {
	return this->vecPosition;
}

void Light::setPosition(glm::vec3 vecPosition) {
	this->vecPosition = vecPosition;
}

void Light::move(glm::vec3 vecMove) {
	this->vecPosition += vecMove;
}

void Light::rotateAround(glm::vec3 vecPoint, glm::vec3 vecRotate) {
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), vecPoint);
	glm::quat quatRotate = glm::quat(cos(glm::radians(vecRotate.z) / 2), glm::vec3(0.f, 0.f, (sin(glm::radians(vecRotate.z) / 2))));
	quatRotate *= glm::quat(cos(glm::radians(vecRotate.y) / 2), glm::vec3(0.f, (sin(glm::radians(vecRotate.y) / 2)), 0.f));
	quatRotate *= glm::quat(cos(glm::radians(vecRotate.x) / 2), glm::vec3((sin(glm::radians(vecRotate.x) / 2)), 0.f, 0.f));
	glm::mat4 inverse = glm::inverse(translate);
	glm::mat4 matrix = translate * glm::toMat4(quatRotate) * glm::inverse(translate);
	this->vecPosition = matrix * glm::vec4(this->vecPosition, 1.0f);
}

glm::vec3 Light::getColor() {
	return this->vecColor;
}

void Light::setColor(glm::vec3 vecColor) {
	this->vecColor = vecColor;
}

float Light::getAmbientStrength() {
	return this->fAmbientStrength;
}

glm::vec3 Light::getAmbientColor() {
	return this->vecAmbientColor;
}

float Light::getSpecStrength() {
	return this->fSpecStrength;
}

float Light::getSpecPhong() {
	return this->fSpecPhong;
}

float Light::getIntensity() {
	return this->fIntensity;
}

void Light::setIntensity(float fIntensity) {
	this->fIntensity = fIntensity;
}