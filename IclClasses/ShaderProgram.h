#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IclShader.h"

class ShaderProgram : public Resource {
private:
    uint32_t program_id;
public:
    // i dont even need to explain this one
    uint32_t get_id() const;

    // Constructor & destructor
    ShaderProgram(const std::initializer_list<Shader*> /*std::initializer_list<std::unique_ptr<Shader>>*/ shaders);
    ~ShaderProgram();

    // Overloads for different types
    void set_uniform(const char* name, const bool value) const;
    void set_uniform(const char* name, const float value) const;
    void set_uniform(const char* name, const int32_t value) const;
    void set_uniform(const char* name, const uint32_t value) const;
    void set_uniform(const char* name, const glm::vec2 value) const;
    void set_uniform(const char* name, const glm::vec3 value) const;
    void set_uniform(const char* name, const glm::vec4 value) const;
    void set_uniform(const char* name, const glm::mat2 value) const;
    void set_uniform(const char* name, const glm::mat3 value) const;
    void set_uniform(const char* name, const glm::mat4 value) const;
};
