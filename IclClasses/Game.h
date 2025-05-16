#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Represents the current state of the game (add more states if needed)
enum GameState {
    GAME_ACTIVE
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    // Constructor/Destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // Initialize game state (load all resources)
    void Init();
    // Game loop
    
    // Processes all the window input
    void ProcessInput(float delta);
    // This function gets called every frame
    void Update(float delta);
    // For rendering
    void Render();
};

#endif