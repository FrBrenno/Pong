#include "../TextureManager.hpp"
#include "Components.hpp"

#include <SDL2/SDL.h>
class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    int spriteID;

public:
    SpriteComponent() = default;
    SpriteComponent(int mSpriteID)
    {
        texture = TextureManager::LoadTexture("assets/pong_spritesheet.png");
        spriteID = mSpriteID;
    }
    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = (int)(spriteID / 10)*32;
        srcRect.y = (int)(spriteID % 10)*32;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    
        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
        destRect.h = transform->height * transform->scale;
        destRect.w = transform->width * transform->scale;
    }

    void update(int deltaTime) override
    {
        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
    }
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
