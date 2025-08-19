#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "../Node.h"

class Viewport : public Node {
public:
    using ResizeCallback = std::function<void(int width, int height)>;

    Viewport(uint16_t width, uint16_t height, const std::string& title,
        GLFWmonitor* monitor = nullptr, GLFWwindow* shared = nullptr);
    ~Viewport();

    void make_context_current() const;
    void swap_buffers() const;
    void poll_events() const;
    void set_resize_callback(ResizeCallback callback);
    void set_framebuffer_size_callback();
    void set_key_callback();


    GLFWwindow* get_window() const;
    uint16_t get_width() const;
    uint16_t get_height() const;
    const std::string& get_title() const;
    bool should_close() const;

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    uint16_t              width;
    uint16_t              height;
    const std::string     title;
    GLFWmonitor* const    monitor;
    GLFWwindow* const     shared;
    GLFWwindow*           window = nullptr;

    ResizeCallback        resize_callback;
};

