#include "Components.hpp"

class MovementComponent : public Component
{
public:
    TransformComponent *transform;
    Vector2D direction;
    float speed = 3;
    MovementComponent(float mSpeed)
    {
        speed = mSpeed;
    }
    MovementComponent(Vector2D mdirection)
    {
        direction = mdirection.Normalize();
    }
    MovementComponent(Vector2D mdirection, float mSpeed)
    {
        direction = mdirection.Normalize();
        speed = mSpeed;
    }
    ~MovementComponent(){};

    void init() override
    {
        direction.Zero();
        transform = &entity->getComponent<TransformComponent>();
    }

};

class KeyboardControlledMovement : public MovementComponent
{
public:
    KeyboardControlledMovement(float speed) : MovementComponent(speed){};

    void update(float deltaTime) override
    {
        transform->position += direction * speed;
        if (transform->position.y + transform->height * transform->scale > Game::windowHeight)
            transform->position.y = Game::windowHeight - transform->height * transform->scale;
        else if (transform->position.y < 0)
            transform->position.y = 0;
    }
    void setDirection(Vector2D mDirection)
    {
        direction = mDirection;
    }
};

class BallMovement : public MovementComponent
{
public:
    BallMovement(float speed) : MovementComponent(speed) {}
    ~BallMovement() {}

    void init() override
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::bernoulli_distribution dist;

        int x_rand = dist(mt) ? -1 : 1;
        int y_rand = dist(mt) ? -1 : 1;

        direction = Vector2D(x_rand, y_rand).Normalize();

        transform = &entity->getComponent<TransformComponent>();
    }

    void update(float deltaTime) override
    {
        if ((transform->position.y + transform->height * transform->scale > Game::windowHeight) ||
            (transform->position.y < 0))
            direction.y *= -1;
        transform->position.x += direction.x * speed;
        transform->position.y += direction.y * speed;
    }
};