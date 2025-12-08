#include "IclShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <filesystem>

uint32_t Shader::get_id() const {
    return shader_id;
}

std::unique_ptr<Shader> Shader::Load(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) { // Error handling
        std::cerr << "\x1b[38;5;9m\ Failed to open a shader: " << path;
        return nullptr;
    }

    uint32_t type = NULL; // Shader type

    std::string line;
    std::stringstream ss;
    
    std::string filename(path);

    if (filename.find(".vert") != std::string::npos) {
        type = GL_VERTEX_SHADER;
    }
    else if (filename.find(".frag") != std::string::npos) {
        type = GL_FRAGMENT_SHADER;
    }
    else {
        std::cerr << "\x1b[38;5;9m\ Couldn't find shader type specifier at path: " << path << "\n";
        return nullptr;
    }

    while (std::getline(file, line)) {
            if (type != NULL) {
                ss << line << "\n";
            }
            else {
                std::cerr << "\x1b[38;5;9m\ Couldn't find shader type specifier at path: " << path << "\n";
                return nullptr;
            }
    }

    // Create a new shader which will get returned
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(type, ss.str().c_str());

    return shader;
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
        std::cerr << "\x1b[38;5;9mShader compilation failed: " << info_log << "\n";
    }
}

Shader::~Shader() {
    glDeleteShader(shader_id);
}
