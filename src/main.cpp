//
//  main.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/9/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include <iostream>
#include <math.h>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//SOIL2
#include "SOIL2/SOIL2.h"

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//other includes
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Geometry.h"
#include "BinaryStlLoader.h"

const GLint WIDTH = 800, HEIGHT = 600; //screen dimensions
int SCREEN_WIDTH, SCREEN_HEIGHT; //required for mac due to retina display

void keyCallback (GLFWwindow *window, int key, int scancode, int action, int mode);
void scrollCallback (GLFWwindow *window, double xOffset, double yOffset);
void mouseCallback (GLFWwindow *window, double xPos, double yPos);
void doMovement ();
void doRotationX (float &angleX, glm::vec3 &up);
void doRotationY (float &angle, glm::vec3 &right);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH/2.0f;
GLfloat lastY = WIDTH/2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main () {
    
    //Initialize GLFW
    glfwInit();
    
    //set all required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //openGL version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for mac
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); //prevents window from being resized
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Mesh Reduction Demo", nullptr, nullptr);
    
    
    if (window == nullptr) {
        std::cout << "Failed to creat GLFW window!\n";
        glfwTerminate();
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); //required for mac due to retina display
    
    
    //set callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    glewExperimental = GL_TRUE; //fixes glew bug
    
    //Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW!\n";
        
        return EXIT_FAILURE;
    }
    
    
    //define viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    //enable depth
    glEnable(GL_DEPTH_TEST);
    
    
    //enable alpha support for images
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

    char const *modelPath = "res/models/nanosuit.obj";
    Model loadedModel(modelPath);
    
//    Geometry geom = loadBinarySTL("res/models/001.stl");
    Geometry geom = loadBinarySTL("res/models/sphere_small.stl");
    
    Shader lightingShader("res/shaders/lighting.vs", "res/shaders/lighting.frag");
    
    GLfloat *geomData = new GLfloat[geom.getNumTriangles() * 18];
    float *vert = geom.getVertices();
    int *ind = geom.getIndices();
    float *norm = geom.getVertexNormals();
    for (int i=0; i<geom.getNumTriangles() * 3; i++) {
        geomData[6 * i + 0] = vert[ind[i] * 3 + 0];
        geomData[6 * i + 1] = vert[ind[i] * 3 + 1];
        geomData[6 * i + 2] = vert[ind[i] * 3 + 2];
        
        geomData[6 * i + 3] = norm[ind[i] * 3 + 0];
        geomData[6 * i + 4] = norm[ind[i] * 3 + 1];
        geomData[6 * i + 5] = norm[ind[i] * 3 + 2];
    }
    
    unsigned long frames = 0;
    bool line = false;
    
    float angleX = 0.0f;
    float angleY = 0.0f;
    
    //draw loop
    while (!glfwWindowShouldClose(window)) {
        
        if (frames%600 == 0 && line == false) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            line = true;
        }
        else if (frames%600 == 0 && line == true) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            line = false;
        }
        
        glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat) SCREEN_WIDTH/ (GLfloat) SCREEN_HEIGHT, camera.getNearClippingPlane(), camera.getFarCLippingPlane());
        
        //detect time between frames
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        
        //check if any events were acitivated
        glfwPollEvents();
        doMovement();
        doRotationX(angleX, loadedModel.up);
        doRotationY(angleY, loadedModel.right);
        
        //clear the colorbuffer
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //dark
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //light
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        glm::quat xQuat = glm::angleAxis(angleX, loadedModel.right);
        glm::quat yQuat = glm::angleAxis(angleY, loadedModel.up);
        
        //scale model and set position
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model *= glm::toMat4(xQuat);
        model *= glm::toMat4(yQuat);
//        model *= glm::toMat4(xQuat);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        loadedModel.draw(shader);
//        mesh.draw(shader);
        
        //swap sceen buffers
        glfwSwapBuffers(window);
        
        frames++;
        
        if (frames >= ULONG_LONG_MAX) {
            frames = 0;
        }
    }
    
    //Deallocate resources
    
    
    //Terminate GLFW and clear any resources allocated by GLFW
    glfwTerminate();
    
    
    return EXIT_SUCCESS;
    
}

void doMovement () {
    if (keys[GLFW_KEY_W])
        camera.processKeyboard(UP, deltaTime);
    
    if (keys[GLFW_KEY_S])
        camera.processKeyboard(DOWN, deltaTime);
    
    if (keys[GLFW_KEY_A])
        camera.processKeyboard(LEFT, deltaTime);
    
    if (keys[GLFW_KEY_D])
        camera.processKeyboard(RIGHT, deltaTime);
    
}

void doRotationX (float &angleX, glm::vec3 &up) {
    GLfloat velocity = camera.getMovementSpeed() * deltaTime;
    
    if (keys[GLFW_KEY_UP]) {
        angleX += velocity;
    }
    
    if (keys[GLFW_KEY_DOWN])
        angleX -= velocity;
    
    if (angleX > M_PI)
        angleX -= 2*M_PI;
    
    if (angleX < -1*M_PI)
        angleX += 2*M_PI;
    
//    std::cout << "angleX: " << angleX << std::endl;
    
    up.y = cosf(angleX);
    up.z = sinf(angleX);
    
//    std::cout << "up: " << up.x << ", " << up.y << ", " << up.z << std::endl;
}

void doRotationY (float &angleY, glm::vec3 &right) {
    GLfloat velocity = camera.getMovementSpeed() * deltaTime;
    
    if (keys[GLFW_KEY_RIGHT])
        angleY += velocity;
    
    if (keys[GLFW_KEY_LEFT])
        angleY -= velocity;
    
    if (angleY > M_PI)
        angleY -= 2*M_PI;
    
    if (angleY < -1*M_PI)
        angleY += 2*M_PI;
    
//    std::cout << "angleY: " << angleY << std::endl;
    
//    right.x = cosf(angleY)*-1;
//    right.z = sinf(angleY)*-1;
    
//    std::cout << "right: " << right.x << ", " << right.y << ", " << right.z << std::endl;
    
}

void keyCallback (GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if( key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouseCallback (GLFWwindow *window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback (GLFWwindow *window, double xOffset, double yOffset) {
    camera.processMouseScroll(yOffset);
}




