//
//  MeshReducer.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/29/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include <float.h>
#include <algorithm>

#include "MeshReducer.h"

template<class T> void remove(std::vector<T> &vec, T t);
template<class T> bool contains(std::vector<T> &vec, T t);

MeshReducer::MeshReducer (Geometry *geom, int percent) {
    if(percent > 100)
        percent = 100;
    if (percent < 0)
        percent = 0;
    this->percentToRemove = percent;
    
    this->originalGeometry = geom;
    
    this->vertexArray = new Vertex_[geom->getNumVertices()];
    this->trianglesArray = new Triangle[geom->getNumTriangles()];
    
    loadData(geom->getVertices(), geom->getIndices());
    calculateInitialCosts();
    loadQueue();
    reduceMesh(geom);
}

void MeshReducer::loadData(float *vert, int *ind) {
    //load vertices and indices into data structures:
    this->originalVertices = new float[originalGeometry->getNumVertices() * 3];
    for (int i=0; i<originalGeometry->getNumVertices() * 3; i++)
        originalVertices[i] = vert[i];
    
    for (int i=0; i<originalGeometry->getNumVertices(); i++)
        vertexArray[i] = *new Vertex_(i);
    
    for (int i=0; i<originalGeometry->getNumTriangles(); i++) {
        trianglesArray[i] = *new Triangle(&vertexArray[ind[3 * i + 0]],
                                          &vertexArray[ind[3 * i + 1]],
                                          &vertexArray[ind[3 * i + 2]], this->originalVertices);
    }
}

void MeshReducer::calculateInitialCosts() {
    //for each edge, compute the effect on the model it would have if collapsed
    for (int i=0; i < originalGeometry->getNumVertices(); i++)
        computeCostAtVertex(&vertexArray[i]);
}

void MeshReducer::computeCostAtVertex(Vertex_ *v) {
    if (0 == v->neighbors.size()) {
        //v has no neighbors therefore costs nothing to collapse
        v->collapse = nullptr;
        v->cost = -0.01f;
        return;
    }
    
    v->cost = 1000000;
    v->collapse = nullptr;
    float cst = 0.0f;
    
    for (int i=0; i < v->neighbors.size(); i++) {
        cst = computeEdgeCost(v, &vertexArray[v->neighbors.at(i)]);
        
        if (FLT_MAX == cst) {
            v->cost = cst;
            v->collapse = nullptr;
            return;
        }
        else if (cst < v->cost) {
            v->collapse = &vertexArray[v->neighbors.at(i)];
            v->cost = cst;
        }
    }
}

float MeshReducer::computeEdgeCost(Vertex_ *u, Vertex_ *v) {
    this->tempU = {originalVertices[u->index * 3 + 0],
                   originalVertices[u->index * 3 + 1],
                   originalVertices[u->index * 3 + 2]};
    this->tempV = {originalVertices[v->index * 3 + 0],
                   originalVertices[v->index * 3 + 1],
                   originalVertices[v->index * 3 + 2]};
    
    tempV = tempV - tempU;
    
    float edgelength = tempV.length();
    float curvature = 0.0f;
    
    //find the trianlges on edge uv:
    this->sides.clear();
    this->otherTris.clear();
    for (int i=0; i < u->faces.size(); i++) {
        if (u->faces[i]->hasVertex(v))
            sides.push_back(u->faces[i]);
        else
            otherTris.push_back(u->faces[i]);
    }
    
    //if uv is a border edge, do not reduce:
    if (sides.size() < 2)
        return FLT_MAX;
    
    //use triangle facing most away from the sides to determine curvature:
    for (int i=0; i < u->faces.size(); i++) {
        float mincurv = 1;
        normU = {u->faces.at(i)->normal[0], u->faces.at(i)->normal[1], u->faces.at(i)->normal[2]};
        
        for (int j=0; j < sides.size(); j++) {
            normV = {sides.at(j)->normal[0], sides.at(j)->normal[1], sides.at(j)->normal[2]};
            float dotProduct = glm::dot(normU, normV);
            
            mincurv = std::min(mincurv, (1-dotProduct)/2.0f);
        }
        curvature = std::max(curvature, mincurv);
    }
    
    return (edgelength * curvature);
}

void MeshReducer::loadQueue() {
    for (int i=0; i < originalGeometry->getNumVertices(); i++) {
        if (vertexArray[i].cost < FLT_MAX)
            vertexQueue.push(&vertexArray[i]);
    }
}

void MeshReducer::reduceMesh(Geometry *geom) {
    int verticesToRemove = (int)(geom->getNumVertices()*3 * (((double)percentToRemove)/100.0));
    int decreaser = geom->getNumVertices()*3;
    
    while (decreaser > verticesToRemove) {
        Vertex_ *vertexToCollapse = getVertexToCollapse();
        collapse(vertexToCollapse, vertexToCollapse->collapse);
        
        decreaser -= 3;
    }
}

Vertex_* MeshReducer::getVertexToCollapse() {
    Vertex_ *remove = vertexQueue.top();
    vertexQueue.pop();
    if (vertexQueue.top()->cost < remove->cost) {
        vertexQueue.push(remove);
        remove = vertexQueue.top();
        vertexQueue.pop();
    }
    
    return remove;
}

void MeshReducer::collapse(Vertex_ *u, Vertex_ *v) {
    if (nullptr == v) {
        delete u;
        return;
    }
    
    tempNeighbors.clear();
    tempNeighbors.reserve(u->neighbors.size());
    
    tempNeighbors.insert(tempNeighbors.begin(), u->neighbors.begin(), u->neighbors.end());
    
    //delete Triangles on edge uv:
    for (long i=u->faces.size()-1; i>=0; i--) {
        if (u->faces[i]->hasVertex(v))
            delete (u->faces[i]);
    }
    
    //update remaining trianlges to have v instead of u:
    for (long i=u->faces.size()-1; i>=0; i--)
        u->faces[i]->replaceVertex(u, v);
    
    delete u;
    
    //recompute edge collapse costs for neighboring vertices:
    for (int i=0; i<tempNeighbors.size(); i++)
        computeCostAtVertex(&vertexArray[tempNeighbors.at(i)]);
}

void MeshReducer::removeVertex(Vertex_ *v) {
    while (v->neighbors.size() > 0) {
        int idxToRemove = v->index;
        remove(vertexArray[v->neighbors[v->neighbors.size() - 1]].neighbors, idxToRemove);
    }
    
    vertexArray[v->index] = NULL;
}

void MeshReducer::removeTriangle(Triangle *t) {
    t->deleted = true;
    for (int i=0; i<3; i++) {
        if(t->points.at(i) == NULL)
            remove(t->points.at(i)->faces, t);
    }
    
    for (int i=0; i<3; i++) {
        int i2 = (i+1)%3;
        if (t->points.at(i) != NULL && t->points.at(i) != NULL) {
            t->points.at(i)->removeIfNonNeighbor(t->points.at(i2));
            t->points.at(i2)->removeIfNonNeighbor(t->points.at(i));
        }
    }
}

Geometry* MeshReducer::remakeGeometry() {
    int deletedVerts = 0;
    for (int i=0; i<originalGeometry->getNumVertices(); i++) {
        if (vertexArray[i] == NULL)
            deletedVerts++;
        else
            vertexArray[i].index -= deletedVerts;
    }
    
    float *newVertices = new float[(originalGeometry->getNumVertices() - deletedVerts) * 3];
    int vertCount = 0;
    for (int i=0; i<originalGeometry->getNumVertices(); i++) {
        if (vertexArray[i] != NULL) {
            newVertices[3 * vertexArray[i].index + 0] = originalVertices[3 * i + 0];
            newVertices[3 * vertexArray[i].index + 1] = originalVertices[3 * i + 1];
            newVertices[3 * vertexArray[i].index + 2] = originalVertices[3 * i + 2];
            
            vertCount++;
        }
    }
    
    int deletedTris = 0;
    for (int i=0; i<originalGeometry->getNumTriangles(); i++) {
        if (trianglesArray[i].deleted)
            deletedTris++;
    }
    
    int *newIndices = new int[(originalGeometry->getNumTriangles() - deletedTris) * 3];
    int triCount = 0;
    for (int i=0; i<originalGeometry->getNumTriangles(); i++) {
        if (trianglesArray[i].deleted)
            continue;
        
        newIndices[3 * triCount + 0] = trianglesArray[i].points.at(0)->index;
        newIndices[3 * triCount + 1] = trianglesArray[i].points.at(1)->index;
        newIndices[3 * triCount + 2] = trianglesArray[i].points.at(2)->index;
        
        triCount++;
    }
    
    Geometry *geom = new Geometry(nullptr, newVertices, newIndices, triCount, vertCount);
    return geom;
}

//Vertex_::~Vertex_() {
//    //mabye should use deleteVertex function instead, then delete later due to t.deleted part
//    //OR set spots in Triangle array to nullptr and then get rid of Trianlge::deleted
//    while (this->neighbors.size() > 0) {
//        int idxToRemove = this->index;
//        
//    }
//}

void Vertex_::removeIfNonNeighbor(Vertex_ *v) {
    // remove v from neighbors if v is no longer a neighbor
    int idxToRemove = v->index;
    if (contains(this->neighbors, idxToRemove))
        return;
    
    for (int i=0; i<this->faces.size(); i++) {
        if (this->faces.at(i)->hasVertex(v))
            return;
    }
    
    remove(this->neighbors, idxToRemove);
}

Triangle::Triangle(Vertex_ *v0, Vertex_ *v1, Vertex_ *v2, float *origVerts) {
    points.push_back(v0);
    points.push_back(v1);
    points.push_back(v2);
    
    this->originalVertices = origVerts;
    computeNormal();
    
    for (int i=0; i<3; i++) {
        points.at(i)->faces.push_back(this);
        for (int j=0; j<3; j++) {
            if (i != j) {
                if (!contains(points.at(i)->neighbors, points.at(j)->index))
                    points.at(i)->neighbors.push_back(points.at(j)->index);
            }
        }
    }
}

//temp variables to be reused:
glm::vec3 vec0;
glm::vec3 vec1;
glm::vec3 vec2;
glm::vec3 temp1;
glm::vec3 temp2;
glm::vec3 nml;

void Triangle::computeNormal() {
    vec0 = {originalVertices[points.at(0)->index * 3 + 0],
            originalVertices[points.at(0)->index * 3 + 1],
            originalVertices[points.at(0)->index * 3 + 2]};
    vec1 = {originalVertices[points.at(1)->index * 3 + 0],
            originalVertices[points.at(1)->index * 3 + 1],
            originalVertices[points.at(1)->index * 3 + 2]};
    vec2 = {originalVertices[points.at(2)->index * 3 + 0],
            originalVertices[points.at(2)->index * 3 + 1],
            originalVertices[points.at(2)->index * 3 + 2]};
    
    temp1 = vec1 - vec0;
    temp2 = vec2 - vec1;
    
    nml = glm::cross(temp1, temp2);
    
    if (nml.length() == 0) {
        this->normal[0] = nml.x;
        this->normal[1] = nml.y;
        this->normal[2] = nml.z;
        return;
    }
    
    nml = glm::normalize(nml);
    this->normal[0] = nml.x;
    this->normal[1] = nml.y;
    this->normal[2] = nml.z;
}

bool Triangle::hasVertex(Vertex_ *v) {
    return ((*v == *points.at(0)) || (*v == *points.at(1)) || (*v == *points.at(2)));
}

void Triangle::replaceVertex(Vertex_ *vOld, Vertex_ *vNew) {
    for (int i=0; i<3; i++) {
        if (*vOld == *this->points.at(i)) {
            points.at(i) = vNew;  // do I need a copy constructor for this?
            break;
        }
    }
    
    remove(vOld->faces, this);
    vNew->faces.push_back(this);
    
    for (int i=0; i<3; i++) {
        vOld->removeIfNonNeighbor(points.at(i));
        points.at(i)->removeIfNonNeighbor(vOld);
    }
    
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (i != j) {
                if (!contains(points.at(i)->neighbors, points.at(j)->index))
                    points.at(i)->neighbors.push_back(points.at(j)->index);
            }
        }
    }
    
    computeNormal();
}

template<class T> void remove(std::vector<T> &v, T t) {
    v.erase(std::remove(v.begin(), v.end(), t), v.end());
}

template<class T> bool contains(std::vector<T> &v, T t) {
    return (std::find(v.begin(), v.end(), t) != v.end());
}

