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
    int tiles[] = {04, 14};
    int selectedTile = tiles[rand() % (sizeof(tiles) / sizeof(int))];
    int nX = ceil(windowHeight/ tsize);
    int nY = ceil(windowWidth / tsize);

    int srcX, srcY; 

    for (int y = 0; y < nY; y++)
    {
        for (int x = 0; x < nX; x++)
        {
            srcX = x;
            srcY = y;
            //Game::AddTile(selectedTile, srcX, srcY, x * (tsize * tscale), y * (tsize * tscale), tsize, tscale);
        }
    }
}
