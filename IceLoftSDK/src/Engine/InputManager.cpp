#include "Input.h"
#include <Level/Root.h>

std::map<uint16_t, bool> Input::Keys;
std::unordered_map<uint16_t, bool> Input::KeyWasDownLastFrame;

double Input::mouse_pos_x;
double Input::mouse_pos_y;

void Input::set_input_callbacks(GLFWwindow* window) {
	glfwSetKeyCallback(window, Input::key_callback);
	glfwSetMouseButtonCallback(window, Input::mouse_button_callback);
	glfwSetCursorPosCallback(window, Input::cursor_pos_callback);
}

// GLFW input callbacks ------

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	root->ProcessInput();

	if (action == GLFW_PRESS) {
		Keys[key] = true;
	} 
	else if (action == GLFW_RELEASE) {
		Keys[key] = false;
	}
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	root->ProcessInput();

	if (action == GLFW_PRESS) {
		Keys[button] = true;
	}
	else if (action == GLFW_RELEASE) {
		Keys[button] = false;
	}
}

void Input::cursor_pos_callback(GLFWwindow* window, double x, double y) {
	mouse_pos_x = x;
	mouse_pos_y = y;
}

// ---------------------------

bool Input::is_key_pressed(int key) {
	if (Keys[key])
		return Keys[key];
}

bool Input::is_key_just_pressed(int key) {
	bool isDownNow = Keys[key];
	bool wasDownBefore = KeyWasDownLastFrame[key];

	KeyWasDownLastFrame[key] = isDownNow;

	return isDownNow && !wasDownBefore;
}

bool Input::is_mouse_button_pressed(int button) {
	if (Keys[button])
		return Keys[button];
}

glm::vec2 Input::get_mouse_cursor_pos() {
	return glm::vec2(mouse_pos_x, mouse_pos_y);
}
