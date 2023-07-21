#include "Shaders.hpp"

using namespace models;

Shaders::Shaders(std::string strVertexPath, std::string strFragPath) {
    std::fstream vertSrc(strVertexPath);
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc(strFragPath);
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertShader);
    glAttachShader(this->shaderID, fragShader);

    glLinkProgram(this->shaderID);
}

Shaders::~Shaders() {

}

void Shaders::use() {
    glUseProgram(this->shaderID);
}

void Shaders::setInt(const std::string& strName, int nValue) {
    glUniform1i(glGetUniformLocation(this->shaderID, strName.c_str()), nValue);
}

void Shaders::setFloat(const std::string& strName, float fValue) {
    glUniform1f(glGetUniformLocation(this->shaderID, strName.c_str()), fValue);
}

void Shaders::setFloatVec3(const std::string& strName, glm::vec3 vecValue) {
    glUniform3fv(glGetUniformLocation(this->shaderID, strName.c_str()), 1, glm::value_ptr(vecValue));
}

void Shaders::setFloatMat4(const std::string& strName, glm::mat4 matValue) {
    glUniformMatrix4fv(glGetUniformLocation(this->shaderID, strName.c_str()), 1, GL_FALSE, glm::value_ptr(matValue));
}