#include "Viewport.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

// Static mapping from window pointer to Viewport instance
static std::unordered_map<GLFWwindow*, Viewport*> window_map;

Viewport::Viewport(uint16_t w, uint16_t h, const std::string& t,
    GLFWmonitor* m, GLFWwindow* s)
    : width(w), height(h), title(t), monitor(m), shared(s)
{
    window = glfwCreateWindow(width, height, title.c_str(), monitor, shared);
    if (!window) {
        std::cerr << "Failed to create GLFW window: " << title << std::endl;
        throw std::runtime_error("GLFW window creation failed");
    }

    window_map[window] = this;
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Viewport::~Viewport() {
    if (window) {
        window_map.erase(window);
        glfwDestroyWindow(window);
        window = nullptr;
    }
}

void Viewport::make_context_current() const {
    glfwMakeContextCurrent(window);
}

void Viewport::swap_buffers() const {
    glfwSwapBuffers(window);
}

void Viewport::poll_events() const {
    glfwPollEvents(); // You can call this globally if managing multiple windows
}

GLFWwindow* Viewport::get_window() const {
    return window;
}

uint16_t Viewport::get_width() const {
    return width;
}

uint16_t Viewport::get_height() const {
    return height;
}

const std::string& Viewport::get_title() const {
    return title;
}

bool Viewport::should_close() const {
    return glfwWindowShouldClose(window);
}

void Viewport::set_resize_callback(ResizeCallback callback) {
    resize_callback = callback;
}

void Viewport::framebuffer_size_callback(GLFWwindow* win, int w, int h) {
    auto* vp = static_cast<Viewport*>(glfwGetWindowUserPointer(win));
    if (vp) {
        vp->width = w;
        vp->height = h;
        if (vp->resize_callback) {
            vp->resize_callback(w, h);
        }
    }
}

