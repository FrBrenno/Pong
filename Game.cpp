#include <iostream>

#include "ECS/Components.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "TextureManager.hpp"

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
Map *map;
Manager manager;
// std::vector<ColliderComponent *> Game::colliders;

auto &player(manager.addEntity());

Game::Game() {}
Game::~Game() {}

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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!" << std::endl;
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    map = new Map();
    Map::loadMap(32, 1, windowHeight, windowWidth);

    player.addComponents<TransformComponent>(64, 64, 32, 32, 2);
    player.addComponents<SpriteComponent>(00);
    player.addGroup(groupPlayers);
}

auto &players(manager.getGroup(Game::groupPlayers));

void Game::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();
    /* for (auto cc : colliders)
    {
        Collision::AABB(soldierA.getComponent<ColliderComponent>(), *cc);
    } */
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // Render things here
    /* for (auto &t : tiles)
    {
        t->draw();
    } */
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

bool Game::running() { return isRunning; }

void Game::AddTile(int srcX, int srcY, int x, int y, int tsize, int tscale)
{
    auto &tile(manager.addEntity());
    // tile.addComponents<TileComponent>(x, y, tsize, tscale, "assets/pong_spritesheet.png");
    tile.addGroup(groupMap);
}
