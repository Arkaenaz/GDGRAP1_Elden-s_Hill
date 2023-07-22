#include "Model3D.hpp"

using namespace models;

Model3D::Model3D(std::string strObjectPath, std::string strTexturePath, glm::vec3 vecPosition, glm::vec3 vecScale) {
    //this->strPath = strPath;
    //this->mesh_indices = mesh_indices;
    //this->fullVertexData = fullVertexData;
    //this->texture = texture;

    this->loadTexture(strTexturePath.c_str());
    this->loadModel(strObjectPath);

    this->vecPosition = vecPosition;
    this->vecScale = vecScale;

    this->matTranslate = glm::mat4(1.0f);
    this->matRotate = glm::mat4(1.0f);
    this->matScale = glm::mat4(1.0f);
}

void Model3D::loadTexture(const char* strTexturePath) {
    int img_width, img_height, colorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load(strTexturePath, &img_width, &img_height, &colorChannels, 4);

    glGenTextures(1, &this->texture);
    
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // Depending on bool value, it would either choose RGB or RGBA
    //if (bAlpha)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    //else
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);
}

void Model3D::loadModel(const std::string& strObjectPath) {
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shape, &material, &warning, &error, strObjectPath.c_str());

    this->mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        this->mesh_indices.push_back(shape[0].mesh.indices[i].vertex_index);
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
    }
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
void Model3D::draw(Shaders &shader) {
    shader.use();
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
}

std::string Model3D::getPath() {
    return this->strPath;
}

std::vector<GLuint> Model3D::getMeshIndices() {
    return this->mesh_indices;
}

GLuint Model3D::getTexture() {
    return this->texture;
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