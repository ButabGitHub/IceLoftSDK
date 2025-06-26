#include "Node.h"

void Node::Init() {
	for (const std::unique_ptr<Node>& child : children) {
		child->Init();
	}
}

std::unique_ptr<Node>& Node::GetChild(const int child) {
	return children.at(child);
}

//std::unique_ptr<Node>& Node::GetChild(const std::string child) {
//	for (const std::unique_ptr<Node>& _child : children) {
//		if (_child.name == child) {
//			return children.at(child);
//		}
//	}
//}
