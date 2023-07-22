#include "Model3D.hpp"

using namespace models;

Model3D::Model3D(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale) {
    this->loadModel(strObjectPath.c_str());
    this->setupVAO();

    this->vecPosition = vecPosition;
    this->vecScale = vecScale;

    this->matTranslate = glm::translate(glm::mat4(1.0f), this->vecPosition);
    this->matScale = glm::scale(glm::mat4(1.0f), this->vecScale);
    this->matRotate = glm::mat4(1.0f);
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

void Model3D::loadModel(const char* objectPath) {
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shape, &material, &warning, &error, objectPath);

    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        this->meshIndices.push_back(shape[0].mesh.indices[i].vertex_index);
    }

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shape[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shape[0].mesh.indices[i];
        tinyobj::index_t vData2 = shape[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shape[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[(vData1.texcoord_index * 2)],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[(vData2.texcoord_index * 2)],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[(vData3.texcoord_index * 2)],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);
        this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);
        this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);
        this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);
        this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
        this->fullVertexData.push_back(tangents[i].x);
        this->fullVertexData.push_back(tangents[i].y);
        this->fullVertexData.push_back(tangents[i].z);
        this->fullVertexData.push_back(bitangents[i].x);
        this->fullVertexData.push_back(bitangents[i].y);
        this->fullVertexData.push_back(bitangents[i].z);
    }
}

void Model3D::setupVAO() {
    GLuint VBO;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->fullVertexData.size(), this->fullVertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)0);

    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)normPtr);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)uvPtr);

    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)tangentPtr);

    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)bitangentPtr);

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
    return this->matTranslate * this->matRotate * this->matScale;
}

/*
    Moves itself
    @param vecMove movement speed values
*/
void Model3D::move(glm::vec3 vecMove) {
    this->vecPosition += vecMove;
    this->matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(this->vecPosition.x, this->vecPosition.y, this->vecPosition.z));
}

/*
    Scales itself
    @param vecScale scale values
*/
void Model3D::scale(glm::vec3 vecScale) {
    this->vecScale += vecScale;
    this->matScale = glm::scale(glm::mat4(1.0f), glm::vec3(this->vecScale.x, this->vecScale.y, this->vecScale.z));
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
    this->matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(this->vecPosition.x, this->vecPosition.y, this->vecPosition.z));
    this->matRotate = glm::toMat4(quatRotate) * this->matRotate;
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
    this->vecPosition = quatRotate * (this->vecPosition - vecPoint) + vecPoint;
    this->matRotate = glm::toMat4(quatRotate) * this->matRotate;
    this->matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(this->vecPosition.x, this->vecPosition.y, this->vecPosition.z));
}

/*
    Draws the model
*/
void Model3D::draw(Shaders &CShaders) {
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
    this->vecPosition = vecPosition;
    this->matTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(this->vecPosition.x, this->vecPosition.y, this->vecPosition.z));
}

glm::vec3 Model3D::getScale() {
    return this->vecScale;
}

void Model3D::setScale(glm::vec3 vecScale) {
    this->vecScale = vecScale;
    this->matScale = glm::scale(glm::mat4(1.0f), glm::vec3(this->vecScale.x, this->vecScale.y, this->vecScale.z));
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