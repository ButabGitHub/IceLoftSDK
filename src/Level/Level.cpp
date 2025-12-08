#include "Level/Level.h"

Level::Level() {

}

Level::~Level() {
	
}

void Level::Update(double Delta) {
	OnUpdate(Delta);
	for (auto& _node : nodes) {
		_node->Update(Delta);
	}
	// ...
}

void Level::AddNode(std::unique_ptr<Node> node) {
	nodes.push_back(node);
}

std::unique_ptr<Node>& Level::GetNode(const int node) {
	if (node < 0 || node >= static_cast<int>(nodes.size())) {
		std::cerr << "\x1b[38;5;9m\ Invalid child index: " << node << ".\n";
		std::runtime_error("Invalid node index");
	}
	else
		return nodes.at(node);
}

std::unique_ptr<Node>& Level::GetNode(const std::string& node) {
	for (auto& _node : nodes) {
		if (_node->name == node) {
			return _node;
		}
	}
	std::cerr << "\x1b[38;5;9m\ Invalid child name: " << node << ".\n";
}
