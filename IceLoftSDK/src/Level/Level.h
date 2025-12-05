#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Engine/DataStructs.h"
#include "Base/Node.h"
#include "Base/Object.h"

#include <string>

class Level : IObject {
public:
	Level();
	~Level();

	void Enter() override final;
	void Update(double Delta) override final;
	void Exit() override final;
	void ProcessInput() override final;

	void AddNode(std::unique_ptr<Node> node);
	
	std::unique_ptr<Node>& GetNode(const int node);
	std::unique_ptr<Node>& GetNode(const std::string& node);

private:
	std::vector<std::unique_ptr<Node>> nodes;
};