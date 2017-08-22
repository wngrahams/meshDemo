//
//  QuatMath.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/21/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#ifndef __meshReduction__QuatMath__
#define __meshReduction__QuatMath__

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

glm::quat lookAt(glm::vec3 direction, glm::vec3 desiredUp);

glm::quat rotateTowards(glm::quat q1, glm::quat q2, float maxAngle);

glm::quat rotateBetweenVectors(glm::vec3 startPos, glm::vec3 endPos);

#endif /* defined(__meshReduction__QuatMath__) */
