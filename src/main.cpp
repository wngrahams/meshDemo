//
//  main.cpp
//  meshReduction
//
//  Created by Graham Stubbs on 8/9/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#include <iostream>

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

//Shaders
#include "Shader.h"

//Camera
#include "Camera.h"

const GLint WIDTH = 800, HEIGHT = 600; //screen dimensions
int SCREEN_WIDTH, SCREEN_HEIGHT; //required for mac due to retina display

void keyCallback (GLFWwindow *window, int key, int scancode, int action, int mode);
void scrollCallback (GLFWwindow *window, double xOffset, double yOffset);
void mouseCallback (GLFWwindow *window, double xPos, double yPos);
void doMovement ();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH/2.0f;
GLfloat lastY = WIDTH/2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main () {
    
    //Initialize GLFW
    glfwInit();
    
    //set all required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //openGL version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for mac
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //prevents window from being resized
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Mesh Reduction Demo", nullptr, nullptr);
    
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); //required for mac due to retina display
    
    if (nullptr == window) {
        std::cout << "Failed to creat GLFW window!\n";
        glfwTerminate();
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    //set callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glewExperimental = GL_TRUE; //fixes glew bug
    
    //Initialize GLEW
    if (GLEW_OK != glewInit()) {
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
    
    //build and compile shader program
    Shader myShader("res/shaders/core.vs", "res/shaders/core.frag");
    
    //create vertex data for drawing
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,           //each group corresponds to a face
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,            //first three vals on each row are vertex positions
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,            //last two are normalized texture coordinates
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),

    };
    
    //create vertex buffer object (VBO) and vertex array object (VAO) and element buffer object (EBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    //bind VAO
    glBindVertexArray(VAO);
    
    //bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //create vertex position attirbute               //number of info for each line
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    //texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    //unbind VAO
    glBindVertexArray(0);
    
    //load textures
    GLuint texture;
    
    int width, height;
    glGenTextures(1, &texture);
    
    //bind texture using 2d textures
    glBindTexture(GL_TEXTURE_2D, texture);
    
    //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //load & create texture and generate mitmaps
    unsigned char *image = SOIL_load_image("res/images/Image1.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //free image data
    SOIL_free_image_data(image);
    
    //unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //draw loop
    while (!glfwWindowShouldClose(window)) {
        
        //detect time between frames
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //check if any events were acitivated
        glfwPollEvents();
        doMovement();
        
        //render
        //clear the colorbuffer
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //clear depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //bind and set textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture"), 0);
        
        //indicate which shader program to use
        myShader.use();
        
        //create view frustrum
        //fov  //aspect ratio                               //near,far clipping
        glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat) SCREEN_WIDTH/ (GLfloat) SCREEN_HEIGHT, camera.getNearClippingPlane(), camera.getFarCLippingPlane());
        //TODO: put near and far clipping in camera class
        
        //add rotation and translation matrix
        glm::mat4 model;
        glm::mat4 view;
        view = camera.getViewMatrix();
        //model = glm::rotate(model, (GLfloat) glfwGetTime() * 1.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        
        GLint modelLoc = glGetUniformLocation(myShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
        GLint projLoc = glGetUniformLocation(myShader.Program, "projection");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //draw object
        glBindVertexArray(VAO);

        for (GLuint i=0; i<10; i++) {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            
            glDrawArrays(GL_TRIANGLES, 0, 36); //36 because 2 triangles per face, 6 faces in total, 3 vertices per triangle
        }
        
        glBindVertexArray(0);
        
        //swap sceen buffers
        glfwSwapBuffers(window);
    }
    
    //Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    //Terminate GLFW and clear any resources allocated by GLFW
    glfwTerminate();
    
    return EXIT_SUCCESS;
        
}

void doMovement () {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.processKeyboard(FORWARD, deltaTime);
    
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.processKeyboard(BACKWARD, deltaTime);
    
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.processKeyboard(LEFT, deltaTime);
    
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.processKeyboard(RIGHT, deltaTime);

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






