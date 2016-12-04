#include "Shader.hpp"

GLuint Shader::loadShaderFromFile(const std::string& shaderFile, GLenum type) {
    // Open a file stream with the file name
    std::ifstream infile(shaderFile);
    
    if(infile.fail()) {
        std::cout << "Error opening shader file:" << shaderFile << std::endl;
    }
    
    // Create a string buffer and stream the file to it
    std::stringstream buffer;
    buffer << infile.rdbuf();
    
    // Load the shader from the contents of the file
    return loadShaderFromString(buffer.str(), type);
}

GLuint Shader::loadShaderFromString(const std::string& shaderContents, GLenum type) {
    
    // Create a shader of specified type
    GLuint shaderID = glCreateShader(type);
    
    // Get the pointer to the C string from the STL string
    const char *shaderString = shaderContents.c_str();
    GLint shaderStringLength = (GLint) shaderContents.size();
    
    // Set the shader source to the string and compile shader
    glShaderSource(shaderID, 1, &shaderString, &shaderStringLength);
    glCompileShader(shaderID);
    
    // Check if the shader compiled properly
    GLint compileSuccess;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileSuccess);
    
    // If the shader did not compile, print the error to stdout
    if (compileSuccess == GL_FALSE) {
        GLchar messages[512];
        glGetShaderInfoLog(shaderID, sizeof(messages), 0, &messages[0]);
        std::cout << messages << std::endl;
    }
    
    // return the shader id
    return shaderID;
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
    
    // create the vertex shader
    vertexShader = loadShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
    
    // create the fragment shader
    fragmentShader = loadShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);
    
    // Create the final shader program from our vertex and fragment shaders
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    
    GLint linkSuccess, validateStatus;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkSuccess);
    if(linkSuccess == GL_FALSE ) {
        std::cout << "Shader Linking FAILED" << std::endl;
        GLchar messages[256];
        glGetProgramInfoLog(programID, sizeof(messages),0,&messages[0]);
        std::cout << messages;
    }
    
    glValidateProgram(programID);
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &validateStatus);
    
    std::cout << "Link: " << linkSuccess << "  Validate: " << validateStatus << std::endl;
    if(linkSuccess == GL_FALSE ) {
        std::cout << "Shader Validation FAILED" << std::endl;
        GLchar messages[256];
        glGetProgramInfoLog(programID, sizeof(messages),0,&messages[0]);
        std::cout << messages;
    }
    
    modelMatrixUniform = glGetUniformLocation(programID, "modelMatrix");
    projectionMatrixUniform = glGetUniformLocation(programID, "projectionMatrix");
    viewMatrixUniform = glGetUniformLocation(programID, "viewMatrix");
    
    positionAttribute = glGetAttribLocation(programID, "position");
    colorAttribute = glGetAttribLocation(programID, "color");
    texCoordAttribute = glGetAttribLocation(programID, "texCoord");
}

void Shader::bind() { glUseProgram(programID); }
void Shader::unbind() { glUseProgram(0); }

GLuint Shader::getModelMatrixID() { return modelMatrixUniform; }
GLuint Shader::getViewMatrixID() { return viewMatrixUniform; }
GLuint Shader::getProjectionMatrixID() { return projectionMatrixUniform; }
GLuint Shader::getProgramID() { return programID; }
GLuint Shader::getPositionAttrib() { return positionAttribute; }
GLuint Shader::getColorAttrib() { return colorAttribute; }
GLuint Shader::getTexCoordAttrib() { return texCoordAttribute; }
GLuint Shader::getVertexShaderID() { return vertexShader; }
GLuint Shader::getFragmentShaderID() { return fragmentShader; }

void Shader::setViewMatrix(const Matrix &matrix) {
    glUseProgram(programID);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, matrix.ml);
}

void Shader::setModelMatrix(const Matrix &matrix) {
    glUseProgram(programID);
    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, matrix.ml);
}

void Shader::setProjectionMatrix(const Matrix &matrix) {
    glUseProgram(programID);
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, matrix.ml);
}
