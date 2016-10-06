#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include "Matrix.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
private:
    
    // Program Id
    GLuint programID;

    // Id's for each matrix
    GLuint projectionMatrixUniform;
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    
    // Id's for each attribute
    GLuint positionAttribute;
    GLuint colorAttribute;
    GLuint texCoordAttribute;

    // Id's for shaders
    GLuint vertexShader;
    GLuint fragmentShader;
    
    // Loading shaders
    GLuint loadShaderFromFile(const std::string& shaderFile, GLenum type);
    GLuint loadShaderFromString(const std::string& shaderContents, GLenum type);

public:
    
    // Constructor
    Shader(const char* vertexShaderFile, const char* fragmentShaderFile);

    // Bind the current program
    void bind();

    // Unbind the current program
    void unbind();

    // Getters
    GLuint getModelMatrixID();
    GLuint getViewMatrixID();
    GLuint getProjectionMatrixID();
    GLuint getProgramID();
    GLuint getPositionAttrib();
    GLuint getColorAttrib();
    GLuint getTexCoordAttrib();
    GLuint getVertexShaderID();
    GLuint getFragmentShaderID();

    // Setters
    void setModelMatrix(const Matrix &matrix);
    void setProjectionMatrix(const Matrix &matrix);
    void setViewMatrix(const Matrix &matrix);
};

#endif /* __SHADER_HPP__ defined */
