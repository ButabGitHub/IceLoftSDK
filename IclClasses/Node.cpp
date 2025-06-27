#include "Node.h"

void Node::Init() {
	for (const std::unique_ptr<Node>& child : children) {
		child->Init();
	}
}

std::unique_ptr<Node>& Node::GetChild(const int child) {
    if (child < 0 || child >= static_cast<int>(children.size()))
        std::cerr << "\x1b[38;5;9m\ Invalid child index: " << child << ", at the node named \"" << this->name << "\" .\n";

	return children.at(child);
}

std::unique_ptr<Node>& Node::GetChild(const std::string& child) {
    for (auto& _child : children) {
        if (_child->name == child) {
            return _child;
        }
    }
    std::cerr << "\x1b[38;5;9m\ Invalid child name: " << child << ", at the node named \"" << this->name << "\" .\n";
}

