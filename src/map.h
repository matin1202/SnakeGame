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
    TimeItem tItem;
    Map(int h = 21, int w = 21, int wallCount = 0);
    Map(const Map &m);

    void print_map();
};

// void : 0, wall : 1, immune wall : -1, gate: 2, snake head: 3, snake body: 4

Map::Map(int h, int w, int wallCount)
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
    while (wallCount--)
    {
        int x = rand() % h + 1, y = rand() % w + 1, length = rand() % 6 + 4;
        int direction = rand() % 4 + 1; // Up = 1, Down = 4, Left = 2, Right = 3
        while (length--)
        {
            if (x >= 1 && x < h && y >= 1 && y < w)
            {
                Coord tmp;
                tmp.x = x;
                tmp.y = y;
                if (head.coord == tmp)
                {
                    continue;
                }
                bool isMerged = false;
                for (auto it = head.body.begin(); it != head.body.end(); it++)
                {
                    if (it->coord == tmp)
                    {
                        isMerged = true;
                    }
                }
                if (isMerged)
                    continue;
                wall.push_back(Wall(x, y));
            }
            switch (direction)
            {
            case 1:
                x--;
                break;
            case 2:
                y--;
                break;
            case 3:
                y++;
                break;
            case 4:
                x++;
                break;
            }
        }
    }
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
    tItem = m.tItem;
}

void Map::print_map()
{
}
#endif