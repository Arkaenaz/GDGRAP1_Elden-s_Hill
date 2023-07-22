#ifndef MODELS_MODEL3D_HPP
#define MODELS_MODEL3D_HPP

#include <cmath>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../tiny_obj_loader.h"

#include "../stb_image.h"

#include "Shaders.hpp"


namespace models {
	class Model3D {
		protected:
			GLuint texture;
			std::vector<GLuint> meshIndices;
			std::vector<GLfloat> fullVertexData;
			GLuint VAO;

			glm::vec3 vecColor = glm::vec3(0.f, 0.f, 0.f);
			glm::vec3 vecPosition;
			glm::vec3 vecScale;
			float fAngle;
			glm::vec3 vecAxis;

			glm::mat4 matTranslate;
			glm::mat4 matScale;
			glm::mat4 matRotate;

		public:
			Model3D(std::string strObjectPath, std::string strTexturePath, glm::vec3 vecPosition, glm::vec3 vecScale);

		protected:
			void loadTexture(const char* texturePath);
			virtual void loadModel(const char* objectPath) = 0;
			void setupVAO();

		public:
			void draw(Shaders *CShaders);

		public:
			void move(glm::vec3 vecMove);
			void scale(glm::vec3 vecScale);
			void rotate(glm::vec3 vecRotate);
			void rotateAround(glm::vec3 vecPoint, glm::vec3 vecRotate);

		public:
			std::vector<GLuint> getMeshIndices();
			std::vector<GLfloat> getVertexData();
			GLuint getTexture();
			glm::mat4 getTransformation();
			glm::vec3 getPosition();
			void setPosition(glm::vec3 vecPosition);
			glm::vec3 getScale();
			void setScale(glm::vec3 vecScale);
			glm::vec3 getColor();
			void setColor(glm::vec3 vecColor);
	};
}

#endif