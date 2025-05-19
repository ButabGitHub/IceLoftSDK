#pragma once

#include <string>
#include <type_traits>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Resource.h"

class Shader : public Resource {
public:
private:
    uint32_t shader_id; // OpenGL ID of the shader
public:
    uint32_t get_id() const;

    Shader(const uint32_t type, const char* code);
    ~Shader();

    // Compiles the shader from given source code
    void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

private:
    // Checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(uint32_t object, std::string type);
};
