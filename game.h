#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "block.h"
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

class Game
{
public:
    Map map;
    Game()
    {
        map = Map(21, 41);
        initscr();
        clear();
        noecho();
        cbreak();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_WHITE); // Wall
        init_pair(2, COLOR_BLACK, COLOR_WHITE); // Immuned Wall
        init_pair(3, COLOR_CYAN, COLOR_CYAN);   // Snake Head
        init_pair(4, COLOR_GREEN, COLOR_GREEN); // Snake Body
        init_pair(5, COLOR_BLUE, COLOR_BLUE);   // Growth Item
        init_pair(6, COLOR_RED, COLOR_RED);     // Poison Item
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        generateItems();
    }
    ~Game()
    {
        endwin();
    }

    void refreshScreen()
    {
        while (1)
        {
            clear();
            WINDOW *board = newwin(map.size.h + 2, map.size.w + 2, 0, 0);
            box(board, 0, 0);
            for (auto it = map.iWall.begin(); it != map.iWall.end(); it++)
            {
                wattron(board, COLOR_PAIR(2));
                mvwaddch(board, it->coord.x, it->coord.y, '+');
                wattroff(board, COLOR_PAIR(2));
            }
            for (auto it = map.wall.begin(); it != map.wall.end(); it++)
            {
                wattron(board, COLOR_PAIR(1));
                mvwaddch(board, it->coord.x, it->coord.y, ' ');
                wattroff(board, COLOR_PAIR(1));
            }
            for (auto it = map.head.body.begin(); it != map.head.body.end(); it++)
            {
                wattron(board, COLOR_PAIR(4));
                mvwaddch(board, it->coord.x, it->coord.y, ' ');
                wattroff(board, COLOR_PAIR(4));
            }
            wattron(board, COLOR_PAIR(3));
            mvwaddch(board, map.head.coord.x, map.head.coord.y, ' ');
            wattroff(board, COLOR_PAIR(3));
            wattron(board, COLOR_PAIR(5));
            mvwaddch(board, map.gItem.coord.x, map.gItem.coord.y, 'G');
            wattroff(board, COLOR_PAIR(5));
            wattron(board, COLOR_PAIR(6));
            mvwaddch(board, map.pItem.coord.x, map.pItem.coord.y, 'P');
            wattroff(board, COLOR_PAIR(6));

            refresh();
            wrefresh(board);
            int key = getch();
            int prev = map.head.direction;
            switch (key)
            {
            case KEY_UP:
                map.head.direction = 1;
                break;
            case KEY_DOWN:
                map.head.direction = 4;
                break;
            case KEY_RIGHT:
                map.head.direction = 3;
                break;
            case KEY_LEFT:
                map.head.direction = 2;
                break;
            }
            if (!update(prev))
                break;
            usleep(1000 * 200);
        }
    }

    bool isValid(int prev = 0)
    {
        if (prev == 5)
            return false;
        for (auto it = map.wall.begin(); it != map.wall.end(); it++)
        {
            if (it->coord == map.head.coord)
                return false;
        }
        for (auto it = map.head.body.begin(); it != map.head.body.end(); it++)
        {
            if (it->coord == map.head.coord)
                return false;
        }
        if (map.head.body.size() < 3)
            return false;
        return true;
    }

    void generateItems()
    {
        map.pItem = PoisonItem(rand() % (map.size.h - 2) + 2, rand() % (map.size.w - 2) + 2);
        map.gItem = GrowthItem(rand() % (map.size.h - 2) + 2, rand() % (map.size.w - 2) + 2);
    }

    void generateGItem()
    {
        map.gItem = GrowthItem(rand() % (map.size.h - 2) + 2, rand() % (map.size.w - 2) + 2);
    }

    void generatorPItem()
    {
        map.pItem = PoisonItem(rand() % (map.size.h - 2) + 2, rand() % (map.size.w - 2) + 2);
    }

    bool update(int prev = 0)
    {
        if (map.head.direction != -1)
        {
            map.head.body.insert(map.head.body.begin(), SnakeBody(map.head));
            map.head.body.pop_back();
        }
        map.head.move();

        if (map.head.coord == map.gItem.coord)
        {
            generateGItem();
            auto last = map.head.body.end() - 1, sec = map.head.body.end() - 2;
            map.head.body.push_back(SnakeBody(last->coord.x - (sec->coord.x - last->coord.x), last->coord.y - (sec->coord.y - last->coord.y)));
        }
        if (map.head.coord == map.pItem.coord)
        {
            generatorPItem();
            map.head.body.pop_back();
        }

        return isValid(prev);
    }
};

#endif