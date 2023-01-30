#include "../TextureManager.hpp"
#include "Components.hpp"

#include <SDL2/SDL.h>
#include <map>

struct Animation
{
    int index, frames, speed;
};
class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    bool isRepeatedY = false;

    bool isAnimated = false;
    int frames = 0;
    int speed = 100;

public:
    int animIndex = 0;
    std::map<const char *, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(int srcRectX, int srcRectY, bool animation, bool repeatY)
    {
        texture = TextureManager::LoadTexture("assets/pong_spritesheet.png");
        srcRect.x = srcRectX;
        srcRect.y = srcRectY;
        isAnimated = animation;
        isRepeatedY = repeatY;

        if (isAnimated)
        {
            Animation rotating = {0, 4, 100};
            animations.emplace("Rotating", rotating);
            Play("Rotating");
        }
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

        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.h = transform->height * transform->scale;
        destRect.w = transform->width * transform->scale;
    }

    void update() override
    {
        if (isAnimated)
            srcRect.x = 36 + srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
    }

    void draw() override
    {
        int repetition = 0;
        if (isRepeatedY)
        {
            repetition = (int)ceil(Game::windowHeight / (destRect.h));
            for (int i = 0; i < repetition; i++)
            {
                TextureManager::Draw(texture, srcRect, destRect);
                destRect.y += destRect.h;
            }
        }
        else
        {
            TextureManager::Draw(texture, srcRect, destRect);
        }
    }

    void Play(const char* animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
};

