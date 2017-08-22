//
//  QuatMath.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/21/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include "QuatMath.h"

/// Returns a quaternion that will make the object face 'direction'
/// Assumes object initially faces (0.0f, 0.0f, -1.0f)
///
/// @param direction Direction the object will face
/// @param desiredUp Object's up vector
glm::quat lookAt(glm::vec3 direction, glm::vec3 desiredUp) {
    
    //return unit quat if square length of direction is less than 0.0001
    if (length2(direction) < 0.0001f )
        return glm::quat();
    
    //compute desiredUp so that it's orthogonal to direction:
    glm::vec3 right = cross(direction, desiredUp);
    desiredUp = cross(right, direction);
    
    //find the rotation between the front of the object and the desired direction
    glm::quat rot1 = rotateBetweenVectors(glm::vec3(0.0f, 0.0f, -1.0f), direction);
    
    //rotating will have caused the up vector of the model to move, so calculate
    //requred rotation to get back to desiredUp
    glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat rot2 = rotateBetweenVectors(newUp, desiredUp);
    
    //apply rotations in reverse order
    return rot2 * rot1;
}

/// Similar to SLERP, but prevents rotation greater than maxAngle
///
/// @param q1 Initial orientation
/// @param q2 Desired orientation
/// @param maxAngle Cannot rotate more than this (radians)
glm::quat rotateTowards(glm::quat q1, glm::quat q2, float maxAngle) {
    //no rotation
    if (maxAngle < 0.0001f)
        return q1;
    
    float cosTheta = dot(q1, q2);
    
    //q1 and q2 are already equal
    if (cosTheta > 0.9999f)
        return q2;
    
    //choose the shortest path for the rotation
    if (cosTheta < 0) {
        q1 = q1 * -1.0f;
        cosTheta *= -1.0f;
    }
    
    //take arccos of cosTheta to get theta
    float theta = acos(cosTheta);
    
    if (theta < maxAngle)
        return q2;
    
    float t = maxAngle / theta;
    theta = maxAngle;
    
    glm::quat result = (glm::sin((1.0f - t) * theta) * q1 + glm::sin(t * theta) * q2) / glm::sin(theta);
    result = normalize(result);
    
    return result;
}

/// Returns a quaternion s.t. quat * startPos = endPos
///
/// @param startPos Starting position
/// @param endPos Ending position
glm::quat rotateBetweenVectors(glm::vec3 startPos, glm::vec3 endPos) {
    startPos = normalize(startPos);
    endPos = normalize(endPos);
    
    float cosTheta = dot(startPos, endPos);
    glm::vec3 rotationAxis;
    
    if (cosTheta < -1 + 0.001f) {
        //case when vectors are in opposite directions
        rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), startPos);
        if (length2(rotationAxis) < 0.01 ) //if rotationAxis and startPos are parallel, adjust rotationAxis
            rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), startPos);
        
        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(glm::radians(180.0f), rotationAxis);
    }
    
    //build rotation quaternion
    rotationAxis = cross(startPos, endPos);
    
    float w = sqrt((1 + cosTheta) * 2);
    float invw = 1 / w;
    
    return glm::quat(w * 0.5f, rotationAxis.x * invw, rotationAxis.y * invw, rotationAxis.z * invw);
}



