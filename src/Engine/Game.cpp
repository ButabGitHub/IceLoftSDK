#include "Game.h"
#include "Engine/InputManager.h"
#include <Include/stb_image/stb_image.h>

Game::Game() {

}

Game::~Game() {
    glfwTerminate(); // Terminate all GLFW resources
}

void Game::Init(GameConfig conf) {

#ifdef _WIN32
    // Make ANSI escape sequences work for Windows command prompt
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_VIRTUAL_TERMINAL_INPUT);
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(hOutput, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif // _WIN32

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Only for MacOS
#endif // __APPLE__

    GameWindow = glfwCreateWindow(conf.WinSize.x, conf.WinSize.y, conf.Title, NULL, NULL); // Main GLFW Window
    if (GameWindow == NULL) {
        std::cout << "\x1b[38;5;9mFailed to create GLFW window" << std::endl;
        glfwTerminate();
        // return -1;
    }
    glfwMakeContextCurrent(GameWindow);

    Input::set_input_callbacks(GameWindow);

    glfwSetWindowAspectRatio(GameWindow, conf.WinAspectRatio.x, conf.WinAspectRatio.y); // Set aspect ratio

    glfwSwapInterval(conf.VSync);

    // Glad load all OpenGL function properties
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "\x1b[38;5;9mFailed to initialize GLAD" << std::endl;
    }

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    GLFWimage WindowIcon[1];
    WindowIcon[0].pixels = stbi_load(conf.WinIconPath, &WindowIcon[0].width, &WindowIcon[0].height, 0, 4); // RGBA channels
    glfwSetWindowIcon(GameWindow, 1, WindowIcon);
    stbi_image_free(WindowIcon[0].pixels);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Game::Enter() {
#if CUSTOM_CLOSING_LOGIC
    while (!WindowShouldClose) {
#else
    while (!glfwWindowShouldClose(GameWindow)) {
#endif // CUSTOM_CLOSING_LOGIC
        LvManager.Update(0.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

GLFWwindow*& Game::GetWindow() {
    GLFWwindow*& win = GameWindow;
    return win;
}

LevelManager& Game::GetLevelManager() {
    LevelManager& LMRef = LvManager;
    return LMRef;
}
