#pragma once

#include <glad/glad.h>

#include "Resource.h"

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D : Resource {
private:
    // Holds the ID of the texture object, used for all texture operations to reference to this particular texture
    uint32_t texture_id;

public:
    // Returns the texture id
    uint32_t get_id() const;
    // Texture image dimensions
    uint32_t Width, Height; // Width and height of loaded image in pixels
    // Texture Format
    uint32_t Internal_Format; // Format of texture object
    uint32_t Image_Format; // Format of loaded image
    // Texture configuration
    uint32_t Wrap_S; // Wrapping mode on S axis
    uint32_t Wrap_T; // Wrapping mode on T axis
    uint32_t Filter_Min; // Filtering mode if texture pixels < screen pixels
    uint32_t Filter_Max; // Filtering mode if texture pixels > screen pixels
    // Constructor (sets default texture modes)
    Texture2D();
    // Generates texture from image data
    void Generate(uint32_t width, uint32_t height, unsigned char* data);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};
