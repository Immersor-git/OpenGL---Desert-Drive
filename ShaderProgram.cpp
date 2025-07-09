#include "ShaderProgram.h"
#include "Mesh3D.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SFML/Window.hpp>

ShaderProgram::ShaderProgram()
    : m_programId(-1) {

}



void ShaderProgram::load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        throw std::runtime_error("Failed to locate vertex or fragment shader files");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        throw std::runtime_error(infoLog);
    };

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        throw std::runtime_error(infoLog);
    };

    // shader Program
    m_programId = glCreateProgram();
    glAttachShader(m_programId, vertex);
    glAttachShader(m_programId, fragment);
    glLinkProgram(m_programId);
    // print linking errors if any
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_programId, 512, NULL, infoLog);
        throw std::runtime_error(infoLog);
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

uint32_t myFbo;
uint32_t depthBufferId;

void ShaderProgram::EnableShadowMap() {
    // Generate and bind a custom framebuffer.
    // Generate and bind a custom framebuffer.
    //myFbo;
    glGenFramebuffers(1, &myFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, myFbo);
    //setUniform("shadowMap", myFbo);
// Render commands will no longer render to the screen.

    //depthBufferId;
    glGenTextures(1, &depthBufferId);
    glBindTexture(GL_TEXTURE_2D, depthBufferId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 300, 300, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// Bind the two textures as the write-destinations for color and depth.
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferId, 0);
}

void ShaderProgram::RenderShadowMap() {
    glBindFramebuffer(GL_FRAMEBUFFER, myFbo);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 300, 300);
}

void ShaderProgram::ShadowMapComplete() {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthBufferId);
    setUniform("shadowMap", 1);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShaderProgram::activate()
{
    glUseProgram(m_programId);
}

void ShaderProgram::setUniform(const std::string& uniformName, bool value)
{
    glUniform1i(glGetUniformLocation(m_programId, uniformName.c_str()), (int32_t)value);
}

void ShaderProgram::setUniform(const std::string& uniformName, uint32_t value)
{
    glUniform1i(glGetUniformLocation(m_programId, uniformName.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& uniformName, int32_t value)
{
    glUniform1i(glGetUniformLocation(m_programId, uniformName.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& uniformName, float_t value)
{
    glUniform1f(glGetUniformLocation(m_programId, uniformName.c_str()), (int)value);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::vec2& value)
{
    glUniform2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, &value[0]);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, &value[0]);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::vec4& value)
{
    glUniform4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, &value[0]);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::mat2& value)
{
    glUniformMatrix2fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, &value[0][0]);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::mat3& value)
{
    glUniformMatrix3fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, &value[0][0]);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_programId, uniformName.c_str()), 1, false, &value[0][0]);
}

/**
 * @brief Constructs a shader program that renders textured meshes in the Phong reflection model.
 * The shaders used here are incomplete; see their source codes.
 * @return
 */
ShaderProgram ShaderProgram::phongLighting() {
    ShaderProgram program;
    try {
        program.load("../shaders/light_perspective.vert", "../shaders/lighting.frag");
    }
    catch (std::runtime_error& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        exit(1);
    }
    return program;
}

ShaderProgram ShaderProgram::skyShading() {
    ShaderProgram program;
    try {
        program.load("../shaders/no_transform.vert", "../shaders/colors.frag");
        std::cout << "Sky shader loaded!" << std::endl;
    }
    catch (std::runtime_error& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        exit(1);
    }
    return program;
}
/**
 * @brief Constructs a shader program that renders textured meshes without lighting.
 */
ShaderProgram ShaderProgram::textureMapping() {
    ShaderProgram program;
    try {
        program.load("../shaders/texture_perspective.vert", "../shaders/texturing.frag");
    }
    catch (std::runtime_error& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        exit(1);
    }
    return std::move(program);
}
