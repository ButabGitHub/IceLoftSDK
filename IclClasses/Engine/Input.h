#pragma once

#include <GLFW/glfw3.h>
#include <map>

class Input {
public:

	static void set_input_callbacks();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool is_key_pressed(int key);
	//static bool is_key_just_pressed(int key);
	static bool is_mouse_button_pressed(int button);

protected:
	static std::map<uint16_t, bool> Keys;

};

