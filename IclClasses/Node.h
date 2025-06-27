#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Node {
private:
    Node* parent;
    std::vector<std::unique_ptr<Node>> children; // Should I use vector or array for this??
                                                 // Answer: I can't use array for this

public:
    std::string name;

    // Lifecycle stuff
    virtual void Init();
    virtual void Update(double delta);
    virtual void End(); // End is such an awful names for this function (probably lol) but I'll keep it like that for now

    // Node interaction stuff
    void AddChild(std::unique_ptr<Node>);

    std::unique_ptr<Node>& GetChild(const int child); // GetChild by it's ID
    std::unique_ptr<Node>& GetChild(const std::string& child); // GetChild by it's name

    void Destroy(); // Destroy the Node
    
    std::vector<std::unique_ptr<Node>> GetChildren();

    // Idk what I can add here

};