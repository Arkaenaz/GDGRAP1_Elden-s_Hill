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