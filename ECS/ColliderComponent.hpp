#include "Components.hpp"
#include <string>

class ColliderComponent : public Component
{
private:
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Rect colliderBox;
    Vector2D normal;
    std::string tag;

public:
    ColliderComponent(std::string mTag) { tag = mTag; }
    ~ColliderComponent() {}

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
            entity->addComponents<TransformComponent>();

        transform = &entity->getComponent<TransformComponent>();
        normal.Zero();
        Game::colliders.push_back(this);
    }

    void update() override
    {
        colliderBox.x = static_cast<int>(transform->position.x);
        colliderBox.y = static_cast<int>(transform->position.y);
        colliderBox.h = transform->height * transform->scale;
        colliderBox.w = transform->width * transform->scale;
    }

    SDL_Rect getColliderBox() const { return colliderBox; }
    Vector2D getCollisionNormal() const { return normal; }
    std::string getTag() const { return tag; }

    void setCollisionNormal(ColliderComponent &other)
    {

        Vector2D vecA = entity->getComponent<TransformComponent>().position;
        Vector2D vecB = other.entity->getComponent<TransformComponent>().position;
        normal = vecB - vecA;
    }
};

class Collision
{
public:
    static bool AABB(ColliderComponent &colA, ColliderComponent &colB)
    {
        SDL_Rect recA = colA.getColliderBox();
        SDL_Rect recB = colB.getColliderBox();
        if (
            recA.x + recA.w - 20 >= recB.x &&
            recB.x + recB.w - 20 >= recA.x &&
            recA.y + recA.h >= recB.y &&
            recB.y + recB.h >= recA.y)
        {
            if (colA.entity != colB.entity)
            {
                colA.setCollisionNormal(colB);
                return true;
            }
        }
        return false;
    }

};