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

//Shaders
#include "src/Shader.h"

const GLint WIDTH = 800, HEIGHT = 600; //screen dimensions

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
    
    int screenWidth, screenHeight; //required for mac due to retina display
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight); //required for mac due to retina display
//    std::cout << "screenWidth: " << screenWidth << ", screenHeight: " << screenHeight << std::endl;
    
    if (nullptr == window) {
        std::cout << "Failed to creat GLFW window!\n";
        glfwTerminate();
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE; //fixes glew bug
    
    //Initialize GLEW
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW!\n";
        
        return EXIT_FAILURE;
    }
    
    //define viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);
    
    Shader myShader("resources/shaders/core.vs", "resources/shaders/core.frag");
    
    //create vertex data for drawing
    GLfloat vertices[] = {
        //position                  //color
        -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f, //bottom left
        0.5f, -0.5f, 0.0f,          0.0f, 1.0f, 0.0f, //bottom right
        0.0f, 0.5f, 0.0f,           0.0f, 0.0f, 1.0f  //top
    };
    
    //create vertex buffer object (VBO) and vertex array object (VAO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    //bind VAO
    glBindVertexArray(VAO);
    
    //bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //create vertex pointer                         //number of values
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        //check if any events were acitivated
        glfwPollEvents();
        
        //render
        //clear the colorbuffer
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //draw object
        //indicate which shader program to use
        myShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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



