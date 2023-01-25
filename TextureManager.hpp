#pragma once

#include "Game.hpp"

/// @brief Manages any texture of the game
class TextureManager
{
public:
    /// @brief Loads an image file and generates a SDL_Texture* of it
    /// @param filename Path to the image
    /// @return SDL_Texture of the image
    static SDL_Texture* LoadTexture(const char* filename);
    /// @brief Draws the src rectangle of the texture in the dest rectangle
    /// @param texture texture of the object
    /// @param src source rectangle, which part of the source texture will be drawn
    /// @param dest destination rectangle, where the texture will be drawn
    static void Draw(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest);
};