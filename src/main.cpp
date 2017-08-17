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
    Shader lightingShader("res/shaders/lighting.vs", "res/shaders/lighting.frag");
    Shader lampShader("res/shaders/lamp.vs", "res/shaders/lamp.frag");
    
    
    //create vertex data for drawing
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,  //each group corresponds to a face
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 0.0f,  //first three vals on each row are vertex positions
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 1.0f, //second three are normal vectors, last two are textrue coords
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f
    };
    
    //Positions of multiple cubes:
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -4.4f),
        glm::vec3(-1.8f, 3.0f, -8.5f),
        glm::vec3(1.3f, -2.0f, -2.3f),
        glm::vec3(1.5f, 2.0f, -2.4f),
        glm::vec3(1.5f, 0.2f, -1.4f),
        glm::vec3(-1.4f, 1.0f, 0.0f)
    };
    
    //create vertex buffer object (VBO) and vertex array object (VAO) and element buffer object (EBO)
    GLuint VBO, boxVAO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &VBO);
    
    
    //bind VAO
    glBindVertexArray(boxVAO);
    
    
    //bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    //create vertex position attirbute               //number of info for each line
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    
    //Normal attribute                                                     //how far in to start reading
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    
    //Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    
    //unbind VAO
    glBindVertexArray(0);
    
    
    
    //LIGHTING:
    
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);
    
    //bind VAO
    glBindVertexArray(lightVAO);
    
    
    //bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    //create vertex position attirbute               //number of info for each line
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    
    //unbind VAO
    glBindVertexArray(0);
    
    
    //lighting maps:
    GLuint diffuseMap, specularMap, emissionMap;
    glGenTextures(1, &diffuseMap);
    glGenTextures(1, &specularMap);
    glGenTextures(1, &emissionMap);
    
    int textureWidth, textureHeight;
    unsigned char *image;
    
    //diffuse map
    image = SOIL_load_image("res/images/container2.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    
    //specular map
    image = SOIL_load_image("res/images/container2_specular.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    
    //unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    lightingShader.use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

    
    glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat) SCREEN_WIDTH/ (GLfloat) SCREEN_HEIGHT, camera.getNearClippingPlane(), camera.getFarCLippingPlane());
    
    //draw loop
    while (!glfwWindowShouldClose(window)) {
        
        //light movement
//        lightPos.x -= 0.005f;
//        lightPos.z -= 0.005f;
        
        
        //detect time between frames
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        
        //check if any events were acitivated
        glfwPollEvents();
        doMovement();
        
        
        //render
        //clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //dark
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //light
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        //clear depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        lightingShader.use();
        GLint lightPosLoc = glGetUniformLocation( lightingShader.Program, "light.position");
//        GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
        GLint viewPosLoc = glGetUniformLocation( lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
//        glUniform3f(lightDirLoc, -0.2f, 1.0f, -0.3f);
        glUniform3f(viewPosLoc, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    
        
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
        
        glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);
        
        //material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
        
        
        
        glm::mat4 view;
        view = camera.getViewMatrix();
        
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        //activate and bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        
        //draw container
//        glBindVertexArray(boxVAO);
//        glm::mat4 model;
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawArrays(GL_TRIANGLES, 0, 36); //number of vertices
//        
//        glBindVertexArray(0);
        
        glm::mat4 model;
        glBindVertexArray(boxVAO);
        
        for (GLuint i=0; i<10; i++) {
            model = glm::mat4 ();
            model = glm::translate(model, cubePositions[i]);
            
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.4f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        
        lampShader.use();
        
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        //draw container
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); //num of vertices
        glBindVertexArray(0);

        //swap sceen buffers
        glfwSwapBuffers(window);
    }
    
    //Deallocate resources
    glDeleteVertexArrays(1, &boxVAO);
    glDeleteVertexArrays(1, &lightVAO);
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






