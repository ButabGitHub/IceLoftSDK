#include "Input.h"
#include "../Node/Root.h"

std::map<uint16_t, bool> Input::Keys;

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	root->ProcessInput();

	if (action == GLFW_PRESS) {
		Keys[key] = true;
	} 
	else if (action == GLFW_RELEASE) {
		Keys[key] = false;
	}
}

bool Input::is_key_pressed(int key) {
	if (Keys[key])
		return Keys[key];
}

//bool Input::is_key_just_pressed(int key) {
//	bool isDownNow = Input::is_key_pressed(key);
//	bool wasDownBefore = Keys[key];
//
//	Keys[key] = isDownNow;
//
//	return isDownNow && !wasDownBefore;
//}
