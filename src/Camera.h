//
//  Camera.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/11/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

//#ifndef meshReduction_Camera_h
//#define meshReduction_Camera_h

#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    
    UP,
    DOWN,
    LEFT,
    RIGHT
};

const GLfloat YAW =  -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f; //fov
const GLfloat CLIPPING_PLANE_NEAR = 0.1f;
const GLfloat CLIPPING_PLANE_FAR = 1000.0f;

class Camera {
    
private:
    
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    GLfloat yaw;
    GLfloat pitch;
    
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
    
    GLfloat nearClipping = CLIPPING_PLANE_NEAR;
    GLfloat farClipping = CLIPPING_PLANE_FAR;
    
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
    
public:
    
    //constructor taking vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH): front (glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    
    //constructor taking float values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch): front (glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    
    glm::mat4 getViewMatrix() {
        
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }
    
    //process input from keyboard
    void processKeyboard(Camera_Movement direction, GLfloat deltaTime) {
        
        GLfloat velocity = this->movementSpeed * deltaTime;
        
        if (UP == direction)
            this->position -= this->up * velocity;
        
        if (DOWN == direction)
            this->position += this->up * velocity;
        
        if (LEFT == direction)
            this->position += this->right * velocity;
        
        if (RIGHT == direction)
            this->position -= this->right * velocity;
        
    }
    
    //process movement input from mouse
    void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true) {
        
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;
        
        this->yaw += xOffset;
        this->pitch += yOffset;
        
        if (constrainPitch) {
            if (this->pitch > 89.0f)
                this->pitch = 89.0f;
                
            if (this->pitch < -89.0f)
                this-> pitch = -89.0f;
        }
        
        this->updateCameraVectors();
    }
    
    //process scroll input from mouse
    void processMouseScroll(GLfloat yOffset) {
        
        yOffset *= this->mouseSensitivity;
        this->position -= this->front * yOffset;
    }
    
    GLfloat getZoom() {
        return this->zoom;
    }
    
    GLfloat getNearClippingPlane () {
        return this->nearClipping;
    }
    
    GLfloat getFarCLippingPlane () {
        return this->farClipping;
    }
    
    glm::vec3 getPosition () {
        return this->position;
    }
    
    glm::vec3 getFront () {
        return this->front;
    }
};











