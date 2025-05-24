#pragma once

#include "Resource.h"

class Shader : public Resource {
public:
private:
    uint32_t shader_id; // OpenGL ID of the shader

    char shader_parse(const char* code); // Parse the shader
public:
    uint32_t get_id() const;

    Shader(const uint32_t type, const char* code);
    ~Shader();

    virtual Resource Load(const char* path) override;
};
