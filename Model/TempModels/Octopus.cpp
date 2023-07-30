#include "Octopus.hpp"

using namespace models;

Octopus::Octopus(std::string strObjectPath, glm::vec3 vecPosition, glm::vec3 vecScale) : Model3D(vecPosition, vecScale) {
    this->loadModel(strObjectPath.c_str());
    this->setupVAO();
}

void Octopus::loadModel(const char* objectPath) {
    //this->fullVertexData.clear();

    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shape, &material, &warning, &error, objectPath);

    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        this->meshIndices.push_back(shape[0].mesh.indices[i].vertex_index);
    }

    for (int j = 0; j < shape.size(); j++) {
        switch (j) {
        case 0://1 4 5 6 8 11 12 14
        case 2:
        case 3:
        case 7:
        case 9:
            //case 10:
        case 13:

            std::vector<glm::vec3> tangents;
            std::vector<glm::vec3> bitangents;

            for (int i = 0; i < shape[j].mesh.indices.size(); i += 3) {
                tinyobj::index_t vData1 = shape[j].mesh.indices[i];
                tinyobj::index_t vData2 = shape[j].mesh.indices[i + 1];
                tinyobj::index_t vData3 = shape[j].mesh.indices[i + 2];

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

            for (int i = 0; i < shape[j].mesh.indices.size(); i++) {
                tinyobj::index_t vData = shape[j].mesh.indices[i];
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
    }
    this->nVertexValues = 14;
}

void Octopus::setupVAO() {
    GLuint VBO;
    
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->fullVertexData.size(), this->fullVertexData.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, this->nVertexValues * sizeof(GLfloat), (void*)0);
    
    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, this->nVertexValues * sizeof(float), (void*)normPtr);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, this->nVertexValues * sizeof(float), (void*)uvPtr);
    
    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, this->nVertexValues * sizeof(float), (void*)tangentPtr);
    
    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, this->nVertexValues * sizeof(float), (void*)bitangentPtr);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Octopus::setShaderValues(Shaders& CShaders) {
    CShaders.setFloatMat4("transform", this->getTransformation());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->getTexture(0));
    CShaders.setInt("tex0", 0);
    CShaders.setFloatVec3("objColor", this->getColor());
}