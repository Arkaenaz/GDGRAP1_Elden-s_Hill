#ifndef CAMERAS_PERSPECTIVE_CAMERA_HPP
#define CAMERAS_PERSPECTIVE_CAMERA_HPP

#include "../../Config/Settings.hpp"

#include "../Camera.hpp"

namespace cameras {
	using namespace models;
	class PerspectiveCamera : public Camera {
		private:
			float fFOV;
			float fZFar;

			float fZNear = 1.0f;

			float fZoom = -750.f;
			float fPitch = 0.0f;
			float fYaw = 0.f;

		public:
			PerspectiveCamera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp, float fFOV, float fZFar);
			~PerspectiveCamera();

		public:
			void updatePosition(glm::vec3 vecRotate, glm::vec3 vecPoint);
		public:
			void addYaw(float fYaw);
			void addPitch(float fPitch);
			float getYaw();
			float getPitch();
			void setPitch(float fPitch);
			glm::mat4 getProjection();
	};
}


#endif