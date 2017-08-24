//
//  BinaryStlLoader.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/23/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include "BinaryStlLoader.h"

Geometry loadBinarySTL (std::string filename) {
    std::ifstream stlFile(filename.c_str(), std::ios::in | std::ios::binary);
    
    if (!stlFile)
        std::cout << "File: '" << filename << "' could not be loaded\n";
    
    char buffer[HEADER_SIZE];
    stlFile.read(buffer, HEADER_SIZE);
    
    if (!stlFile)
        std::cout << "Error reading the first 80 bytes of '" << filename << "'\n";
    
    int numTriangles = 0;
    stlFile.read((char*)&numTriangles, TRIANGLE_COUNT_SIZE);
    
    if (!stlFile)
        std::cout << "Error reading number of triangles from '" << filename << "'\n";
    
//    float vertices[numTriangles * 9];
//    int indices[numTriangles * 3];
//    float normals[numTriangles * 3];
    
    float *vertices = new float[numTriangles * 9];
    int *indices = new int[numTriangles * 3];
    float *normals = new float[numTriangles * 3];
    
    for (int i=0; i<(numTriangles * 3); i++) {
//        vertices[3*i] = 0.0f;
//        vertices[3*i + 1] = 0.0f;
//        vertices[3*i + 2] = 0.0f;
//        indices[i] = 0;
//        normals[i] = 0.0f;
        
        *(vertices + i) = 0.0f;
        *(indices + i) = 0;
        *(normals + i) = 0.0f;
    }
    
    char unusedBuffer[UNUSED_BUFFER_SIZE];
    
    for (int i=0; i<numTriangles; i++) {
        
//        stlFile.read((char*)&normals[3 * i], COORD_SIZE);
//        stlFile.read((char*)&normals[3 * i + 1], COORD_SIZE);
//        stlFile.read((char*)&normals[3 * i + 2], COORD_SIZE);
        
        stlFile.read((char*)(normals + (3*i)), COORD_SIZE);
        stlFile.read((char*)(normals + (3*i + 1)), COORD_SIZE);
        stlFile.read((char*)(normals + (3*i + 2)), COORD_SIZE);
        
        for (int j=0; j<3; j++) {
            indices[3 * i + j] = 3 * i + j;
            
//            stlFile.read((char*)&vertices[9 * i + 3 * j], COORD_SIZE);
//            stlFile.read((char*)&vertices[9 * i + 3 * j + 1], COORD_SIZE);
//            stlFile.read((char*)&vertices[9 * i + 3 * j + 2], COORD_SIZE);
            
            stlFile.read((char*)(vertices + (9 * i + 3 * j)), COORD_SIZE);
            stlFile.read((char*)(vertices + (9 * i + 3 * j + 1)), COORD_SIZE);
            stlFile.read((char*)(vertices + (9 * i + 3 * j + 1)), COORD_SIZE);
        }
        
        stlFile.read(unusedBuffer, UNUSED_BUFFER_SIZE);
    }
    
    stlFile.close();
    
    return Geometry(normals, vertices, indices, numTriangles);
}



