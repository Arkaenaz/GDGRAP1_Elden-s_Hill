#include "TankTurret.hpp"

using namespace models;

TankTurret::TankTurret(std::string strObjectPath, std::string strTexturePath, glm::vec3 vecPosition, glm::vec3 vecScale) : Model3D(strObjectPath, strTexturePath, vecPosition, vecScale) {
    this->loadModel(strObjectPath.c_str());
    this->setupVAO();
}

void TankTurret::loadModel(const char *objectPath) {
	//this->fullVertexData.clear();


    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shape, &material, &warning, &error, objectPath);

    /*for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        this->meshIndices.push_back(shape[0].mesh.indices[i].vertex_index);
    }*/

    for(int j = 0; j < shape.size(); j++) {
        for(int i = 0; i < shape[j].mesh.indices.size(); i++) {
           

            switch(j) {
                case 0://1 4 5 6 8 11 12 14
                case 2:
                case 3:
                case 7:
                case 9:
                //case 10:
                case 13:
                    tinyobj::index_t vData = shape[j].mesh.indices[i];
                    this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
                    this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
                    this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);
                    //this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);
                    //this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);
                    //this->fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);
                    this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
                    this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
            }
            
        }
    }
}