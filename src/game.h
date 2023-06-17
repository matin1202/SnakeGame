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
#include <cstring>

using namespace std;

class Game
{
public:
    Map map;
    int activeTick = 0;
    int grow = 0;
    int poison = 0;
    int usedGate = 0;
    int maxSize = 3;
    int timer = 0;
    int tick = 200;
    float multipler = 1;
    int multi_timer = 0;

    char missionS = ' ';
    char missionG = ' ';
    char missionP = ' ';
    char missionUG = ' ';
    string reason = "";

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
        init_pair(8, COLOR_YELLOW, COLOR_YELLOW);
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
        int gtimer = 0, ptimer = 0, gateTimer = 0, ttimer = 0;
        bool isStart = false;
        while (1)
        {
            if (isStart)
            {
                gtimer++;
                ptimer++;
                gateTimer++;
                ttimer++;
                timer++;
            }
            clear();
            WINDOW *board = newwin(map.size.h + 2, map.size.w + 2, 0, 0);
            WINDOW *score = newwin(9, 27, 0, map.size.w + 4);
            WINDOW *mission = newwin(9, 27, 10, map.size.w + 4);
            box(board, 0, 0);
            box(score, 0, 0);
            box(mission, 0, 0);

            mvwprintw(score, 1, 1, "*******Score Board*******");
            mvwprintw(score, 3, 1, " B: %d/%d", map.head.body.size(), maxSize);
            mvwprintw(score, 4, 1, " +: %d", grow);
            mvwprintw(score, 5, 1, " -: %d", poison);
            mvwprintw(score, 6, 1, " G: %d", usedGate);
            mvwprintw(score, 7, 1, " time: %d", timer / (1000 / tick));

            mvwprintw(mission, 1, 1, "******Mission Board******");
            mvwprintw(mission, 3, 1, " B: 7 / %d (%c) ", map.head.body.size(), missionS);
            mvwprintw(mission, 4, 1, " +: 5 / %d (%c) ", grow, missionG);
            mvwprintw(mission, 5, 1, " -: 2 / %d (%c) ", poison, missionP);
            mvwprintw(mission, 6, 1, " G: 1 / %d (%c) ", usedGate, missionUG);

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
            wattron(board, COLOR_PAIR(8));
            mvwaddch(board, map.tItem.coord.x, map.tItem.coord.y, ' ');
            wattroff(board, COLOR_PAIR(8));

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
                isStart = true;
                break;
            case KEY_DOWN:
                map.head.direction = 4;
                isStart = true;
                break;
            case KEY_RIGHT:
                map.head.direction = 3;
                isStart = true;
                break;
            case KEY_LEFT:
                map.head.direction = 2;
                isStart = true;
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
            if (ttimer >= 30)
            {
                ttimer = 0;
                generateTItem();
            }
            if (multi_timer != 0)
            {
                multi_timer--;
            }
            if (multi_timer == 0)
                multipler = 1;
            if (gateTimer >= 80 && !(map.gate[0].isActive || map.gate[1].isActive))
            {
                gateTimer = 0;
                generateGate();
            }
            if (missionCompleted)
            {
                while (1)
                {
                    wclear(score);
                    box(score, 0, 0);
                    mvwprintw(score, 3, 2, "Press \'P\' to next stage.");
                    mvwprintw(score, 4, 2, "Press \'E\' to exit.");
                    wrefresh(score);
                    char key = getch();
                    if (key == 'e')
                        endwin();
                    if (key == 'p')
                    {
                        missionCompleted = false;
                        map = Map(21, 41, rand() % 4 + 2);
                        activeTick = 0;
                        grow = 0;
                        poison = 0;
                        usedGate = 0;
                        maxSize = 3;
                        timer = 0;
                        isStart = false;
                        multipler = 1;

                        missionS = ' ';
                        missionG = ' ';
                        missionP = ' ';
                        missionUG = ' ';
                        generateItems();
                        generateGate();
                        tick = ((rand() % 25) + 5) * 10;
                        break;
                    }
                    usleep(1000 * 100);
                }
            }
            if (!update(gtimer, ptimer, ttimer, prev))
            {
                while (1)
                {
                    map.head.move();
                    wclear(score);
                    box(score, 0, 0);
                    const char *_reason = reason.c_str();
                    mvwprintw(score, 3, 2, _reason);
                    mvwprintw(score, 4, 2, "Press \'P\' to continue.");
                    mvwprintw(score, 5, 2, "Press \'E\' to exit");
                    wrefresh(score);
                    char key = getch();
                    if (key == 'e')
                    {
                        endwin();
                        return;
                    }
                    if (key == 'p')
                    {
                        map.head = SnakeHead(map.size.h / 2, map.size.w / 2);
                        map.head.body.clear();
                        map.head.body.push_back(SnakeBody(map.size.h / 2 + 1, map.size.w / 2));
                        map.head.body.push_back(SnakeBody(map.size.h / 2 + 2, map.size.w / 2));
                        map.head.body.push_back(SnakeBody(map.size.h / 2 + 3, map.size.w / 2));
                        missionCompleted = false;
                        activeTick = 0;
                        grow = 0;
                        poison = 0;
                        usedGate = 0;
                        maxSize = 0;
                        timer = 0;
                        isStart = false;
                        multipler = 1;

                        missionS = ' ';
                        missionG = ' ';
                        missionP = ' ';
                        missionUG = ' ';
                        generateItems();
                        generateGate();
                        break;
                    }
                    usleep(1000 * 100);
                }
            }

            usleep(1000 * ((float)tick / multipler));
        }
    }

    bool isValid(int prev = 0)
    {
        if (prev == 5)
        {
            reason = "Tried moving in the opposite direction.";
            return false;
        }
        for (auto it = map.wall.begin(); it != map.wall.end(); it++)
        {
            if (it->coord == map.head.coord)
            {
                reason = "Collided with the wall.";
                return false;
            }
        }
        for (auto it = map.head.body.begin(); it != map.head.body.end(); it++)
        {
            if (it->coord == map.head.coord)
            {
                reason = "Collided with the body.";
                return false;
            }
        }
        if (map.head.body.size() < 3)
        {
            reason = "Length is less than 3.";
            return false;
        }
        return true;
    }

    void generateRandCoord(int &x, int &y, bool shouldIncludeWall = false)
    {
        while (1)
        {
            x = rand() % (map.size.h - 1) + 2;
            y = rand() % (map.size.w - 1) + 2;
            Coord tmp;
            tmp.x = x;
            tmp.y = y;
            bool same = false;
            if (!shouldIncludeWall)
            {
                for (auto it = map.wall.begin(); it != map.wall.end(); it++)
                {
                    if (it->coord == tmp)
                        same = true;
                }
            }
            for (auto it = map.head.body.begin(); it != map.head.body.end(); it++)
            {
                if (it->coord == tmp)
                    same = true;
            }
            for (auto it = map.gate.begin(); it != map.gate.end(); it++)
            {
                if (it->coord == tmp)
                    same = true;
            }
            if (map.head.coord == tmp)
                same = true;
            if (map.gItem.coord == tmp)
                same = true;
            if (map.pItem.coord == tmp)
                same = true;

            if (!same)
                break;
        }
    }

    void generateGate()
    {
        int idx1, idx2;
        while (1)
        {
            idx1 = rand() % map.wall.size();
            idx2 = rand() % map.wall.size();
            if (map.wall[idx1].coord.x == 1 || map.wall[idx1].coord.x == map.size.h || map.wall[idx1].coord.y == 1 || map.wall[idx1].coord.y == map.size.w)
            {
                int possible = 4;
                for (int i = 0; i < 4; i++)
                {
                    Coord tmp = map.wall[idx1].coord;
                    switch (i)
                    {
                    case 0:
                        tmp.x--;
                        break;
                    case 1:
                        tmp.x++;
                        break;
                    case 2:
                        tmp.y--;
                        break;
                    case 3:
                        tmp.y++;
                        break;
                    }
                    for (auto it = map.wall.begin(); it != map.wall.end(); it++)
                    {
                        if (tmp == it->coord)
                            possible--;
                    }
                }
                if (possible <= 1)
                    continue;
            }
            if (map.wall[idx2].coord.x == 1 || map.wall[idx2].coord.x == map.size.h || map.wall[idx2].coord.y == 1 || map.wall[idx2].coord.y == map.size.w)
            {
                int possible = 4;
                for (int i = 0; i < 4; i++)
                {
                    Coord tmp = map.wall[idx2].coord;
                    switch (i)
                    {
                    case 0:
                        tmp.x--;
                        break;
                    case 1:
                        tmp.x++;
                        break;
                    case 2:
                        tmp.y--;
                        break;
                    case 3:
                        tmp.y++;
                        break;
                    }
                    for (auto it = map.wall.begin(); it != map.wall.end(); it++)
                    {
                        if (tmp == it->coord)
                            possible--;
                    }
                }
                if (possible <= 1)
                    continue;
            }
            if (idx1 != idx2)
                break;
        }
        map.gate[0] = Gate(map.wall[idx1]);
        map.gate[1] = Gate(map.wall[idx2]);
    }

    void generateItems()
    {
        generateGItem();
        generatePItem();
        generateTItem();
    }

    void generateTItem()
    {
        int x, y;
        generateRandCoord(x, y);
        map.tItem = TimeItem(x, y);
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

    bool update(int &gtimer, int &ptimer, int &ttimer, int prev = 0)
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
        for (int i = 0; i < map.gate.size(); i++)
        {
            auto it = map.gate.begin() + i;
            if (it->coord == map.head.coord)
            {
                it->isActive = true;
                activeTick = map.head.body.size();
                auto other = (i == 0 ? map.gate.begin() + 1 : map.gate.begin());
                if (other->possible == 6)
                {
                    bool canGo = true;
                    Coord toGo = other->coord;
                    switch (map.head.direction)
                    {
                    case 1:
                        toGo.x--;
                        break;
                    case 2:
                        toGo.y--;
                        break;
                    case 3:
                        toGo.y++;
                        break;
                    case 4:
                        toGo.y--;
                        break;
                    }
                    for (auto w = map.wall.begin(); w != map.wall.end(); w++)
                    {
                        if (w->coord == toGo)
                        {
                            canGo = false;
                            break;
                        }
                    }
                    int possible = 4;
                    int dir = -1;
                    for (int i = 0; i < 4; i++)
                    {
                        Coord tmp = other->coord;
                        switch (i)
                        {
                        case 0:
                            tmp.x--;
                            break;
                        case 1:
                            tmp.x++;
                            break;
                        case 2:
                            tmp.y--;
                            break;
                        case 3:
                            tmp.y++;
                            break;
                        }
                        int t = possible;
                        for (auto it = map.wall.begin(); it != map.wall.end(); it++)
                        {
                            if (tmp == it->coord)
                                possible--;
                        }
                        if (possible == t)
                        {
                            switch (i)
                            {
                            case 0:
                                dir = 1;
                                break;
                            case 1:
                                dir = 2;
                                break;
                            case 2:
                                dir = 3;
                                break;
                            case 3:
                                dir = 4;
                                break;
                            }
                        }
                    }
                    if (!canGo && possible != 1)
                    {
                        switch (map.head.direction)
                        {
                        case 1:
                            map.head.direction = 3;
                            break;
                        case 2:
                            map.head.direction = 1;
                            break;
                        case 3:
                            map.head.direction = 4;
                            break;
                        case 4:
                            map.head.direction = 2;
                            break;
                        }
                    }
                    if (!canGo && possible == 1)
                    {
                        map.head.direction = dir;
                    }
                }
                else
                {
                    map.head.direction = other->possible;
                }
                map.head.coord = other->coord;
                map.head.move();
                usedGate++;
            }
        }

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
        if (map.head.coord == map.tItem.coord)
        {
            generateTItem();
            ttimer = 0;
            multipler = 1.5;
            multi_timer = 40;
        }

        // mission
        if (map.head.body.size() >= 7)
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
        if (map.head.body.size() >= 7 && grow >= 5 && poison >= 2 && usedGate >= 1)
        {
            missionCompleted = true;
        }
        if (map.head.body.size() > maxSize)
            maxSize = map.head.body.size();

        return isValid(prev);
    }
};

#endif
