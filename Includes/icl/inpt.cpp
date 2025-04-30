//#include "inpt.h"
//#include <unordered_map>
//#include <glfw3.h>
//
//std::unordered_map<int, bool> keyWasDownLastFrame;
//
//namespace inpt {
//    bool IsKeyPressedOnce(GLFWwindow* window, int key) {
//        bool isDownNow = glfwGetKey(window, key) == GLFW_PRESS;
//        bool wasDownBefore = keyWasDownLastFrame[key];
//
//        keyWasDownLastFrame[key] = isDownNow;
//
//        return isDownNow && !wasDownBefore;
//    }
//}
