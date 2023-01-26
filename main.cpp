#include <SDL2/SDL.h>
#include <iostream>

#include "Game.hpp"

int main()
{
    const int FPS = 60;
    const float frameDelay = 1.0f / FPS;
    float lastFrameTime = 0;
    float deltaTime = 0;

    Game *game = new Game();
    game->init("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 800, false);

    while (game->isRunning)
    {
        float currentTime = SDL_GetTicks() / 1000.0f;
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        game->update(deltaTime);
        game->render();
        game->handleEvents();

        float delay = frameDelay - (SDL_GetTicks()/1000.0f - currentTime);
        if (delay > 0)
        {
            SDL_Delay(delay*1000.0f);
        }
        /* std::cout << "Average FPS " << 1.0f / deltaTime << std::endl;
        std::cout << "Delay: " << delay << std::endl; */
    }
    delete game;
    return 0;
}