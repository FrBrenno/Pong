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
        texture = TextureManager::LoadTexture("/assets/soldier_a.png");
        spriteID = mSpriteID;
        
    }
    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = (int)(spriteID / 10);
        srcRect.w = 32;
        if (spriteID == 00 || spriteID == 10)
        {
            srcRect.y = 2 * (int)(spriteID % 10);
            srcRect.h = 64;
        }

        else
        {
            srcRect.y = (int)(spriteID % 10);
            srcRect.h = 32;
        }

        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
        destRect.h = (transform->height * transform->scale);
        destRect.w = (transform->width * transform->scale);
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
