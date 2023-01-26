#include "../Vector2D.hpp"
#include "Components.hpp"

#define HEIGHT 32
#define WIDTH 32
#define SCALE 1
#define SPEED 2

class TransformComponent : public Component
{
public:
    Vector2D position;

    int height = HEIGHT;
    int width = WIDTH;
    int scale = SCALE;

    ~TransformComponent(){};

    TransformComponent()
    {
        position.Zero();
    }
    TransformComponent(float x, float y)
    {
        position = Vector2D(x, y);
    }
    TransformComponent(float x, float y, int w, int h, int sc)
    {
        position = Vector2D(x, y);
        height = h;
        width = w;
        scale = sc;
    }

    void init() override
    {
    }

    void update(int deltaTime) override
    {
    }
};