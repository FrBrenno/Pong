#include "TextureManager.hpp"
#include <SDL2/SDL.h>

SDL_Texture *TextureManager::LoadTexture(const char *filename)
{
    SDL_Surface *tmpSurface = IMG_Load(filename);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return texture;
}

void TextureManager::Draw(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}