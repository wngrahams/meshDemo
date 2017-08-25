//
//  VertexJoiner.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/25/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#ifndef __meshReduction__VertexJoiner__
#define __meshReduction__VertexJoiner__

#include <stdio.h>
#include <glm/glm.hpp>

class VertexJoiner {
public:
    VertexJoiner (int *indices, float *vertices, int numIndices, int numVertices);
    
    ~VertexJoiner ();
    
    float* getVertices () {return this->newVertices;}
    int* getIndices () {return this->newIndices;}
    
protected:
    float *newVertices;
    float *oldVertices;
    int *newIndices;
};

class VertexDummyArray {
public:
    VertexDummyArray (int i) {this->idx = i;}
    
private:
    int idx;
    int parent;
};

inline bool operator==(const VertexDummyArray& lhs, const VertexDummyArray& rhs){ /* do actual comparison */ }
inline bool operator!=(const VertexDummyArray& lhs, const VertexDummyArray& rhs){return !operator==(lhs,rhs);}
inline bool operator< (const VertexDummyArray& lhs, const VertexDummyArray& rhs){ /* do actual comparison */ }
inline bool operator> (const VertexDummyArray& lhs, const VertexDummyArray& rhs){return  operator< (rhs,lhs);}
inline bool operator<=(const VertexDummyArray& lhs, const VertexDummyArray& rhs){return !operator> (lhs,rhs);}
inline bool operator>=(const VertexDummyArray& lhs, const VertexDummyArray& rhs){return !operator< (lhs,rhs);

#endif /* defined(__meshReduction__VertexJoiner__) */
