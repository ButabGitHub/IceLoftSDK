#include "IclShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

uint32_t Shader::get_id() const {
    return shader_id;
}

Resource Shader::Load(const char* path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open a file: " + path);

    std::unordered_map<ShaderType, std::string> shaders;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("#type") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else
                throw std::runtime_error("Unknown shader type in file");
        }
        else {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    shaders[ShaderType::VERTEX] = ss[0].str();
    shaders[ShaderType::FRAGMENT] = ss[1].str();
    return shaders;
}

char Shader::shader_parse(const char* code) {

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
