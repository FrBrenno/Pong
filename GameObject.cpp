
#include "GameObject.hpp"
#include "TextureManager.hpp"

#define SCALE_FACTOR 2
#define SPRITE_SIZE 32

GameObject::GameObject(const char *texture_sheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texture_sheet);
    xpos = x;
    ypos = y;
}

void GameObject::Update()
{
    srcRect.h = SPRITE_SIZE;
    srcRect.w = SPRITE_SIZE;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.h = srcRect.h * SCALE_FACTOR;
    destRect.w = srcRect.w * SCALE_FACTOR;
}

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}