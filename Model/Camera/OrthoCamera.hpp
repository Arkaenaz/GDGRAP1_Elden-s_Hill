#ifndef CAMERAS_ORTHO_CAMERA_HPP
#define CAMERAS_ORTHO_CAMERA_HPP

#include "../Camera.hpp"

namespace cameras {
	using namespace models;
	class OrthoCamera : public Camera {
		private:
			glm::vec3 vecMin;
			glm::vec3 vecMax;
		public:
			OrthoCamera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp, glm::vec3 vecMin, glm::vec3 vecMax);
			~OrthoCamera();

		public:
			glm::mat4 getProjection();
	};
}


#endif