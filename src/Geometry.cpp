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

//TODO: delete any "new" arrays in the destructor

Geometry::Geometry (float *normals, float *vertices, int *indices, int numTris) {
    
    this->numTriangles = numTris;
    this->numVertices = numTris * 3;
    
    this->faceNormals = normals;
    this->vertices = vertices;
    this->indices = indices;
    
    this->vertexNormals = nullptr;
    
    //join vertices, then calculate normals
    joinVertices();
    
    //calculate face normals before calculating vertex normals:
    this->calculateFaceNormals();
    this->calculateVertexNormals();
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
    if (nullptr == this->faceNormals)
        calculateFaceNormals();
    
    this->vertexNormals = new float[this->numVertices * 3];
    
    for (int i=0; i < this->numVertices * 3; i++)
        vertexNormals[i] = 0;
    
    for (int i=0; i < this->numTriangles; i++) {
        int i1 = indices[3 * i + 0];
        int i2 = indices[3 * i + 1];
        int i3 = indices[3 * i + 2];
        
        vertexNormals[3 * i1 + 0] += faceNormals[3 * i + 0];
        vertexNormals[3 * i1 + 1] += faceNormals[3 * i + 1];
        vertexNormals[3 * i1 + 2] += faceNormals[3 * i + 2];

        vertexNormals[3 * i2 + 0] += faceNormals[3 * i + 0];
        vertexNormals[3 * i2 + 1] += faceNormals[3 * i + 1];
        vertexNormals[3 * i2 + 2] += faceNormals[3 * i + 2];
        
        vertexNormals[3 * i3 + 0] += faceNormals[3 * i + 0];
        vertexNormals[3 * i3 + 1] += faceNormals[3 * i + 1];
        vertexNormals[3 * i3 + 2] += faceNormals[3 * i + 2];
    }
    
    glm::vec3 normal;
    
    for (int i=0; i < this->numVertices; i++) {
        normal = {this->vertexNormals[3 * i + 0],
                  this->vertexNormals[3 * i + 1],
                  this->vertexNormals[3 * i + 2]};
        
        normal = glm::normalize(normal);
        
        this->vertexNormals[3 * i + 0] = normal.x;
        this->vertexNormals[3 * i + 1] = normal.y;
        this->vertexNormals[3 * i + 2] = normal.z;
    }
}








