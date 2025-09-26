#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Engine/DataStructs.h"

class EntryPoint {
public:
	static int Enter();
};

class Game {
public:
	Game();
	~Game();
	Game(Game& game) = delete;
	Game& operator=(Game& game) = delete;

	template<typename T> void AddLevel() {

	}
};
