#include "Node.h"

#include <algorithm>

Node::Node() {

}

Node::~Node() {

}

void Node::Init() {
	for (const std::unique_ptr<Node>& child : children) {
		child->Init();
	}
}

void Node::Update(double delta) {
    for (const std::unique_ptr<Node>& child : children) {
        child->Update(delta);
    }
}

void Node::ProcessInput() {
    for (const std::unique_ptr<Node>& child : children) {
        child->ProcessInput();
    }
}

void Node::End() {
    for (const std::unique_ptr<Node>& child : children) {
        child->End();
    }
}

void Node::AddChild(std::unique_ptr<Node> child) {
    child->parent = this;
    child->Init();
    children.push_back(std::move(child));
}

std::unique_ptr<Node>& Node::GetChild(const int child) {
    if (child < 0 || child >= static_cast<int>(children.size())) {
        std::cerr << "\x1b[38;5;9m\ Invalid child index: " << child << ", at the node named \"" << this->name << "\" .\n";
        std::runtime_error("Invalid child index");
    }
    else
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

Node* Node::GetParent() {
    if (parent)
        return parent;
    else
        std::cerr << "\x1b[38;5;9m\ Node named \"" << this->name << "\" has no parent.\n";
        return nullptr;
}

std::vector<std::unique_ptr<Node>>& Node::GetChildren() {
   return children;
}

void Node::Destroy() {
    // Call the End() first
    this->End();

    // Check if this Node has children and if so, call this function inside of them
    if (children.size() > 0) 
        for (const std::unique_ptr<Node>& child : children) {
            child->Destroy();
        }

    children.clear();
    
    /*auto node_unique_ptr = std::find_if(children.begin(), children.end(), [](std::unique_ptr<Node> node) {
            node.reset();
        }
    );*/

}
