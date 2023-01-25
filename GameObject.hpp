#pragma once

#include "Game.hpp"

/// @brief Every element in the game is a game object
class GameObject
{
public:
    /// @brief Initialise the game object by setting it texture and position
    /// @param texture_sheet path to image
    /// @param x x position
    /// @param y y position
    GameObject(const char *texture_sheet, int x, int y);
    ~GameObject();

    /// @brief Updates object source and destination rectangles
    void Update();
    /// @brief Renders the object in the window
    void Render();

private:
    int xpos, ypos;             // Position of the object in the window
    SDL_Texture *objTexture;    // Object texture
    SDL_Rect srcRect, destRect; // Object rectangles
};