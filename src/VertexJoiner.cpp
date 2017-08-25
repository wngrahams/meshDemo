//
//  VertexJoiner.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/25/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include "VertexJoiner.h"

VertexJoiner::VertexJoiner (int *ind, float *vert, int numInd, int numVert) {
    if (nullptr == ind || nullptr == vert || numInd == 0 || numVert == 0)
        std::cout << "Indices and vertices must contain at least 1 vertex\n";
    
//    std::vector<float> oldVertices;
//    oldVertices.assign(vert, vert + numVert);
    this->oldVertices = vert;
    
    std::vector<DummyVertex> vertVec(numVert);
    std::vector<DummyVertex> vertVec2(numVert);
    
    for (int i=0; i < numVert; i++) {
        vertVec[i] = *new DummyVertex(i, this);
        vertVec2[i] = *new DummyVertex(i, this);
    }
    
    std::sort(vertVec.begin(), vertVec.end());
    int vcount = 1;
    vertVec[0].parent = 0;
    
    std::vector<int> restVerts;
    restVerts.push_back(0);
    
    for (int i=0; i < numVert - 1; i++) {
        if (vertVec[i] == vertVec[i + 1])
            vertVec[i + 1].parent = vertVec[i].parent;
        else {
            vertVec[i + 1].parent = vcount++;
            restVerts.push_back(i + 1);
        }
    }
    
    this->newVertices = new float[vcount * 3];
    this->newIndices = new int[numInd];
    
    for (int i=0; i<vcount; i++) {
        this->newVertices[3 * i + 0] = vert[vertVec[restVerts.at(i)].idx * 3 + 0];
        this->newVertices[3 * i + 1] = vert[vertVec[restVerts.at(i)].idx * 3 + 1];
        this->newVertices[3 * i + 2] = vert[vertVec[restVerts.at(i)].idx * 3 + 2];
    }
    
    for (int i=0; i<numInd; i++)
        newIndices[i] = vertVec2[ind[i]].parent;
    
    oldVertices = nullptr;
}




