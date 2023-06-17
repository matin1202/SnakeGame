#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "block.h"

using namespace std;

struct SIZE
{
    int h, w;
};

class Map
{
public:
    SIZE size;
    SnakeHead head;
    vector<ImmunedWall> iWall;
    vector<Wall> wall;
    vector<Gate> gate = vector<Gate>(2);
    GrowthItem gItem;
    PoisonItem pItem;
    Map(int h = 21, int w = 21);
    Map(const Map &m);

    void print_map();
};

// void : 0, wall : 1, immune wall : -1, gate: 2, snake head: 3, snake body: 4

Map::Map(int h, int w)
{
    size.h = h;
    size.w = w;
    for (int i = 1; i <= h; i++)
    {
        for (int j = 1; j <= w; j++)
        {
            if (i == 1 || i == h)
            {
                if (j == 1 || j == w)
                    iWall.push_back(ImmunedWall(i, j));
                else
                    wall.push_back(Wall(i, j, (i == 1 ? 1 : 4)));
            }
            else
            {
                if (j == 1 || j == w)
                    wall.push_back(Wall(i, j, (j == 1 ? 2 : 3)));
            }
        }
    }
    head = SnakeHead(h / 2, w / 2);
    head.body.push_back(SnakeBody(h / 2 + 1, w / 2));
    head.body.push_back(SnakeBody(h / 2 + 2, w / 2));
    head.body.push_back(SnakeBody(h / 2 + 3, w / 2));
}

Map::Map(const Map &m)
{
    size = m.size;
    head = m.head;
    iWall = m.iWall;
    wall = m.wall;
    gate = m.gate;
    gItem = m.gItem;
    pItem = m.pItem;
}

void Map::print_map()
{
}
#endif