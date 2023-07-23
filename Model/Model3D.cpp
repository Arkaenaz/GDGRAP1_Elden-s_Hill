#include "Model3D.hpp"

#include "iostream"

using namespace models;

Model3D::Model3D(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale) {
    this->setupVAO();
    this->vecPosition = vecPosition;
    this->vecScale = vecScale;
    this->vecColor = glm::vec3(0.f, 0.f, 0.f);

    this->modelMatrix = glm::mat4(1.0f);
    this->modelMatrix = glm::translate(this->modelMatrix, this->vecPosition);
    this->modelMatrix = glm::scale(this->modelMatrix, this->vecScale);
}

void Model3D::addTexture(const char* texturePath) {
    int img_width, img_height, colorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load(texturePath, &img_width, &img_height, &colorChannels, 0);

    GLuint texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + this->textures.size());
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum color = GL_RGBA;
    if (colorChannels == 1)
        color = GL_RED;
    else if (colorChannels == 3)
        color = GL_RGB;
    else if (colorChannels == 4)
        color = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, color, img_width, img_height, 0, color, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    this->textures.push_back(texture);
}


void Model3D::setupVAO() {
    GLuint VBO;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->fullVertexData.size(), this->fullVertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)0);

    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)normPtr);

    GLintptr uvPtr = 3 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)uvPtr);

    GLintptr tangentPtr = 5 * sizeof(float);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)tangentPtr);

    GLintptr bitangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)bitangentPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/*
    Returns a transformation matrix
    @return transformation matrix
*/
glm::mat4 Model3D::getTransformation() {
    return this->modelMatrix;
}

/*
    Moves itself
    @param vecMove movement speed values
*/
void Model3D::move(glm::vec3 vecMove) {
    this->vecPosition += vecMove;
    this->modelMatrix = glm::translate(this->modelMatrix, vecMove);
}

/*
    Scales itself
    @param vecScale scale values
*/
void Model3D::scale(glm::vec3 vecScale) {
    this->vecScale += vecScale;
    this->modelMatrix = glm::scale(this->modelMatrix, vecScale);
}

/*
    Rotates around itself.
    Quaternion initialization used is simpler than hard coding the quaternions so it was used.
    @param vecRotate rotation angles
*/
void Model3D::rotate(glm::vec3 vecRotate) {
    glm::quat quatRotate = glm::quat(cos(glm::radians(vecRotate.z) / 2), glm::vec3(0.f, 0.f, (sin(glm::radians(vecRotate.z) / 2))));
    quatRotate *= glm::quat(cos(glm::radians(vecRotate.y) / 2), glm::vec3(0.f, (sin(glm::radians(vecRotate.y) / 2)), 0.f));
    quatRotate *= glm::quat(cos(glm::radians(vecRotate.x) / 2), glm::vec3((sin(glm::radians(vecRotate.x) / 2)), 0.f, 0.f));
    this->modelMatrix = glm::toMat4(quatRotate) * this->modelMatrix;
    //this->matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(this->vecPosition.x, this->vecPosition.y, this->vecPosition.z));
    //this->matRotate = glm::toMat4(quatRotate) * this->matRotate;
}

/*
    Rotates around a point
    @param vecPoint the point to rotate around
    @param vecRotate rotation angles
*/
void Model3D::rotateAround(glm::vec3 vecPoint, glm::vec3 vecRotate) {
    glm::quat quatRotate = glm::quat(cos(glm::radians(vecRotate.z) / 2), glm::vec3(0.f, 0.f, (sin(glm::radians(vecRotate.z) / 2))));
    quatRotate *= glm::quat(cos(glm::radians(vecRotate.y) / 2), glm::vec3(0.f, (sin(glm::radians(vecRotate.y) / 2)), 0.f));
    quatRotate *= glm::quat(cos(glm::radians(vecRotate.x) / 2), glm::vec3((sin(glm::radians(vecRotate.x) / 2)), 0.f, 0.f));
    this->modelMatrix = glm::toMat4(quatRotate) * this->modelMatrix;
    //this->vecPosition = quatRotate * (this->vecPosition - vecPoint) + vecPoint;
    //this->matRotate = glm::toMat4(quatRotate) * this->matRotate;
    //this->matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(this->vecPosition.x, this->vecPosition.y, this->vecPosition.z));
}

/*
    Draws the model
*/
void Model3D::draw(Shaders& CShaders) {
    
    CShaders.setFloatMat4("transform", this->getTransformation());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->getTexture(0));
    CShaders.setInt("tex0", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->getTexture(1));
    CShaders.setInt("norm_tex", 0);

    CShaders.setFloatVec3("objColor", this->getColor());

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 14);
}

std::vector<GLuint> Model3D::getMeshIndices() {
    return this->meshIndices;
}

GLuint Model3D::getTexture(int nIndex) {
    if (nIndex > textures.size() - 1)
        return NULL;
    return textures[nIndex];
}

glm::vec3 Model3D::getPosition() {
    return this->vecPosition;
}

void Model3D::setPosition(glm::vec3 vecPosition) {
    this->modelMatrix = glm::translate(this->modelMatrix, this->vecPosition - vecPosition);
    this->vecPosition = vecPosition;
    //this->matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(this->vecPosition.x, this->vecPosition.y, this->vecPosition.z));
}

glm::vec3 Model3D::getScale() {
    return this->vecScale;
}

void Model3D::setScale(glm::vec3 vecScale) {
    this->modelMatrix = glm::scale(this->modelMatrix, this->vecScale - vecScale);
    this->vecScale = vecScale;
    //this->matScale = glm::scale(glm::mat4(1.0f), glm::vec3(this->vecScale.x, this->vecScale.y, this->vecScale.z));
}

std::vector<GLfloat> Model3D::getVertexData() {
    return this->fullVertexData;
}

glm::vec3 Model3D::getColor() {
    return this->vecColor;
}

void Model3D::setColor(glm::vec3 vecColor) {
    this->vecColor = vecColor;
}