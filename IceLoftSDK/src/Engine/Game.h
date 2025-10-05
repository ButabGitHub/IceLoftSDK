#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Engine/DataStructs.h"

class Game {
public:
	Game();
	~Game();
	Game(Game& game) = delete;
	Game& operator=(Game& game) = delete;

	void Init(GameConfig conf);
	void Enter();

	GLFWwindow*& GetWindow();

	template<typename T> void AddLevel() {

	}

private:
	GLFWwindow* GameWindow = nullptr;
};
