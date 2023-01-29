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
    bool isAnimated = false;

public:
    SpriteComponent() = default;
    SpriteComponent(int srcRectX, int srcRectY, bool animation)
    {
        texture = TextureManager::LoadTexture("assets/pong_spritesheet.png");
        srcRect.x = srcRectX;
        srcRect.y = srcRectY;
        isAnimated = animation;

    }
    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        srcRect.w = transform->width;
        srcRect.h = transform->height;
        
        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
        destRect.h = transform->height * transform->scale;
        destRect.w = transform->width * transform->scale;
    }

    void update() override
    {
        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
    }
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
