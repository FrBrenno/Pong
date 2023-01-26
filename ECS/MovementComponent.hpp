#include "Components.hpp"

class MovementComponent : public Component
{
public:
    TransformComponent *transform;
    Vector2D velocity; // Direction
    float speed = 2;

    MovementComponent(Vector2D mVelocity)
    {
        velocity = mVelocity.Normalize();
    }
    MovementComponent(Vector2D mVelocity, float mSpeed)
    {
        velocity = mVelocity.Normalize();
        speed = mSpeed;
    }
    ~MovementComponent(){};

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    bool outOfTheWindow()
    {
        return transform->position.x < 0 ||
               transform->position.x + transform->width * transform->scale > Game::windowWidth ||
               transform->position.y < 0 ||
               transform->position.y + transform->height * transform->scale > Game::windowHeight;
    }
};

class MouseControlledMovement : public MovementComponent
{
public:
    MouseControlledMovement(Vector2D mVelocity) : MovementComponent(mVelocity){};

    void update(int deltaTime) override
    {
        int yMouse;
        SDL_GetMouseState(NULL, &yMouse);
        yMouse -= 128;
        if (yMouse < 0)
            yMouse = 0;
        if (yMouse > Game::windowHeight)
            yMouse = Game::windowHeight - 128;

        transform->position.y = yMouse;
    }
};

class AIControlledMovement : public MovementComponent
{
public:
    AIControlledMovement(Vector2D mVelocity, float mSpeed) : MovementComponent(mVelocity, mSpeed){};

    void update(int deltaTime) override
    {
        std::cout << transform->position.y + transform->height * transform->scale << "(" << Game::windowHeight << ")" << std::endl;
        if (
            (transform->position.y) < 0 ||
            (transform->position.y + transform->height * transform->scale) > Game::windowHeight)
        {
            velocity.y *= -1;
        }
        transform->position.y += velocity.y * speed;
    }
};