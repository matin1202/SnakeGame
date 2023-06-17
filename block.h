#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <vector>

using namespace std;

struct Coord
{
    int x, y;

    bool operator==(const Coord &other)
    {
        return x == other.x && y == other.y;
    }
};

class Block
{

public:
    Coord coord;
    int identity = 0;
    // void : 0, wall : 1, immune wall : -1, gate: 2, snake head: 3, snake body: 4, growth_item: 5, poison_item: -5
    virtual int showIdentity() { return identity; }
    Block()
    {
        coord.x = 0;
        coord.y = 0;
    }
    Block(int x, int y)
    {
        coord.x = x;
        coord.y = y;
    }
    Block(const Block &b)
    {
        coord = b.coord;
    }
};

class Wall : public Block
{
public:
    int position;
    Wall() : Block() { identity = 1; }
    Wall(int x, int y, int position = -1) : Block(x, y)
    {
        identity = 1;
        this->position = position;
    }
    int showIdentity() { return identity; }

    /* Up = 1, Down = 4, Left = 3, Right = 2 */
};

class ImmunedWall : public Block
{
public:
    ImmunedWall() : Block() { identity = 2; }
    ImmunedWall(int x, int y) : Block(x, y) { identity = 2; }
    int showIdentity() { return identity; }
};

class GrowthItem : public Block
{
public:
    GrowthItem() : Block() {}
    GrowthItem(int x, int y) : Block(x, y) { identity = 5; }
    GrowthItem(const GrowthItem &gitem)
    {
        coord = gitem.coord;
    }
    int showIdentity() { return identity; }
};

class PoisonItem : public Block
{
public:
    PoisonItem() : Block() {}
    PoisonItem(int x, int y) : Block(x, y) { identity = -5; }
    PoisonItem(const PoisonItem &pitem)
    {
        coord = pitem.coord;
    }
    int showIdentity() { return identity; }
};

class TimeItem : public Block
{
public:
    TimeItem() : Block() {}
    TimeItem(int x, int y) : Block(x, y) { identity = 6; }
    TimeItem(const TimeItem &titem)
    {
        coord = titem.coord;
    }
    int showIdentity() { return identity; }
};

class Gate : public Block
{
public:
    int possible;
    Gate() : Block() { identity = 2; }
    Gate(const Wall &wall)
    {
        coord = wall.coord;
        possible = 5 - wall.position;
    }
    Gate(int x, int y) : Block(x, y) { identity = 2; }
    bool isActive = false;
    int showIdentity() { return identity; }
};

class SnakeHead;

class SnakeBody : public Block
{
public:
    SnakeBody() : Block() { identity = 4; }
    SnakeBody(int x, int y) : Block(x, y) { identity = 4; }
    SnakeBody(const SnakeHead &head);
    int showIdentity() { return identity; }
};

class SnakeHead : public Block
{
public:
    vector<SnakeBody> body = vector<SnakeBody>();
    friend SnakeBody;
    SnakeHead() : Block() { identity = 3; }
    SnakeHead(int x, int y) : Block(x, y)
    {
        identity = 3;
    }
    int showIdentity() { return identity; }
    void move()
    {
        switch (direction)
        {
        case 1:
            coord.x--;
            break;
        case 2:
            coord.y--;
            break;
        case 3:
            coord.y++;
            break;
        case 4:
            coord.x++;
            break;
        }
    }

    /* Up = 1, Down = 4, Left = 3, Right = 2 */
    int direction = -1;
};

SnakeBody::SnakeBody(const SnakeHead &head)
{
    identity = 4;
    coord = head.coord;
}

#endif