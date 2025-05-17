#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>
#include <type_traits>
#include <stdexcept>

#include "IclShader.h"
#include "Texture.h"

class ResourceManager {
public:
    template<typename T>
    static T Load(const char* file1, const char* file2, const char* file3, const std::string& name);

    // Specialization for Texture2D with alpha
    template<>
    static Texture2D Load<Texture2D>(const char* file, bool alpha, const std::string& name);

    template<typename T>
    static T Get(const std::string& name);

    static void Clear();

private:
    ResourceManager() = default;

    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;

    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

#endif


