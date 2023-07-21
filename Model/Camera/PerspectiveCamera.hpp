#ifndef CAMERAS_PERSPECTIVE_CAMERA_HPP
#define CAMERAS_PERSPECTIVE_CAMERA_HPP

#include "../../Config/Settings.hpp"

#include "../Camera.hpp"

namespace cameras {
	using namespace models;
	class PerspectiveCamera : public Camera {
		private:
			float fFOV;
			float fZNear = 1.0f;
			float fZFar;
		public:
			PerspectiveCamera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp, float fFOV, float fZFar);
			~PerspectiveCamera();

		public:
			glm::mat4 getProjection();
	};
}


#endif