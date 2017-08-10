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
    
    //enable depth
    glEnable(GL_DEPTH_TEST);
    
    //enable alpha support for images
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Shader myShader("res/shaders/core.vs", "res/shaders/core.frag");
    
    //create vertex data for drawing
    // Set up vertex data (and buffer(s)) and attribute pointers
    // use with Orthographic Projection
    
//     GLfloat vertices[] = {
//     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,
//     0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 0.0f,
//     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
//     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
//     -0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,
//     
//     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
//     0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
//     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,
//     -0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 1.0f,
//     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
//     
//     -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     -0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
//     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
//     -0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     
//     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
//     0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
//     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     
//     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
//     0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     -0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
//     -0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     
//     -0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f,
//     0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,
//     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.0f,
//     -0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f,
//     -0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 1.0f
//     };
    
    
    // use with Perspective Projection
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
    
    glm::mat4 projection; //projection type
    //create view frustrum
                                  //fov  //aspect ratio                               //near,far clipping
    projection = glm::perspective(45.0f, (GLfloat) screenWidth/ (GLfloat) screenHeight, 0.1f, 1000.0f);
//    projection = glm::ortho(0.0f, (GLfloat) screenWidth, 0.0f, (GLfloat) screenHeight, 0.1f, 1000.0f);
    
    while (!glfwWindowShouldClose(window)) {
        //check if any events were acitivated
        glfwPollEvents();
        
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
        
        //add rotation and translation matrix
        glm::mat4 model;
        glm::mat4 view;
        model = glm::rotate(model, (GLfloat) glfwGetTime() * 1.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//        model = glm::rotate(model, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
//        view = glm::translate(view, glm::vec3(screenWidth/2, screenHeight/2, -700.0f));
        
        GLint modelLoc = glGetUniformLocation(myShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
        GLint projLoc = glGetUniformLocation(myShader.Program, "projection");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //draw object
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); //36 because 2 triangles per face, 6 faces in total, 3 vertices per triangle
        
        glBindVertexArray(0);
        
//        //create transformation
//        glm::mat4 transfrom;
//        transfrom = glm::translate(transfrom, glm::vec3(0.5f, -0.5f, 0.0f));
//        transfrom = glm::rotate(transfrom, (GLfloat) glfwGetTime() * 8.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//        
//        GLint transfromLocation = glGetUniformLocation(myShader.Program, "transform");
//        glUniformMatrix4fv(transfromLocation, 1, GL_FALSE, glm::value_ptr(transfrom));
        
        
        
        
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
        
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



