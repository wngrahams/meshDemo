//
//  Geometry.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/24/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include <glm/glm.hpp>
#include "Geometry.h"
#include "VertexJoiner.h"

Geometry::Geometry (float *normals, float *vertices, int *indices, int numTris) {
    
    this->numTriangles = numTris;
    this->numVertices = numTris * 3;
    
    this->faceNormals = normals;
    this->vertices = vertices;
    this->indices = indices;
    
    //join vertices, then calculate normals
    joinVertices();
    
    for (int i=0; i<this->numVertices; i++) {
        std::cout << this->vertices[3*i] << std::endl;
        std::cout << this->vertices[3*i + 1] << std::endl;
        std::cout << this->vertices[3*i + 2] << std::endl;
        
        //        std::cout << faceNormals[i] << std::endl;
    }
    
    std::cout << std::endl;
    
    for (int i=0; i<this->numTriangles*3; i++) {
        std::cout << this->indices[i] << std::endl;
    }
    
    //calculate face normals before calculating vertex normals:
    this->calculateFaceNormals();
}

void Geometry::joinVertices() {
    VertexJoiner *joiner = new VertexJoiner(this->indices, this->vertices, this->numTriangles*3, this->numVertices);
    this->vertices = joiner->getVertices();
    this->indices = joiner->getIndices();
    
    this->faceNormals = nullptr;
    this->numVertices = joiner->getNewNumVerts();
}

void Geometry::calculateFaceNormals() {
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
    
    glm::vec3 temp1;
    glm::vec3 temp2;
    
    glm::vec3 normal;
    
    this->faceNormals = new float[this->numTriangles * 3];
    
    for (int i=0; i<this->numTriangles; i++) {
        v0 = {this->vertices[indices[3 * i + 0] * 3 + 0],
              this->vertices[indices[3 * i + 0] * 3 + 1],
              this->vertices[indices[3 * i + 0] * 3 + 2]};
        v1 = {this->vertices[indices[3 * i + 1] * 3 + 0],
              this->vertices[indices[3 * i + 1] * 3 + 1],
              this->vertices[indices[3 * i + 1] * 3 + 2]};
        v2 = {this->vertices[indices[3 * i + 2] * 3 + 0],
              this->vertices[indices[3 * i + 2] * 3 + 1],
              this->vertices[indices[3 * i + 2] * 3 + 2]};
        
        temp1 = v1 - v0;
        
        temp2 = v2 - v1;
        
        normal = glm::cross(temp1, temp2);
        
        if (normal.length() == 0) {
            this->faceNormals[3 * i + 0] = normal.x;
            this->faceNormals[3 * i + 1] = normal.y;
            this->faceNormals[3 * i + 2] = normal.z;
            
            return;
        }
        
        normal = glm::normalize(normal);
        this->faceNormals[3 * i + 0] = normal.x;
        this->faceNormals[3 * i + 1] = normal.y;
        this->faceNormals[3 * i + 2] = normal.z;
    }
}

void Geometry::calculateVertexNormals () {
    
    
    
}








