#pragma once

#include <GLFW/glfw3.h>

namespace engine {
    double getFps() {
        static unsigned int counter = 0;
        static double prevTime = glfwGetTime();
        static double fps = 0.0;

        double crTime = glfwGetTime();
        counter++;

        // Calculate FPS every 0.1 second
        if (crTime - prevTime >= 0.1) {
            fps = counter / (crTime - prevTime);
            prevTime = crTime;
            counter = 0;
        }

        return fps;
    }
}
