#include "Components.hpp"

class ScoreComponent : public Component
{
private:
    int score;
    TransformComponent *transform; // position
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

public:
    ScoreComponent(float posX, float posY, int width, int height, int scale, int mScore)
    {
        score = mScore;
        transform = new TransformComponent(posX, posY, width, height, scale);
        texture = TextureManager::LoadTexture("assets/pong_spritesheet.png");
        srcRect.x = 0;
        srcRect.y = 64;
    };
    ~ScoreComponent()
    {
        delete transform;
        if (texture)
            SDL_DestroyTexture(texture);
    };

    int getScore() const { return score; }
    
    void resetScore()
    {
        score = 0;
        srcRect.x = 0;
    }
    void incrementScore()
    {
        score++;
        srcRect.x = 32 * score;
    }

    void init() override
    {
        srcRect.w = transform->width;
        srcRect.h = transform->height;

        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
        destRect.h = transform->height * transform->scale;
        destRect.w = transform->width * transform->scale;
    }
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};
