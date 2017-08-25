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
    float *oldVertices;
    
    VertexJoiner (int *indices, float *vertices, int numIndices, int numVertices);
    
    ~VertexJoiner ();
    
    float* getVertices () {return this->newVertices;}
    int* getIndices () {return this->newIndices;}

private:
    float *newVertices;
    int *newIndices;
};

struct DummyVertex {
    int idx;
    int parent;
    VertexJoiner *joiner;
    
    DummyVertex (int i = -1, VertexJoiner *join = nullptr) {this->idx = i; this->joiner = join;}
};

inline bool operator==(const DummyVertex& lhs, const DummyVertex& rhs){
    if (lhs.joiner->oldVertices[3 * lhs.idx] == rhs.joiner->oldVertices[3 * rhs.idx] &&
        lhs.joiner->oldVertices[3 * lhs.idx + 1] == rhs.joiner->oldVertices[3 * rhs.idx + 1] &&
        lhs.joiner->oldVertices[3 * lhs.idx + 2] == rhs.joiner->oldVertices[3 * rhs.idx + 2]) {
        return true;
    }
    
    return false;
}
inline bool operator!=(const DummyVertex& lhs, const DummyVertex& rhs){return !operator==(lhs,rhs);}
inline bool operator< (const DummyVertex& lhs, const DummyVertex& rhs){
    if (lhs.joiner->oldVertices[3 * lhs.idx] < rhs.joiner->oldVertices[3 * rhs.idx])
        return true;
    else if (lhs.joiner->oldVertices[3 * lhs.idx] > rhs.joiner->oldVertices[3 * rhs.idx])
        return false;
    else if (lhs.joiner->oldVertices[3 * lhs.idx + 1] < rhs.joiner->oldVertices[3 * rhs.idx + 1])
        return true;
    else if (lhs.joiner->oldVertices[3 * lhs.idx + 1] > rhs.joiner->oldVertices[3 * rhs.idx + 1])
        return false;
    else if (lhs.joiner->oldVertices[3 * lhs.idx + 2] < rhs.joiner->oldVertices[3 * rhs.idx + 2])
        return true;
    else
        return false;
}
inline bool operator> (const DummyVertex& lhs, const DummyVertex& rhs){return  operator< (rhs,lhs);}
inline bool operator<=(const DummyVertex& lhs, const DummyVertex& rhs){return !operator> (lhs,rhs);}
inline bool operator>=(const DummyVertex& lhs, const DummyVertex& rhs){return !operator< (lhs,rhs);}

#endif /* defined(__meshReduction__VertexJoiner__) */
