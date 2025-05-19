#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Resource.h"

class ShaderProgram : public Resource {
private:
    uint32_t program_id;
public:
    uint32_t get_id() const;

    // Overloads for different types
    void set_uniform(const char* name, const bool value) const;
    void set_uniform(const char* name, const float value) const;
    void set_uniform(const char* name, const int32_t value) const;
    void set_uniform(const char* name, const uint32_t value) const;
    void set_uniform(const char* name, const glm::vec2 value) const;
    void set_uniform(const char* name, const glm::vec3 value) const;
    void set_uniform(const char* name, const glm::vec4 value) const;
    void set_uniform(const char* name, const uint32_t value) const;
    void set_uniform(const char* name, const glm::vec2 value) const;
    void set_uniform(const char* name, const uint32_t value) const;
};
