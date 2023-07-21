#ifndef MODELS_SHADERS_HPP
#define MODELS_SHADERS_HPP

#include <string>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace models {
	class Shaders {
		private:
			unsigned int shaderID;

		public:
			Shaders(std::string strVertexPath, std::string strFragPath);
			~Shaders();

		public:
			void use();

		public:
			void setInt(const std::string &strName, int nValue);
			void setFloat(const std::string& strName, float fValue);
			void setFloatVec3(const std::string& strName, glm::vec3 vecValue);
			void setFloatMat4(const std::string& strName, glm::mat4 matValue);
	};
}

#endif