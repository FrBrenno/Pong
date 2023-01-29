#include "Components.hpp"
#include <string>

class ColliderComponent : public Component
{
private:
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Rect colliderBox;
    std::string tag;
    double lastCollisionTime;
    double collisionCooldown = 0.5; // in seconds

public:
    ColliderComponent(std::string mTag) { tag = mTag; }
    ~ColliderComponent() {}

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
            entity->addComponents<TransformComponent>();

        transform = &entity->getComponent<TransformComponent>();

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
    double getLastCollisionTime() const { return lastCollisionTime; }
    void setLastCollisionTime(double lastCollisionT) { lastCollisionTime = lastCollisionT; }
    double getcollisionCooldown() const { return collisionCooldown; }
    std::string getTag() const { return tag; }
};

enum class CollisionType
{
    NONE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

class Collision
{

public:
    static bool SATCollision(ColliderComponent &colA, ColliderComponent &colB, CollisionType &collision)
    {
        // Get the bounding box of each collider
        SDL_Rect recA = colA.getColliderBox();
        SDL_Rect recB = colB.getColliderBox();

        // Calculate the distance between the two bounding boxes on the x and y axes
        int xDistance = (recA.x + (recA.w / 2)) - (recB.x + (recB.w / 2));
        int yDistance = (recA.y + (recA.h / 2)) - (recB.y + (recB.h / 2));

        // Calculate the sum of the widths and heights of the two bounding boxes
        int totalWidth = (recA.w / 2) + (recB.w / 2);
        int totalHeight = (recA.h / 2) + (recB.h / 2);

        // Check for collision on the x and y axes
        if (abs(xDistance) < totalWidth && abs(yDistance) < totalHeight)
        {
            // There is a collision
            int xPenetration = totalWidth - abs(xDistance);
            int yPenetration = totalHeight - abs(yDistance);

            // Determine the direction of the collision
            if (xPenetration < yPenetration)
            {
                // Collision is on the x axis
                if (xDistance > 0)
                {
                    collision = CollisionType::LEFT;
                }
                else
                {
                    collision = CollisionType::RIGHT;
                }
            }
            else
            {
                // Collision is on the y axis
                if (yDistance > 0)
                {
                    collision = CollisionType::TOP;
                }
                else
                {
                    collision = CollisionType::BOTTOM;
                }
            }
            return true;
        }

        // No collision
        return false;
    }
};