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

auto &player(manager.addEntity());
auto &computer(manager.addEntity());
auto &ball(manager.addEntity());

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

    player.addComponents<TransformComponent>(64, 224, 32, 64, 2);
    player.addComponents<SpriteComponent>(00);
    player.addComponents<MouseControlledMovement>(Vector2D(0, 1));
    player.addComponents<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    computer.addComponents<TransformComponent>(windowWidth - 64 - 32, 224, 32, 64, 2);
    computer.addComponents<SpriteComponent>(10);
    computer.addComponents<AIControlledMovement>(Vector2D(0, 1), 3);
    computer.addComponents<ColliderComponent>("computer");
    computer.addGroup(groupPlayers);

    ball.addComponents<TransformComponent>(windowWidth / 2 - 32, windowHeight / 2 - 32);
    ball.addComponents<SpriteComponent>(02);
    ball.addComponents<BallMovement>();
    ball.addComponents<ColliderComponent>("ball");
    ball.addGroup(groupBall);
}

auto &players(manager.getGroup(Game::groupPlayers));
auto &balls(manager.getGroup(Game::groupBall));

void Game::handleEvents()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
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
                        ball.getComponent<BallMovement>().velocity.x *= -1;
                    }
                    else if (collision == CollisionType::TOP || collision == CollisionType::BOTTOM)
                    {
                        ball.getComponent<BallMovement>().velocity.y *= -1;
                    }
                    ball.getComponent<ColliderComponent>().setLastCollisionTime(currentTime);
                }
            }
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // Render things here
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
