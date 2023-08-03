#ifndef MODELS_CAMERA_HPP
#define MODELS_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace models {
	class Camera {
		protected:
			glm::vec3 vecPosition;
			glm::vec3 vecCenter;
			glm::vec3 worldUp;
			glm::vec3 vecForward;
			glm::vec3 vecRight;
			glm::vec3 vecUp;

		public:
			Camera(glm::vec3 vecPosition, glm::vec3 vecCenter, glm::vec3 worldUp);
			~Camera();

		// updates camera vectors
		protected:
			void updateVectors();

		public:
			// moves camera
			void move(glm::vec3 vecMove);
			// rotates camera around a point
			void rotateAround(glm::vec3 vecPoint, glm::vec3 vecRotate);
			// check if it can rotate around
			glm::vec3 checkRotateAround(glm::vec3 vecPoint, glm::vec3 vecRotate);

		public:
			glm::vec3 getPosition();
			void setPosition(glm::vec3 vecPos);
			void setCenter(glm::vec3 vecCenter);
			glm::mat4 getViewMatrix();
			virtual glm::mat4 getProjection();
	};
}

#endif