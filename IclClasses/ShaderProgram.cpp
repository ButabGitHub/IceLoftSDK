#include "ShaderProgram.h"

// Glad must be included before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

uint32_t ShaderProgram::get_id() const {
    return program_id;
}

// OVERLOADS
////////////////////////////////////////////////////////////////////////////////////////////////////////
void ShaderProgram::set_uniform(const char* name, const bool value) const {
    glUniform1i(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const float value) const {
    glUniform1f(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const int32_t value) const {
    glUniform1i(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const uint32_t value) const {
    glUniform1ui(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const glm::vec2 value) const {
    glUniform1ui(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const glm::vec3 value) const {
    glUniform1ui(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const glm::vec4 value) const {
    glUniform1ui(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const glm::mat2 value) const {
    glUniform1ui(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const glm::mat3 value) const {
    glUniform1ui(glGetUniformLocation(program_id, name), value);
}

void ShaderProgram::set_uniform(const char* name, const glm::mat4 value) const {
    glUniform1ui(glGetUniformLocation(program_id, name), value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ShaderProgram::ShaderProgram(const std::initializer_list<Shader*> shaders) {
    program_id = glCreateProgram();
    for (Shader* shader : shaders) {
        glAttachShader(program_id, shader->get_id());
    }
    glLinkProgram(program_id);

    int32_t success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);

    if (!success) {
        char info_log[2048];
        glGetProgramInfoLog(program_id, 2048, NULL, info_log);
        std::cout << "Shader linking failed: " << info_log << std::endl;
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_id);
}