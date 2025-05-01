#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <Windows.h>

#include <icl/inpt.h>
#include <unordered_map>
#include <icl/icl_ui.h>
#include <icl/icl_camera.h>
#include <engineGraphicalClasses/shader_s.h>
#include <icl/icl_texture.h>
#include <icl/engine.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void debugTexture(unsigned char* data, int width, int height, int nrChannels);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void showEditorMenu();

bool IsKeyPressedOnce(GLFWwindow* window, int key);

void setImGUIStuff();

// Window properties
const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
unsigned int WIN_CURRENT_W = WINDOW_WIDTH, WIN_CURRENT_H = WINDOW_HEIGHT;
const std::string WINDOW_TITLE = "IceLoft";

bool ShowImGUIDemo = true;
bool ShowPerformanceWindow = false;

// Camera 
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool canControl = false;

// Timing
float deltaTime = 0.0f;	// Time between current frame and last frame (like in godot lol)
float lastFrame = 0.0f;

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Only for MacOS
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL); // Main GLFW Window
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Making main window current context
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Call viewport resize func everytime window resizes
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    //glfwSetCursorPosCallback(window, cursor_position_callback);

    // Glad load all OpenGL function properties
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // Enable face culling // NOTE: Doesn't work as expected for now sadly :(
    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);*/

    // Build and compile main shader
    Shader mainShader("texture.vert", "texture.frag");
    Shader lightSrcShader("lightsrc.vert", "lightsrc.frag");
    Shader billboardShader("billboard.vert", "billboard.frag");

    // All rendering shit below
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };


    // World space positions of the cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/

    float quadVertices[] = {
        // positions     // texCoords
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // top left
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f  // bottom left
    };
    unsigned int quadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int quadVAO, quadVBO, quadEBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1); // texCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ImGUI stuff below
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    glm::vec3 viewVec = glm::vec3(0.0f, 0.0f, -3.0f);

    unsigned int missingTexture = icl::loadTexture("assets/textures/textures/img/missingTexture.png");
    unsigned int iconTex = icl::loadTexture("assets/textures/img/Light.png");

    mainShader.use();
    mainShader.setInt("material.diffuse", 0);

    // Process loop
    while (!glfwWindowShouldClose(window)) {

        // Frame stuff
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process all the input
        processInput(window);
        
        // Render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        icl::setImGUIStuff();

        // Show main window menu
        showEditorMenu();

        if (ShowImGUIDemo)
            ImGui::ShowDemoWindow();

        mainShader.use();

        // Set projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        mainShader.setMat4("projection", projection);

        // Set view matrix
        glm::mat4 view = camera.GetViewMatrix();
        mainShader.setMat4("view", view);

        // Render object
        unsigned int transformLoc = glGetUniformLocation(mainShader.ID, "transform");

        mainShader.setVec3("lightColor", lightColor);
        mainShader.setVec3("lightPos", lightPos);


        mainShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        mainShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        mainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        mainShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        mainShader.setFloat("material.shininess", 64.0f);

        // World transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        mainShader.setMat4("model", model);

        // Render the cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, missingTexture);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Also draw the lamp object
        lightSrcShader.use();
        lightSrcShader.setMat4("projection", projection);
        lightSrcShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // A smaller cube
        lightSrcShader.setMat4("model", model);
        lightSrcShader.setVec3("sColor", lightColor);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render light sourrce's (cube's) icon
        glDisable(GL_DEPTH_TEST);
        billboardShader.use();
        billboardShader.setMat4("view", view);
        billboardShader.setMat4("projection", projection);
        billboardShader.setVec3("billboardPos", lightPos);
        billboardShader.setFloat("size", 0.2f); // size in world units
        billboardShader.setVec3("tintColor", glm::vec3(1.0f, 1.0f, 0.8f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, iconTex);
        billboardShader.setInt("iconTexture", 0);

        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        // ImGui window setup
        ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("IceLoft Debug")) {
            ImGui::Text("Debug window");
            ImGui::Text("Camera FOV: %s", std::to_string(camera.Fov).c_str());

            // Adding render mode dropdown
            // Create category for render debug settings
            if (ImGui::CollapsingHeader("Render")) {
                static const char* render_mode_items[] = { "Basic fill mode", "Wireframe line mode" };
                static int render_mode_selectedItem = 0; // Index of the selected item

                if (ImGui::BeginCombo("Rendering mode", render_mode_items[render_mode_selectedItem])) { // The preview item
                    for (int i = 0; i < IM_ARRAYSIZE(render_mode_items); i++) {
                        bool isSelected = (render_mode_selectedItem == i);
                        if (ImGui::Selectable(render_mode_items[i], isSelected)) {
                            render_mode_selectedItem = i; // Set selected item
                        }
                        if (isSelected)
                            ImGui::SetItemDefaultFocus(); // Set default focus if it was selected before
                    }
                    ImGui::EndCombo();
                }

                // Change render mode based on it's selected item value 
                if (render_mode_selectedItem == 0) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                else if (render_mode_selectedItem == 1) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
            }
            // Category for some glm testing shit
            if (ImGui::CollapsingHeader("GLM testing shit")) {
                static float rotation_degrees = 90.0f;

                if (ImGui::DragFloat("Box rotation degrees", &rotation_degrees, 0.1f, 0.0f, 360.0f)) {
                    if (rotation_degrees == 360.0f) rotation_degrees = 0.0f;
                }

                ImGui::Text("Light position: ");
                if (ImGui::DragFloat("x", &lightPos[0], 0.1f));
                if (ImGui::DragFloat("y", &lightPos[1], 0.1f));
                if (ImGui::DragFloat("z", &lightPos[2], 0.1f));
            }
            // Category for ImGui things
            if (ImGui::CollapsingHeader("Window")) {
                if (ImGui::Button("Reset debug window size")) {
                    ImGui::SetWindowSize("IceLoft Debug", ImVec2(500, 200));
                }
                if (ImGui::Button("Reset editor window size")) {
                    glfwSetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
                    glfwRestoreWindow(window);
                }
            }
        }

        // ImGui window end
        ImGui::End(); 

        if (ShowPerformanceWindow) {
            // Get viewport and calculate bottom-right position
            ImVec2 window_size = ImVec2(200, 60); // Adjust as needed
            ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
            ImVec2 window_pos = ImVec2(viewport_size.x - window_size.x - 10, viewport_size.y - window_size.y - 10); // Padding of 10px

            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

            if (ImGui::Begin("Performance", &ShowPerformanceWindow, flags)) {
                std::string fpsStr = std::to_string(engine::getFps());
                ImGui::Text("FPS: %s", fpsStr.c_str());
            }
            ImGui::End();
        }

        // ImGui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll input events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clear ImGui processes
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // De-allocate outlived purpose resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate(); // Terminate all GLFW resources
    return 0;
}

// This function makes viewport size match window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    WIN_CURRENT_W = width;
    WIN_CURRENT_H = height;
}

// Process all the input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true); // Close window when 'Esc' key is pressed
    if (IsKeyPressedOnce(window, GLFW_KEY_Q))
        ShowImGUIDemo = !ShowImGUIDemo;

    // Camera input
    if (canControl) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_REPEAT || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (IsKeyPressedOnce(window, GLFW_KEY_F3)) {
            ShowPerformanceWindow = !ShowPerformanceWindow;
        }
    }
}

void showEditorMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", "CTRL+S")) {}
            if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S")) {}
            if (ImGui::MenuItem("Load")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("New", "CTRL+N")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("IclDebug")) {
            if (ImGui::MenuItem("Show Iceloft debug menu", "")) {}
            if (ImGui::MenuItem("Show ImGUI debug menu", "CTRL+SHIFT+", &ShowImGUIDemo)) {}
            if (ImGui::MenuItem("Show Performance debug menu", "SHIFT+F3", &ShowPerformanceWindow)) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

//void showIclDebugMenu;

std::unordered_map<int, bool> keyWasDownLastFrame;

bool IsKeyPressedOnce(GLFWwindow* window, int key) {
    bool isDownNow = glfwGetKey(window, key) == GLFW_PRESS;
    bool wasDownBefore = keyWasDownLastFrame[key];

    keyWasDownLastFrame[key] = isDownNow;

    return isDownNow && !wasDownBefore;
}

// Whenever the mouse moves this GLFW callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (canControl) {
        //glfwSetCursorPos(window, WIN_CURRENT_W / 2, WIN_CURRENT_H / 2);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
            return;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
}

// Whenever the mouse scroll wheel scrolls this GLFW callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (canControl) {
        camera.ProcessMouseScroll(yoffset);
    }
}

// Whenever the mouse button is pressed this GLFW callback is called
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        canControl = true;
    else
        canControl = false;
}
