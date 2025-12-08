#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <map>
#include <unordered_map>

class Input {
public:

	static void set_input_callbacks(GLFWwindow* window);


	static bool is_key_pressed(int key);
	static bool is_key_just_pressed(int key);
	static bool is_mouse_button_pressed(int button);

	static glm::vec2 get_mouse_cursor_pos();

protected:
	static std::map<uint16_t, bool> Keys;
	static std::unordered_map<uint16_t, bool> KeyWasDownLastFrame;
	
	static double mouse_pos_x;
	static double mouse_pos_y;

	// GLFW input callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static void cursor_pos_callback(GLFWwindow* window, double x, double y);

};

