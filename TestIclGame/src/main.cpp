#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <Windows.h>

#include <unordered_map>
#include "icl/icl_camera.h"
#include "icl/icl_texture.h"
#include "icl/engine.h"

#include "Resource/ShaderProgram.h"
#include "Base/Node.h"
#include "Scene/Root.h"
#include "Engine/Viewport.h"
#include "Engine/Input.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void showEditorMenu();

bool IsKeyPressedOnce(int key);

void setImGUIStuff();

// Window properties
const uint16_t SCR_WIDTH = 1280, SCR_HEIGHT = 720;
uint16_t WIN_CURRENT_W = SCR_WIDTH, WIN_CURRENT_H = SCR_HEIGHT;
const std::string WINDOW_TITLE = "TestIclGame";

uint16_t SCR_CENTER_X, SCR_CENTER_y;

bool ShowIclDebug = true;
bool ShowImGUIDemo = false;
bool ShowPerformanceWindow = false;
bool IsFullscreen = false;

// Camera 
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool canControl = false;

// Delta
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

class test : public Node {
public:
    void Init() override {
        Node::Init();
        std::cout << "\x1b[38;5;70m\ Hello test!!";
    }

    void ProcessInput() override {
        Node::ProcessInput();

        if (Input::is_key_pressed(GLFW_KEY_SPACE)) {
            std::cout << "Space was pressed!";
            this->Destroy();
        }
    }

};

class test2 : public Node {
public:
    void Init() override {
        Node::Init();
        std::cout << "\x1b[38;5;70m\ Hello test!!";
    }

    void ProcessInput() override {
        Node::ProcessInput();

        if (Input::is_key_pressed(GLFW_KEY_B))
            std::cout << "Space dsdfnm";
    }

};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Only for MacOS
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL); // Main GLFW Window
    if (window == NULL) {
        std::cout << "\x1b[38;5;9mFailed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Making main window current context 

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Call viewport resize func everytime window resizes
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    Input::set_input_callbacks(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetWindowAspectRatio(window, 16, 9); // Set aspect ration to 16:9

    glfwSwapInterval(0); // Use to disable vsync (enabled by default)

    //glfwSetCursorPosCallback(window, cursor_position_callback);

    // Glad load all OpenGL function properties
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "\x1b[38;5;9mFailed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    GLFWimage images[1];
    images[0].pixels = stbi_load("assets/icons/IceLoft_Logo2.png", &images[0].width, &images[0].height, 0, 4); // RGBA channels 
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Shaders & shader programs
    std::unique_ptr<Shader> texShaderFrag = Shader::Load("texture.frag.glsl");
    std::unique_ptr<Shader> texShaderVert = Shader::Load("texture.vert.glsl");

    std::unique_ptr<Shader> billboardShaderFrag = Shader::Load("billboard.frag.glsl");
    std::unique_ptr<Shader> billboardShaderVert = Shader::Load("billboard.vert.glsl");

    ShaderProgram textureprog = { texShaderFrag.get(), texShaderVert.get() };
    ShaderProgram billboardprog = { billboardShaderFrag.get(), billboardShaderVert.get() };

    // All rendering stuff below
    //--------------------------------------------------------------------------------------------------

    float vertices[] = {
        // Positions          // Normals           // TexCoords
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
          0.5f,  0.5f , 0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
          0.5f,  0.5f , 0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         -0.5f,  0.5f , 0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };
    float quadVertices[] = {
        // Positions     // TexCoords
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // Top left
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom left
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Bottom right
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f  // Top right
    };
    unsigned int quadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
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

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // Note that we update the light's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint quadVAO, quadVBO, quadEBO;
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

    //--------------------------------------------------------------------------------------------------

    // ImGUI stuff below
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        io.Fonts->AddFontFromFileTTF("fonts/Roboto/static/Roboto-Regular.ttf", 15.0f);

        ImFont* win_title_font = io.Fonts->AddFontFromFileTTF("fonts/Roboto/static/Roboto-Bold.ttf", 18.0f);

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    unsigned int missingTexture = icl::loadTexture("assets/textures/missingTexture.png");
    unsigned int containerTexture = icl::loadTexture("assets/textures/container2.png");
    unsigned int specular_containerTexture = icl::loadTexture("assets/textures/container2_specular.png");
    unsigned int iconTex = icl::loadTexture("assets/icons/editor_PointLight.png");

    glUseProgram(textureprog.get_id());
    textureprog.set_uniform("material.diffuse", 0);
    textureprog.set_uniform("material.specular", 1);

    root->Init();

    auto child1 = std::make_unique<Node>();
    auto child2 = std::make_unique<Node>();
    auto child3 = std::make_unique<Node>();
    auto child4 = std::make_unique<Node>();
    auto testt = std::make_unique<test>();
    auto testtt = std::make_unique<test2>();
    child4->name = "a";

    testt->AddChild(std::move(testtt));
    root->AddChild(std::move(testt));
    root->AddChild(std::move(child1));
    root->GetChild(0)->AddChild(std::move(child2));
    root->GetChild(0)->GetChild(0)->AddChild(std::move(child3));
    root->GetChild(0)->GetChild(0)->GetChild(0)->AddChild(std::move(child4));

    std::cout << root->GetChild(0)->GetChild(0)->GetChild(0)->GetChild(0)->name;

    // Process loop
    while (!glfwWindowShouldClose(window)) {

        // Frame stuff
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        root->Update(deltaTime);

        // Process all the input
        processInput(window);

        // Render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show main window menu
        showEditorMenu();

        if (ShowImGUIDemo)
            ImGui::ShowDemoWindow();

        glUseProgram(textureprog.get_id());

        // Render object
        //textureprog.set_uniform("light.position", lightPos);
        textureprog.set_uniform("light.direction", lightDir);
        textureprog.set_uniform("viewPos", camera.Position);

        // Set projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        textureprog.set_uniform("projection", projection);

        // Set view matrix
        glm::mat4 view = camera.GetViewMatrix();
        textureprog.set_uniform("view", view);

        textureprog.set_uniform("light.ambient", glm::vec3(0.0f));
        textureprog.set_uniform("light.diffuse", lightColor); // Diffuse is the main color of the light
        lightColor[0] = static_cast<float>(sin(glfwGetTime() / 1.9 + 0.6));
        lightColor[1] = static_cast<float>(sin(glfwGetTime() / 2.1 + 0.4));
        lightColor[2] = static_cast<float>(sin(glfwGetTime() / 2.0 + 0.25));
        textureprog.set_uniform("light.specular", glm::vec3(1.0f));

        textureprog.set_uniform("material.shininess", 4.0f);

        // World transformation
        static float rotation_degrees = 0.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation_degrees), glm::vec3(0.0f, 1.0f, 0.0f));
        textureprog.set_uniform("model", model);

        // Render the cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containerTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_containerTexture);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*for (float x = 0.0f; x < 20.0f; x += 0.5f) {
            for (float y = 0.0f; y < 20.0f; y += 0.5f) {
                for (float z = 0.0f; z < 20.0f; z += 0.5f) {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x, y, z));
                    textureprog.set_uniform("model", model);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }*/

        // Render light source's icon
        glUseProgram(billboardprog.get_id());
        billboardprog.set_uniform("view", view);
        billboardprog.set_uniform("projection", projection);
        billboardprog.set_uniform("billboardPos", lightPos);
        billboardprog.set_uniform("size", 0.5f); // size in world units
        billboardprog.set_uniform("tintColor", lightColor);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, iconTex);
        billboardprog.set_uniform("iconTexture", 0);

        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // ImGui window setup
        if (ShowIclDebug) {
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

                    if (ImGui::DragFloat("Box rotation degrees", &rotation_degrees, 0.1f, 0.0f, 360.0f)) {
                        if (rotation_degrees == 360.0f) rotation_degrees = 0.0f;
                    }

                    ImGui::Text("Light position: ");
                    if (ImGui::DragFloat("px", &lightPos[0], 0.1f));
                    if (ImGui::DragFloat("py", &lightPos[1], 0.1f));
                    if (ImGui::DragFloat("pz", &lightPos[2], 0.1f));

                    ImGui::Text("Light direction: ");
                    if (ImGui::DragFloat("rx", &lightDir[0], 0.1f));
                    if (ImGui::DragFloat("ry", &lightDir[1], 0.1f));
                    if (ImGui::DragFloat("rz", &lightDir[2], 0.1f));
                }
                // Category for ImGui things
                if (ImGui::CollapsingHeader("Window")) {
                    if (ImGui::Button("Reset debug window size")) {
                        ImGui::SetWindowSize("IceLoft Debug", ImVec2(500, 200));
                    }
                    if (ImGui::Button("Reset editor window size")) {
                        glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
                        glfwRestoreWindow(window);
                    }
                }
            }

            // ImGui window end
            ImGui::End();
        }

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

    root->Destroy();

    // Clear ImGui processes
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // De-allocate outlived purpose resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);

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
    /*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);*/ // Close window when 'Esc' key is pressed
    if (IsKeyPressedOnce(GLFW_KEY_Q))
        ShowImGUIDemo = !ShowImGUIDemo;

    // Camera input
    if (canControl) {
        if (Input::is_key_pressed(GLFW_KEY_W))
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (Input::is_key_pressed(GLFW_KEY_S))
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (Input::is_key_pressed(GLFW_KEY_A))
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (Input::is_key_pressed(GLFW_KEY_D))
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_REPEAT || Input::is_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
        if (IsKeyPressedOnce(GLFW_KEY_F3)) {
            ShowPerformanceWindow = !ShowPerformanceWindow;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_REPEAT || Input::is_key_pressed(GLFW_KEY_LEFT_ALT)) {
        if (IsKeyPressedOnce(GLFW_KEY_ENTER)) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            if (!IsFullscreen) {
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
            }
            else {
                glfwSetWindowMonitor(window, NULL, mode->width / 7, mode->height / 7, SCR_WIDTH, SCR_HEIGHT, GLFW_DONT_CARE);
            }
            IsFullscreen = !IsFullscreen;
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
            if (ImGui::MenuItem("Show Iceloft debug menu", "idk", &ShowIclDebug)) {}
            if (ImGui::MenuItem("Show ImGUI debug menu", "Q", &ShowImGUIDemo)) {}
            if (ImGui::MenuItem("Show Performance debug menu", "SHIFT+F3", &ShowPerformanceWindow)) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

//void showIclDebugMenu;

std::unordered_map<int, bool> keyWasDownLastFrame;

bool IsKeyPressedOnce(int key) {
    bool isDownNow = Input::is_key_pressed(key);
    bool wasDownBefore = keyWasDownLastFrame[key];

    keyWasDownLastFrame[key] = isDownNow;

    return isDownNow && !wasDownBefore;
}

// Whenever the mouse moves this GLFW callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (canControl) {
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
        camera.ProcessMouseScroll((float)yoffset);
    }
}

// Whenever the mouse button is pressed this GLFW callback is called
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        canControl = true;
    else
        canControl = false;
}

