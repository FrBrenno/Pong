#include "../Vector2D.hpp"
#include "Components.hpp"

#define HEIGHT 32
#define WIDTH 32
#define SCALE 1
#define SPEED 3

class TransformComponent : public Component
{
public:
    Vector2D position;
    Vector2D velocity;

    int height = HEIGHT;
    int width = WIDTH;
    int scale = SCALE;
    int speed = SPEED;

    TransformComponent()
    {
        position.Zero();
    }
    TransformComponent(int sc)
    {
        position.Zero();
        scale = sc;
    }
    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    TransformComponent(float x, float y, int w, int h, int sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }

    void init() override
    {
        velocity.Zero();
    }

    void update() override
    {
        position += velocity*speed;
    }


};