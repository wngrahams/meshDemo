//
//  MeshReducer.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/29/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#ifndef __meshReduction__MeshReducer__
#define __meshReduction__MeshReducer__

#include <stdio.h>
#include <vector>
#include <queue>

#include <glm/glm.hpp>

#include "Geometry.h"

struct Vertex_;
struct Triangle;
class MeshReducer;

struct Vertex_ {
    int index;
    std::vector<int> neighbors;
    std::vector<Triangle*> faces;
    Vertex_ *collapse;
    float cost;
    
    MeshReducer *owner;
    
    Vertex_ (int idx = -1) {this->index = idx;}
    //~Vertex_ ();
    
    void removeIfNonNeighbor (Vertex_ *v);
    
    bool operator==(const Vertex_* rhs) const {return (this->index == rhs->index);}
    bool operator!=(const Vertex_* rhs) const {return !(this == rhs);}
    bool operator< (const Vertex_* rhs) const {return (this->cost < rhs->cost);}
    bool operator> (const Vertex_* rhs) const {return (rhs < this);}
    bool operator<=(const Vertex_* rhs) const {return !(this > rhs);}
    bool operator>=(const Vertex_* rhs) const {return !(this < rhs);}
    
};

struct Triangle {
    std::vector<Vertex_*> points;
    float normal [3];
    bool deleted = false;
    
    Triangle (Vertex_ *v0 = nullptr, Vertex_ *v1 = nullptr, Vertex_ *v2 = nullptr,
              float *originialVerts = nullptr);
    //~Triangle ();
    
    //do i need a copy constructor?
    
    void computeNormal ();
    bool hasVertex (Vertex_ *v);
    void replaceVertex (Vertex_ *vOld, Vertex_ *vNew);
    
private:
    float *originalVertices;
};

class MeshReducer {
public:
    MeshReducer (Geometry *geom, int percent = 50);
    //~MeshReducer ();
    
private:
    Triangle *trianglesArray;
    Vertex_ *vertexArray;
    float *originalVertices;
    
    std::priority_queue<Vertex_*, std::vector<Vertex_*>, std::greater<Vertex_*>> vertexQueue;
    
    int percentToRemove;
    Geometry *originalGeometry;
    
    void loadData (float *vert, int *ind);
    void calculateInitialCosts ();
    void computeCostAtVertex (Vertex_ *v);
    float computeEdgeCost (Vertex_ *u, Vertex_ *v);
    void loadQueue ();
    void reduceMesh (Geometry *geom);
    Vertex_* getVertexToCollapse ();
    void collapse (Vertex_ *u, Vertex_ *v);
    Geometry* remakeGeometry ();
    
    void removeVertex(Vertex_ *v);
    void removeTriangle(Triangle *t);
    
    //temp variables to be reused:
    glm::vec3 tempU;
    glm::vec3 tempV;
    std::vector<Triangle*> sides;
    glm::vec3 normU;
    glm::vec3 normV;
    std::vector<Triangle*> otherTris;
    std::vector<int> tempNeighbors;
};

inline bool operator==(const Vertex_& lhs, const Vertex_& rhs){
    //    std::cout << "INSIDE OVERLOADED EQUALITY OPERATOR.\n";
    return (lhs.index == rhs.index);
}
inline bool operator!=(const Vertex_& lhs, const Vertex_& rhs){return !operator==(lhs,rhs);}
inline bool operator< (const Vertex_& lhs, const Vertex_& rhs){
    //    std::cout << "INSIDE OVERLOADED LESS THAN OPERATOR.\n";
    return (lhs.cost < rhs.cost);
}
inline bool operator> (const Vertex_& lhs, const Vertex_& rhs){return  operator< (rhs,lhs);}
inline bool operator<=(const Vertex_& lhs, const Vertex_& rhs){return !operator> (lhs,rhs);}
inline bool operator>=(const Vertex_& lhs, const Vertex_& rhs){return !operator< (lhs,rhs);}

#endif /* defined(__meshReduction__MeshReducer__) */
