#include "Map.hpp"
#include <random>
#include <stdio.h>
Map::Map()
{
}

Map::~Map()
{
}

void Map::loadMap(int tsize, int tscale, int windowHeight, int windowWidth)
{
    srand(time(NULL));
    int tiles[] = {05, 15};
    int selectedTile = tiles[rand() % (sizeof(tiles) / sizeof(int))];
    int nX = ceil(windowWidth / tsize);
    int nY = ceil(windowHeight / tsize);

    for (int y = 0; y < nY; y++)
    {
        for (int x = 0; x < nX; x++)
        {
            Game::AddTile(selectedTile, x * (tsize * tscale), y * (tsize * tscale), tsize, tscale);
        }
    }
}
