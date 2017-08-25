//
//  Geometry.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/24/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include <glm/glm.hpp>
#include "Geometry.h"

Geometry::Geometry (float *normals, float *vertices, int *indices, int numTris) {
    for (int i=0; i<numTris*3; i++) {
//        std::cout << vertices[3*i] << std::endl;
//        std::cout << vertices[3*i + 1] << std::endl;
//        std::cout << vertices[3*i + 2] << std::endl;
        
        std::cout << faceNormals[i] << std::endl;
    }
//
//    delete normals;
//    delete vertices;
//    delete indices;
    
    this->numTriangles = numTris;
    this->numVertices = numTris * 3;
//    this->normals = new float [this->numTriangles * 3];
//    this->vertices = new float [this->numTriangles * 9];
//    this->indices =  new int [this->numTriangles * 3];
    
    this->faceNormals = normals;
    this->vertices = vertices;
    this->indices = indices;
    
    //join nodes, then calculate normals
}

void Geometry::joinVertices() {
    
}

void Geometry::calculateVertexNormals () {
    this->calculateFaceNormals();
    
    
}

void Geometry::calculateFaceNormals() {
    
}