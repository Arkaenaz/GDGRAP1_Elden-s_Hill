#ifndef MODELS_SKYBOX_HPP
#define MODELS_SKYBOX_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Model3D.hpp"

#include "Drawable.hpp"

namespace models {
	class Skybox : public Drawable { 
		private:
			GLuint texture;
			GLuint VAO;
			float vertices[24] {
				-1.f, -1.f,  1.f, 
				 1.f, -1.f,  1.f,  
				 1.f, -1.f, -1.f, 
				-1.f, -1.f, -1.f,
				-1.f,  1.f,  1.f,  
				 1.f,  1.f,  1.f,   
				 1.f,  1.f, -1.f,  
				-1.f,  1.f, -1.f  
			};
			unsigned int indices[36] {
				1,2,6,
				6,5,1,

				0,4,7,
				7,3,0,

				4,5,6,
				6,7,4,

				0,3,2,
				2,1,0,

				0,1,5,
				5,4,0,

				3,7,6,
				6,2,3
			};

		public:
			Skybox(const std::string& rtFace,
				   const std::string& lfFace,
				   const std::string& upFace,
				   const std::string& dnFace,
				   const std::string& ftFace,
				   const std::string& bkFace);
			~Skybox();

		private:
			void load(std::string faceFiles[6]);
			void setupVAO();

		public:
			void draw(Shaders& CShaders);
	};
}

#endif