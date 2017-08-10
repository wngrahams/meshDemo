//
//  Shader.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/9/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#ifndef meshReduction_Shader_h
#define meshReduction_Shader_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader {
    
public:
    GLuint Program;
    
    //Constructor generates shader at runtime
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
        
        //Retrieve vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        //ensure ifstream can throw exceptions
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        
        try {
            //Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            
            //Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            //close files
            vShaderFile.close();
            fShaderFile.close();
            
            //convert stringstream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            
        } catch (std::ifstream::failure e) {
            std::cout << "\nERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
            std::cout << vertexCode << std::endl;
            std::cout << fragmentCode << std::endl;
        }
        
        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();
        
        //initialize and compile vertex shaders
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        
        //check for vertex shader compilation errors
        GLint success;
        GLchar infoLog[512];
        
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "\nERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            std::cout << vertexCode << std::endl;
        }
        
        //initialize and compile fragment shaders
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);
        
        //check for fragment shader compilation errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "\nERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            std::cout << fragmentCode << std::endl;
        }
        
        //shader program, link shaders
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertexShader);
        glAttachShader(this->Program, fragmentShader);
        glLinkProgram(this->Program);
        
        //check for linking errors
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "\nERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            std::cout << vertexCode << std::endl;
            std::cout << fragmentCode << std::endl;
        }
        
        //clean up shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    
    //Use current shader
    void use() {
        glUseProgram(this->Program);
    }
};

#endif





