#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Node {
public:
    std::string name;

    Node();
    ~Node();

    // Lifecycle stuff

    /// <summary> Called on the first appearance of the node in the tree </summary>
    virtual void Init();

    /// <summary> 
    /// Called every frame 
    /// </summary>
    /// <param name="Delta ">time elapsed since the previous frame </param>
    virtual void Update(double delta);

    /// <summary>
    /// Called every time input is being processed
    /// </summary>
    virtual void ProcessInput();

protected:
    virtual void End();

public:

    // Node interaction stuff
    void AddChild(std::unique_ptr<Node>);

    std::unique_ptr<Node>& GetChild(const int child); // GetChild by it's ID
    std::unique_ptr<Node>& GetChild(const std::string& child); // GetChild by it's name

    /// <summary>
    /// Returns the parent
    /// </summary>
    /// <returns>Node*</returns>
    Node* GetParent();

    void Destroy(); // Destroy the Node
    
    std::vector<std::unique_ptr<Node>>& GetChildren();

    // Idk what I can add here

private:
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

};