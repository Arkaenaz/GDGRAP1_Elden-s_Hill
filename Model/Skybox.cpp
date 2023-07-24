#include "Skybox.hpp"

using namespace models;

Skybox::Skybox(const std::string& rtFace, const std::string& lfFace, const std::string& upFace, const std::string& dnFace, const std::string& ftFace, const std::string& bkFace) {
	std::string skyboxFaceFiles[6] = { rtFace, lfFace, upFace, dnFace, ftFace, bkFace };
	this->load(skyboxFaceFiles);
	this->setupVAO();
}

Skybox::~Skybox() {}

void Skybox::load(std::string faceFiles[6]) {
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int width, height, colorChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(faceFiles[i].c_str(), &width, &height, &colorChannel, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }
}

void Skybox::setupVAO() {
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), &this->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), &this->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
}

void Skybox::draw(Shaders& CShaders) {
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}