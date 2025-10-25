#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Engine/DataStructs.h"
#include "Level/Level.h"

// Defines
#define CUSTOM_CLOSING_LOGIC 0 // If true (1), gives ability to manually handle when the window would close

class Game {
public:
	Game();
	~Game();
	Game(Game& game) = delete;
	Game& operator=(Game& game) = delete;

	// 
	void Init(GameConfig conf); // Initialise the Game
	void Enter(); // Run the Game

	GLFWwindow*& GetWindow();

	template<typename T> void AddLevel() {

	}

private:
	// Window-related stuff
	GLFWwindow* GameWindow = nullptr;
	bool WindowShouldClose = false;

	// DeltaTime-related stuff
	float delta = 0.0f;

};
