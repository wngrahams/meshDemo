//
//  renderer.cpp
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

const GLint WIDTH = 800, HEIGHT = 600; //screen dimensions

int main () {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //openGL version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for mac
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //prevents window from being resized
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Mesh Reduction Demo", nullptr, nullptr);
    
    int screenWidth, screenHeight; //required for mac due to retina display
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight); //required for mac due to retina display
    std::cout << "screenWidth: " << screenWidth << ", screenHeight: " << screenHeight << std::endl;
    
    if (nullptr == window) {
        std::cout << "Failed to creat GLFW window!/n";
        glfwTerminate();
        
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE; //fixes glew bug
    
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW!/n";
        
        return -1;
    }
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
        
}



