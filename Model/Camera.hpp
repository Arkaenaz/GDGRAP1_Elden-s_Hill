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

		private:
			void updateVectors();

		public:
			void move(glm::vec3 vecMove);
			void rotateAround(glm::vec3 vecPoint, glm::vec3 vecRotate);
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