#ifndef Game_hpp
#define Game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

class ColliderComponent;

/// @brief Responsible of the game loop and game settings.
class Game
{

public:
    Game();
    ~Game();
    /// @brief Initialise a game object
    /// @param title Title of the window
    /// @param xpos X position of the window
    /// @param ypos Y position of the windows
    /// @param height Height of the window
    /// @param width Width of the window
    /// @param fullscreen Determine whether it is fullscreen or not
    void init(const char *title, int xpos, int ypos, int height, int width, bool fullscreen);

    /// @brief Handle user inputs/events
    void handleEvents();
    /// @brief Update all game objects
    void update(int deltaTime);
    /// @brief Render objects in the window
    void render();
    /// @brief Destroy the window and the renderer before quitting
    void clean();

    

    static void AddTile(int id, int x, int y, int size, int scale);

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent *> colliders;
    static bool isRunning;
    static int windowHeight, windowWidth;

     
    enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupBall
	};

private:
    SDL_Window *window;
};

#endif /* Game_hpp*/