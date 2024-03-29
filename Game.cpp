#include <iostream>
#include <random>

#include "ECS/Components.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "Timer.hpp"

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
int Game::windowHeight;
int Game::windowWidth;
bool Game::isRunning = false;
Manager manager;
std::vector<ColliderComponent *> Game::colliders;
Timer timer;

bool buttons[4] = {};
enum Buttons
{
    PaddleOneUp = 0,
    PaddleOneDown,
    PaddleTwoUp,
    PaddleTwoDown,
};

int playerScored = 0;
auto &player_1(manager.addEntity());
auto &player_2(manager.addEntity());
auto &ball(manager.addEntity());
auto &net(manager.addEntity());
auto &titleLabel(manager.addEntity());

auto &players(manager.getGroup(Game::groupPlayers));
auto &balls(manager.getGroup(Game::groupBall));
auto &labels(manager.getGroup(Game::groupUI));

Game::Game() {}
Game::~Game()
{
    // Clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Clean up SDL Mixer
    Mix_CloseAudio();
    // Clean up SDL TTF
    TTF_Quit();
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

        // Initialize SDL_Mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cout << "Error: SDL_mixer could not initialize: " << Mix_GetError() << std::endl;
        }
    }
    else
    {
        isRunning = false;
    }

    // Initialize SDL_TTF
    if (TTF_Init() == -1)
        std::cout << "Error: SDL_TTF" << std::endl;

    SDL_Color white = {255, 255, 255};
    titleLabel.addComponents<UILabel>(16, 16, "PONG", 64, white);
    titleLabel.addGroup(groupUI);

    player_1.addComponents<TransformComponent>(64, 224, 13, 64, 2);
    player_1.addComponents<SpriteComponent>(0, 0, false, false); // Sprite coordonates
    player_1.addComponents<KeyboardControlledMovement>(7.0f);
    player_1.addComponents<ColliderComponent>("player_1");
    player_1.addComponents<ScoreComponent>(windowWidth / 2 - (32 + 64) * 2, 64, 32, 32, 2, 0);
    player_1.addGroup(groupPlayers);

    player_2.addComponents<TransformComponent>(windowWidth - 64 - 32, 224, 13, 64, 2);
    player_2.addComponents<SpriteComponent>(13, 0, false, false); // Sprite coordonates
    player_2.addComponents<KeyboardControlledMovement>(7.0f);
    player_2.addComponents<ColliderComponent>("player_2");
    player_2.addComponents<ScoreComponent>(windowWidth / 2 + 64 * 2, 64, 32, 32, 2, 0);
    player_2.addGroup(groupPlayers);

    ball.addComponents<TransformComponent>(windowWidth / 2 - 16, windowHeight / 2 - 16, 32, 32, 1);
    ball.addComponents<SpriteComponent>(35, 0, true, false); // Sprite coordonates
    ball.addComponents<BallMovement>(0.0f);
    ball.addComponents<ColliderComponent>("ball");
    ball.addComponents<SoundComponent>();
    ball.getComponent<SoundComponent>().addSound("paddle_hit", "assets/paddle_hit.wav");
    ball.getComponent<SoundComponent>().addSound("wall_hit", "assets/wall_hit.wav");
    ball.addGroup(groupBall);

    net.addComponents<TransformComponent>(windowWidth / 2 - 4, 0, 9, 64, 1);
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
            // Ball is stopped until player who has scored press any key
            if (ball.getComponent<BallMovement>().speed == 0.0f)
            {
                if ((playerScored == 1 && (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)) ||
                    (playerScored == 2 && (event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_s)) ||
                    (playerScored == 0))
                {
                    ball.getComponent<BallMovement>().speed = 6.0f;
                    ball.getComponent<SpriteComponent>().Play("Rotating");
                }
            }
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

void resetGame()
{
    // Centering the ball
    ball.getComponent<TransformComponent>().position = Vector2D(Game::windowWidth / 2 -
                                                                    ball.getComponent<TransformComponent>().width / 2,
                                                                Game::windowHeight / 2 -
                                                                    ball.getComponent<TransformComponent>().height / 2);
    ball.getComponent<BallMovement>().speed = 0.0f;
    ball.getComponent<SpriteComponent>().Play("Idle");
    // Centering the paddles
    player_1.getComponent<TransformComponent>().position = Vector2D(64, 224);
    player_2.getComponent<TransformComponent>().position = Vector2D(Game::windowWidth - 64 - 32, 224);
    timer.wait(1);
}

void gameover(std::string winner)
{
    std::cout << "The winner is " << winner << std::endl;
    player_1.getComponent<ScoreComponent>().resetScore();
    player_2.getComponent<ScoreComponent>().resetScore();
    playerScored = 0;
    resetGame();
}

void updateScore()
{
    TransformComponent ballTransform = ball.getComponent<TransformComponent>();
    Entity *scoringPlayer = nullptr;
    if (ballTransform.position.x < 0)
    {
        scoringPlayer = &player_2;
        playerScored = 2;
    }
    else if (ballTransform.position.x > Game::windowWidth)
    {
        scoringPlayer = &player_1;
        playerScored = 1;
    }

    if (scoringPlayer != NULL)
    {
        scoringPlayer->getComponent<ScoreComponent>().incrementScore();
        if (scoringPlayer->getComponent<ScoreComponent>().getScore() < 10)
        {
            resetGame();
        }
        else
        {
            gameover(scoringPlayer == &player_1 ? "Player 1" : "Player 2");
        }
    }
}

void updatePlayerDirection(Entity &player, Buttons upButton, Buttons downButton)
{
    Vector2D direction = {0, 0};
    if (buttons[downButton])
        direction.y = 1.0f;
    else if (buttons[upButton])
        direction.y = -1.0f;

    player.getComponent<KeyboardControlledMovement>().setDirection(direction);
}

void checkCollisions()
{
    TransformComponent &ballTransform = ball.getComponent<TransformComponent>();
    BallMovement &ballMovement = ball.getComponent<BallMovement>();
    ColliderComponent &ballCollider = ball.getComponent<ColliderComponent>();
    SoundComponent &ballSound = ball.getComponent<SoundComponent>();

    for (auto &cc : Game::colliders)
    {
        // Ball colliding with itself
        if (cc == &ballCollider)
            continue;

        // Colliders collision
        CollisionType collision = CollisionType::NONE;
        if (Collision::SATCollision(ballCollider, *cc, collision))
        {
            // check if collision cooldown is up
            double currentTime = SDL_GetTicks() / 1000.0;
            if (currentTime - ballCollider.getLastCollisionTime() > ballCollider.getcollisionCooldown())
            {
                // Ball hits the paddle
                ballSound.Play("paddle_hit");
                switch (collision)
                {
                case CollisionType::LEFT:
                case CollisionType::RIGHT:
                    ballMovement.direction.x *= -1;
                    ballMovement.direction.y += (rand() % 20 - 10) * 0.01;
                    break;
                case CollisionType::TOP:
                    ballMovement.direction.x *= 1.1;
                    ballMovement.direction.x += (rand() % 20 - 10) * 0.01;
                    ballMovement.direction.y *= -1.1;
                    break;
                case CollisionType::BOTTOM:
                    ballMovement.direction.x *= 0.9;
                    ballMovement.direction.x += (rand() % 20 - 10) * 0.01;
                    ballMovement.direction.y *= 0.9;
                    break;
                }
                ballMovement.direction.Normalize();
                ballMovement.speed += 0.13f;
                ballCollider.setLastCollisionTime(currentTime);
            }
        }

        // Wall collision
        if (
            (ballTransform.position.y) < 0 ||
            (ballTransform.position.y + ballTransform.height * ballTransform.scale) > Game::windowHeight)
        {
            ball.getComponent<SoundComponent>().Play("wall_hit");
            ballMovement.direction.y *= -1;
            ballMovement.direction.Normalize();
        }
    }
}

void Game::update(float deltaTime)
{
    manager.refresh();
    manager.update(deltaTime);
    checkCollisions();
    // Updating Players position
    updatePlayerDirection(player_1, Buttons::PaddleOneUp, Buttons::PaddleOneDown);
    updatePlayerDirection(player_2, Buttons::PaddleTwoUp, Buttons::PaddleTwoDown);
    // Updating Score
    updateScore();
}

void Game::render(float deltaTime)
{
    SDL_RenderClear(renderer);
    // Render things here
    net.draw();
    for (auto &l : labels)
    {
        l->draw();
    }
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
