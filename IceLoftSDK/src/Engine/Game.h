#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Engine/DataStructs.h"
#include "Engine/LevelManager.h"

// Defines
#define CUSTOM_CLOSING_LOGIC 0 // If true (1), gives ability to manually handle when the window would close

class Game {
public:
	Game();
	~Game();
	Game(Game& game) = delete;
	Game& operator=(Game& game) = delete;

	void Init(GameConfig conf);
	void Enter(); // Run the Game

	LevelManager& GetLevelManager();

	GLFWwindow*& GetWindow();

private:
	// Window-related stuff
	GLFWwindow* GameWindow = nullptr;
#if CUSTOM_CLOSING_LOGIC
	bool WindowShouldClose = false;
#endif

	// Level manager
	LevelManager LvManager;

	// DeltaTime-related stuff
	double Delta = 0.0;

};
