#include <SDL2/SDL.h>
#include <sstream>

#include "Game.hpp"

int main()
{
    const int FPS = 60;
    const float frameDelay = 1.0f / FPS;
    float lastFrameTime = 0;
    float deltaTime = 0;
    int numFrames = 0;
    float totalTime = 0;

    Game *game = new Game();
    game->init("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 800, false);

    while (game->isRunning)
    {
        float currentTime = SDL_GetTicks() / 1000.0f;
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        totalTime += deltaTime;
        numFrames++;

        game->update(deltaTime);
        game->render(deltaTime);
        game->handleEvents();

        float delay = frameDelay - deltaTime;
        if (delay > 0)
        {
            SDL_Delay(delay * 1000.0f);
        }

        if (numFrames == FPS)
        {
            int averageFPS = numFrames / totalTime;
            std::stringstream title;
            title << "PONG - Average FPS: "  << averageFPS ;
            SDL_SetWindowTitle(game->getWindow(), title.str().c_str());

            numFrames = 0;
            totalTime = 0;
        }
    }
    delete game;
    return 0;
}