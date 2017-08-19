//
//  Model.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/19/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

//prototypes
GLint textureFromFile(const char *path, std::string directory);

class Model {
public:
    //constructor
    Model(GLchar *path) {
        this->loadModel(path);
    }
    
    void draw(Shader shader) {
        for (GLuint i=0; i<this->meshes.size(); i++)
            this->meshes[i].draw(shader);
    }
    
private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    
    void loadModel(std::string path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        
        //check if model has successfully loaded
        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            
            return;
        }
        
        this->directory = path.substr(0, path.find_last_of('/'));
        
        //recursively process assimp root nodes
        this->processNode(scene->mRootNode, scene);
    }
    
    void processNode(aiNode *node, const aiScene *scene) {
        //process any meshes
        for (GLuint i=0; i<node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            
            this->meshes.push_back(this->processMesh(mesh, scene));
        }
        
        //recursively process children nodes
        for (GLuint i=0; i<node->mNumChildren; i++)
            this->processNode(node->mChildren[i], scene);
        
    }
    
    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        
        for (GLuint i=0; i<mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;
            
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
            
            //check if mesh contains textures
            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;

                vertex.texCoords = vec;
            }
            else
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            
            vertices.push_back(vertex);
        }
        
        for (GLuint i=0; i<mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            
            for (GLuint j=0; j<face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
            
        }
        
        //process materials
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            
            std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            
            std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        
        return Mesh(vertices, indices, textures);
    }
    
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
        std::vector<Texture> textures;
        
        for (GLuint i=0; i<mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            
            GLboolean skip = false;
            
            //prevent loading the same texture twice
            for (GLuint j = 0; j<textures_loaded.size(); j++) {
                if (textures_loaded[j].path == str) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    
                    break;
                }
            }
            
            if (!skip) {
                Texture texture;
                texture.id = textureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                
                this->textures_loaded.push_back(texture);
            }
        }
        
        return textures;
    }
};

GLint textureFromFile(const char *path, std::string directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height;
    
    unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    SOIL_free_image_data(image);
    
    return textureID;
}



