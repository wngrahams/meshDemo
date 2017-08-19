//
//  Mesh.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/18/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    Mesh(std::vector<Vertex> verts, std::vector<GLuint> ind, std::vector<Texture> texts) {
        this->vertices = verts;
        this->indices = ind;
        this->textures = texts;
        
        this->setupMesh();
    }
    
    void draw(Shader shader) {
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        
        
        for (GLuint i=0; i<this->textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            
            std::stringstream ss;
            std::string number;
            std::string name = this->textures[i].type;
            
            if (name == "texture_diffuse")
                ss << diffuseNr++; //put it into the stringstream and then increment it;
            
            else if (name == "texture_specular")
                ss << specularNr++;
            
            number = ss.str();
            
            glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }
        
        //set each mesh's shininess to a default value
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);
        
        //draw mesh:
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        for (GLuint i=0; i<this->textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
private:
    GLuint VAO, VBO, EBO;
    
    void setupMesh() {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);
        
        glBindVertexArray(this->VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
        
        //vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid * )0);
        
        //vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid * )offsetof(Vertex, normal));

        //vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid * )offsetof(Vertex, texCoords));
        
        glBindVertexArray(0);
    }
};



