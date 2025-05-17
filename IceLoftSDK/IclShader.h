#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <type_traits>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    // State
    unsigned int ID;

    // Constructor
    Shader() {}
    Shader& Use();

    // Compiles the shader from given source code
    void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

    // Generic uniform setter
    template<typename T>
    void Set(const char* name, const T& value, bool useShader = false) {
        if (useShader) this->Use();
        GLint location = glGetUniformLocation(this->ID, name);

        if constexpr (std::is_same_v<T, float>)
            glUniform1f(location, value);
        else if constexpr (std::is_same_v<T, int>)
            glUniform1i(location, value);
        else if constexpr (std::is_same_v<T, glm::vec2>)
            glUniform2fv(location, 1, glm::value_ptr(value));
        else if constexpr (std::is_same_v<T, glm::vec3>)
            glUniform3fv(location, 1, glm::value_ptr(value));
        else if constexpr (std::is_same_v<T, glm::vec4>)
            glUniform4fv(location, 1, glm::value_ptr(value));
        else if constexpr (std::is_same_v<T, glm::mat4>)
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        else
            static_assert(sizeof(T) == 0, "Unsupported uniform type");
    }

    // Float vector overloads
    // Vec2
    template<typename T>
    void Set(const char* name, T x, T y, bool useShader = false) {
        static_assert(std::is_same_v<T, float>, "Only float types supported");
        if (useShader) this->Use();
        glUniform2f(glGetUniformLocation(this->ID, name), x, y);
    }

    // Vec3
    template<typename T>
    void Set(const char* name, T x, T y, T z, bool useShader = false) {
        static_assert(std::is_same_v<T, float>, "Only float types supported");
        if (useShader) this->Use();
        glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
    }

    // Vec4
    template<typename T>
    void Set(const char* name, T x, T y, T z, T w, bool useShader = false) {
        static_assert(std::is_same_v<T, float>, "Only float types supported");
        if (useShader) this->Use();
        glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
    }

private:
    // Checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(unsigned int object, std::string type);
};

#endif

