#include "Game.hpp"

class Map
{
public:

    /// @brief Initialises the map by loading all textures, loading the map grip and fixing the source,
    // destination rectangles size and position
    Map();
    ~Map();

    /// @brief Loads the grid map
    static void loadMap(int tsize, int tscale, int windowHeight, int windowWidth);
};