#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "block.h"
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>

using namespace std;

class Game
{
public:
    Map map;
    int activeTick = 0;
    int grow = 0;
    int poison = 0;
    int usedGate = 0;

    char missionS = ' ';
    char missionG = ' ';
    char missionP = ' ';
    char missionUG = ' ';

    bool missionCompleted = false;

    Game()
    {
        map = Map(21, 41, 2);
        initscr();
        clear();
        noecho();
        cbreak();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_WHITE);   // Wall
        init_pair(2, COLOR_BLACK, COLOR_WHITE);   // Immuned Wall
        init_pair(3, COLOR_CYAN, COLOR_CYAN);     // Snake Head
        init_pair(4, COLOR_GREEN, COLOR_GREEN);   // Snake Body
        init_pair(5, COLOR_BLUE, COLOR_BLUE);     // Growth Item
        init_pair(6, COLOR_RED, COLOR_RED);       // Poison Item
        init_pair(7, COLOR_BLACK, COLOR_MAGENTA); // GATE
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        srand((unsigned int)time(NULL));
        generateItems();
        generateGate();
    }
    ~Game()
    {
    }

    void refreshScreen()
    {
        int gtimer = 0, ptimer = 0, gateTimer = 0;
        while (1)
        {
            gtimer++;
            ptimer++;
            gateTimer++;
            clear();
            WINDOW *board = newwin(map.size.h + 2, map.size.w + 2, 0, 0);
            WINDOW *score = newwin(8, 27, 0, map.size.w + 4);
            WINDOW *mission = newwin(8, 27, 9, map.size.w + 4);
            box(board, 0, 0);
            box(score, 0, 0);
            box(mission, 0, 0);

            mvwprintw(score, 1, 1, "*******Score Board*******");
            mvwprintw(score, 3, 1, " score: %d", map.head.body.size() - 3);
            mvwprintw(score, 4, 1, " grow: %d", grow);
            mvwprintw(score, 5, 1, " poison: %d", poison);
            mvwprintw(score, 6, 1, " Used Gate: %d", usedGate);

            mvwprintw(mission, 1, 1, "******Mission Board******");
            mvwprintw(mission, 3, 1, " score: 4 / %d (%c) ", map.head.body.size() - 3, missionS);
            mvwprintw(mission, 4, 1, " grow: 5 / %d (%c) ", grow, missionG);
            mvwprintw(mission, 5, 1, " poison: 2 / %d (%c) ", poison, missionP);
            mvwprintw(mission, 6, 1, " Used Gate: 1 / %d (%c) ", usedGate, missionUG);

            for (auto it = map.iWall.begin(); it != map.iWall.end(); it++)
            {
                wattron(board, COLOR_PAIR(2));
                mvwaddch(board, it->coord.x, it->coord.y, '+');
                wattroff(board, COLOR_PAIR(2));
            }
            for (auto it = map.wall.begin(); it != map.wall.end(); it++)
            {
                wattron(board, COLOR_PAIR(2));
                mvwaddch(board, it->coord.x, it->coord.y, ' ');
                wattroff(board, COLOR_PAIR(2));
            }
            for (auto it = map.head.body.begin(); it != map.head.body.end(); it++)
            {
                wattron(board, COLOR_PAIR(4));
                mvwaddch(board, it->coord.x, it->coord.y, ' ');
                wattroff(board, COLOR_PAIR(4));
            }
            for (auto it = map.gate.begin(); it != map.gate.end(); it++)
            {
                wattron(board, COLOR_PAIR(7));
                mvwaddch(board, it->coord.x, it->coord.y, ' ');
                wattroff(board, COLOR_PAIR(7));
            }
            wattron(board, COLOR_PAIR(3));
            mvwaddch(board, map.head.coord.x, map.head.coord.y, ' ');
            wattroff(board, COLOR_PAIR(3));
            wattron(board, COLOR_PAIR(5));
            mvwaddch(board, map.gItem.coord.x, map.gItem.coord.y, ' ');
            wattroff(board, COLOR_PAIR(5));
            wattron(board, COLOR_PAIR(6));
            mvwaddch(board, map.pItem.coord.x, map.pItem.coord.y, ' ');
            wattroff(board, COLOR_PAIR(6));

            refresh();
            wrefresh(board);
            wrefresh(score);
            wrefresh(mission);
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
            if (gtimer >= 50)
            {
                gtimer = 0;
                generateGItem();
            }
            if (ptimer >= 50)
            {
                ptimer = 0;
                generatePItem();
            }
            if (gateTimer >= 80 && !(map.gate[0].isActive || map.gate[1].isActive))
            {
                gateTimer = 0;
                generateGate();
            }
            if (!update(gtimer, ptimer, prev))
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
        int x, y;
        generateRandCoord(x, y);
        map.pItem = PoisonItem(x, y);
        generateRandCoord(x, y);
        map.gItem = GrowthItem(x, y);
    }

    void generateGItem()
    {
        int x, y;
        generateRandCoord(x, y);
        map.gItem = GrowthItem(x, y);
    }

    void generatePItem()
    {
        int x, y;
        generateRandCoord(x, y);
        map.pItem = PoisonItem(x, y);
    }

    bool update(int &gtimer, int &ptimer, int prev = 0)
    {
        if (activeTick == 0)
        {
            map.gate[0].isActive = false;
            map.gate[1].isActive = false;
        }
        else
            activeTick--;
        if (map.head.direction != -1)
        {
            map.head.body.insert(map.head.body.begin(), SnakeBody(map.head));
            map.head.body.pop_back();
        }
        map.head.move();

        if (map.head.coord == map.gItem.coord)
        {
            grow++;
            generateGItem();
            gtimer = 0;
            auto last = map.head.body.end() - 1, sec = map.head.body.end() - 2;
            map.head.body.push_back(SnakeBody(last->coord.x - (sec->coord.x - last->coord.x), last->coord.y - (sec->coord.y - last->coord.y)));
        }
        if (map.head.coord == map.pItem.coord)
        {
            poison++;
            generatePItem();
            ptimer = 0;
            map.head.body.pop_back();
        }

        // mission
        if ((map.head.body.size() - 3) >= 4)
            missionS = 'v';
        else
            missionS = ' ';
        if (grow >= 5)
            missionG = 'v';
        else
            missionG = ' ';
        if (poison >= 2)
            missionP = 'v';
        else
            missionP = ' ';
        if (usedGate >= 1)
            missionUG = 'v';
        else
            missionUG = ' ';

        // missionComplted
        if ((map.head.body.size() - 3) >= 4 && grow >= 5 && poison >= 2 && usedGate >= 1)
        {
            missionCompleted = true;
        }

        return isValid(prev);
    }
};

#endif