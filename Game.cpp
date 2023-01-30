#include <iostream>
#include <random>

#include "ECS/Components.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
int Game::windowHeight;
int Game::windowWidth;
bool Game::isRunning = false;
Manager manager;
std::vector<ColliderComponent *> Game::colliders;

bool buttons[4] = {};
enum Buttons
{
    PaddleOneUp = 0,
    PaddleOneDown,
    PaddleTwoUp,
    PaddleTwoDown,
};

auto &player_1(manager.addEntity());
auto &player_2(manager.addEntity());
auto &ball(manager.addEntity());
auto &net(manager.addEntity());

auto &players(manager.getGroup(Game::groupPlayers));
auto &balls(manager.getGroup(Game::groupBall));

Game::Game() {}
Game::~Game()
{
    // Clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::init(const char *title, int xpos, int ypos, int height, int width, bool fullscreen)
{

    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems initialised..." << std::endl;
        // Create a window
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window)
        {
            windowHeight = height;
            windowWidth = width;
            std::cout << "Window created!" << std::endl;
        }
        // Create a renderer
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created!" << std::endl;
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    player_1.addComponents<TransformComponent>(64, 224, 13, 64, 2);
    player_1.addComponents<SpriteComponent>(0, 0, false, false); // Sprite coordonates
    player_1.addComponents<KeyboardControlledMovement>(6.0f);
    player_1.addComponents<ColliderComponent>("player_1");
    player_1.addGroup(groupPlayers);

    player_2.addComponents<TransformComponent>(windowWidth - 64 - 32, 224, 13, 64, 2);
    player_2.addComponents<SpriteComponent>(13, 0, false, false); // Sprite coordonates
    player_2.addComponents<KeyboardControlledMovement>(6.0f);
    player_2.addComponents<ColliderComponent>("player_2");
    player_2.addGroup(groupPlayers);

    ball.addComponents<TransformComponent>(windowWidth / 2 - 32, windowHeight / 2 - 32, 32, 32, 1);
    ball.addComponents<SpriteComponent>(35, 0, true, false); // Sprite coordonates
    ball.addComponents<BallMovement>(9.0f);
    ball.addComponents<ColliderComponent>("ball");
    ball.addGroup(groupBall);

    net.addComponents<TransformComponent>(windowWidth/2 - 4, 0, 9, 64, 1);
    net.addComponents<SpriteComponent>(26, 0, false, true);
    net.addGroup(groupMap);
}

void Game::handleEvents()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            else if (event.key.keysym.sym == SDLK_z)
            {
                buttons[Buttons::PaddleOneUp] = true;
            }
            else if (event.key.keysym.sym == SDLK_s)
            {
                buttons[Buttons::PaddleOneDown] = true;
            }
            else if (event.key.keysym.sym == SDLK_UP)
            {
                buttons[Buttons::PaddleTwoUp] = true;
            }
            else if (event.key.keysym.sym == SDLK_DOWN)
            {
                buttons[Buttons::PaddleTwoDown] = true;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.sym == SDLK_z)
            {
                buttons[Buttons::PaddleOneUp] = false;
            }
            else if (event.key.keysym.sym == SDLK_s)
            {
                buttons[Buttons::PaddleOneDown] = false;
            }
            else if (event.key.keysym.sym == SDLK_UP)
            {
                buttons[Buttons::PaddleTwoUp] = false;
            }
            else if (event.key.keysym.sym == SDLK_DOWN)
            {
                buttons[Buttons::PaddleTwoDown] = false;
            }
        }
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();
    for (auto &cc : colliders)
    {
        if (cc == &ball.getComponent<ColliderComponent>())
            continue;
        else
        {
            CollisionType collision;
            if (Collision::SATCollision(ball.getComponent<ColliderComponent>(), *cc, collision))
            {
                double currentTime = SDL_GetTicks() / 1000.0;
                if (currentTime - ball.getComponent<ColliderComponent>().getLastCollisionTime() >
                    ball.getComponent<ColliderComponent>().getcollisionCooldown())
                {

                    if (collision == CollisionType::LEFT || collision == CollisionType::RIGHT)
                    {
                        ball.getComponent<BallMovement>().direction.x *= -1;
                    }
                    else if (collision == CollisionType::TOP) 
                    {
                        ball.getComponent<BallMovement>().direction.x *= 1.1;
                        ball.getComponent<BallMovement>().direction.y *= -1.1;
                        ball.getComponent<BallMovement>().direction.Normalize();
                    }
                    else if (collision == CollisionType::BOTTOM)
                    {
                        ball.getComponent<BallMovement>().direction.x *= 0.9;
                        ball.getComponent<BallMovement>().direction.y *= 0.9;
                        ball.getComponent<BallMovement>().direction.Normalize();
                    }
                    ball.getComponent<ColliderComponent>().setLastCollisionTime(currentTime);
                }
            }
        }
    }

    // Updating Players position
    Vector2D direction_1 = {0, 0};
    Vector2D direction_2 = {0, 0};
    if (buttons[Buttons::PaddleOneDown])
        direction_1.y = 1.0f;
    else if (buttons[Buttons::PaddleOneUp])
        direction_1.y = -1.0f;
    
    if (buttons[Buttons::PaddleTwoDown])
        direction_2.y = 1.0f;
    else if (buttons[Buttons::PaddleTwoUp])
        direction_2.y = -1.0f;

    player_1.getComponent<KeyboardControlledMovement>().setDirection(direction_1);
    player_2.getComponent<KeyboardControlledMovement>().setDirection(direction_2);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // Render things here
    net.draw();
    for (auto &b : balls)
    {
        b->draw();
    }
    for (auto &p : players)
    {
        p->draw();
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
