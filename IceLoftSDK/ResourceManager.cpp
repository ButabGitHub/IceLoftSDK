#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "stb_image.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

template<>
Shader ResourceManager::Load<Shader>(const char* vFile, const char* fFile, const char* gFile, const std::string& name) {
    Shaders[name] = loadShaderFromFile(vFile, fFile, gFile);
    return Shaders[name];
}

template<>
Texture2D ResourceManager::Load<Texture2D>(const char* file, bool alpha, const std::string& name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

template<>
Shader ResourceManager::Get<Shader>(const std::string& name) {
    return Shaders.at(name);
}

template<>
Texture2D ResourceManager::Get<Texture2D>(const std::string& name) {
    return Textures.at(name);
}

void ResourceManager::Clear() {
    for (auto& [_, shader] : Shaders)
        glDeleteProgram(shader.ID);
    for (auto& [_, texture] : Textures)
        glDeleteTextures(1, &texture.ID);
    Shaders.clear();
    Textures.clear();
}

Shader ResourceManager::loadShaderFromFile(const char* vFile, const char* fFile, const char* gFile) {
    std::ifstream vsFile(vFile), fsFile(fFile), gsFile;
    std::stringstream vsStream, fsStream, gsStream;
    vsStream << vsFile.rdbuf(); fsStream << fsFile.rdbuf();
    std::string vertexCode = vsStream.str();
    std::string fragmentCode = fsStream.str();

    std::string geometryCode;
    if (gFile) {
        gsFile.open(gFile);
        gsStream << gsFile.rdbuf();
        geometryCode = gsStream.str();
    }

    Shader shader;
    shader.Compile(vertexCode.c_str(), fragmentCode.c_str(), gFile ? geometryCode.c_str() : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
    Texture2D texture;
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    texture.Generate(width, height, data);
    stbi_image_free(data);
    return texture;
}

