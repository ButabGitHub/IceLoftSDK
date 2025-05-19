#include "IclShader.h"
#include <iostream>

uint32_t Shader::get_id() const {
    return shader_id;
}

Shader::Shader(const uint32_t type, const char* code) {
    shader_id = glCreateShader(type); // Type ex. GL_VERTEX_SHADER
    glShaderSource(shader_id, 1, &code, NULL);
    glCompileShader(shader_id);

    int32_t success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success) {
        char info_log[2048];
        glGetShaderInfoLog(shader_id, 2048, NULL, info_log);
        std::cout << "Shader compilation failed: " << info_log << std::endl;
    }
}

Shader::~Shader() {
    glDeleteShader(shader_id);
}
