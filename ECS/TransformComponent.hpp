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
        TransformComponent();
        scale = sc;
    }
    TransformComponent(float x, float y)
    {
        position = Vector2D(x, y);
    }
    TransformComponent(float xpos, float ypos, float xvel, float yvel)
    {
        position = Vector2D(xpos, ypos);
        velocity = Vector2D(xvel, yvel);
    }
    TransformComponent(float x, float y, int h, int w, int sc)
    {
        position = Vector2D(x, y);
        height = h;
        width = w;
        scale = sc;
    }
    TransformComponent(float xpos, float ypos, float xvel, float yvel, int h, int w, int sc)
    {
        position = Vector2D(xpos, ypos);
        velocity = Vector2D(xvel, yvel);
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