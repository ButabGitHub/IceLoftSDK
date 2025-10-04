#include "Game.h"
#include "Engine/Input.h"
#include <Include/stb_image/stb_image.h>

void Game::Init(GameConfig conf) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Only for MacOS
#endif

    GLFWwindow* window = glfwCreateWindow(conf.WinSize.x, conf.WinSize.y, conf.Title, NULL, NULL); // Main GLFW Window
    if (window == NULL) {
        std::cout << "\x1b[38;5;9mFailed to create GLFW window" << std::endl;
        glfwTerminate();
        // return -1;
    }
    glfwMakeContextCurrent(window); // Making main window current context 

    Input::set_input_callbacks(window);

    glfwSetWindowAspectRatio(window, 16, 9); // Set aspect ration to 16:9

    glfwSwapInterval(0); // Use to disable vsync (enabled by default)

    //glfwSetCursorPosCallback(window, cursor_position_callback);

    // Glad load all OpenGL function properties
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "\x1b[38;5;9mFailed to initialize GLAD" << std::endl;
        // return -1;
    }

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    GLFWimage WindowIcon[1];
    WindowIcon[0].pixels = stbi_load(conf.WinIconPath, &WindowIcon[0].width, &WindowIcon[0].height, 0, 4); // RGBA channels 
    glfwSetWindowIcon(window, 1, WindowIcon);
    stbi_image_free(WindowIcon[0].pixels);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

