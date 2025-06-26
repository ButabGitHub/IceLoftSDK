#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Node {
private:
    Node* parent;
    std::vector<std::unique_ptr<Node>> children; // Should I use vector or array for this??

public:
    std::string name;

    // Lifecycle stuff
    virtual void Init();
    virtual void Update(double delta);
    virtual void Destroy(); // End is such an awful names for this function (probably lol) but I'll keep it like that for now

    // Node interaction stuff
    void AddChild(std::unique_ptr<Node>);

    std::unique_ptr<Node> GetChild(int child); // GetChild by it's ID
    std::unique_ptr<Node> GetChild(std::string child); // GetChild by it's name

    std::vector<std::unique_ptr<Node>> GetChildren();

    // Idk what I can add here

};