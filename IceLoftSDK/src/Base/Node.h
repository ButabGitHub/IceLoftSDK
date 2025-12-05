#pragma once

#include "Base/Object.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Node : public IObject {
public:
    std::string name;

    Node();
    ~Node();

    // Non-overridable lifecycle functions, 
    /// <summary> 
    /// Called on the first appearance of the node in the tree 
    /// </summary>
    void Enter() override final;
    /// <summary> 
    /// Called every frame 
    /// </summary>
    /// <param name="Delta ">time elapsed since the previous frame </param>
    void Update(double Delta) override final;
    /// <summary> 
    /// Called when the node exits the tree
    /// </summary>
    void Exit() override final;
    /// <summary>
    /// Called every time input is being processed
    /// </summary>
    void ProcessInput() override final;

    // Node interaction functions
    void AddChild(std::unique_ptr<Node>);

    std::unique_ptr<Node>& GetChild(const int child); // GetChild by its index
    std::unique_ptr<Node>& GetNode(const std::string& NodePath);

    /// <summary>
    /// Returns the parent
    /// </summary>
    /// <returns>Node*</returns>
    Node* GetParent();

    void Destroy(); // Destroy the Node
    
    std::vector<std::unique_ptr<Node>>& GetChildren();

private:
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

};