#include "Components.hpp"

#include <SDL2/SDL.h>

class TileComponent : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    SDL_Rect tileRect;
    int tileID, tileSize, tileScale;

    TileComponent() = default;

    ~TileComponent()
    {};

    TileComponent(int id, int x, int y, int size, int scale)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileSize = size;
        tileID = id;
        tileScale = scale;
    }

    void init() override
    {
        entity->addComponents<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileSize, tileSize, tileScale);
        transform = &entity->getComponent<TransformComponent>();

        entity->addComponents<SpriteComponent>(tileID);
        sprite = &entity->getComponent<SpriteComponent>();
    }

    
};