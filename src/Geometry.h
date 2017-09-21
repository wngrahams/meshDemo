//
//  Geometry.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/24/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#ifndef __meshReduction__Geometry__
#define __meshReduction__Geometry__

#include <stdio.h>
#include <iostream>

class Geometry {
public:
    
    Geometry (float *normals = nullptr, float *vertices = nullptr, int *indices = nullptr, int numTris = 0, int numVerts = 0);
    ~Geometry () {delete [] vertexNormals; delete [] faceNormals;}
    
    void joinVertices ();
    
    void calculateVertexNormals ();
    void calculateFaceNormals ();
    
    int getNumTriangles () {return this->numTriangles;}
    int getNumVertices () {return this->numVertices;}
    int getNumIndices () {return this->numTriangles * 3;}
    float* getFaceNormals () {return this->faceNormals;}
    float* getVertexNormals () {return this->vertexNormals;}
    float* getVertices () {return this->vertices;}
    int* getIndices () {return this->indices;}
    
private:
    float *faceNormals;
    float *vertices;
    int *indices;
    
    float *vertexNormals;
    
    int numTriangles;
    int numVertices;
};

#endif /* defined(__meshReduction__Geometry__) */
