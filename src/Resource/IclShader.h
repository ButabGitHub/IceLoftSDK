#pragma once

#include <Base/Resource.h>
//#include <unordered_map>

class Shader : public Resource {
private:
    uint32_t shader_id; // OpenGL ID of the shader
public:
    uint32_t get_id() const;

    Shader(const uint32_t type, const char* code);
    ~Shader();

    static std::unique_ptr<Shader> Load(const char* path); // Load stuff from the resource class itself because I have too much skill issue to do Godot-like system
};
