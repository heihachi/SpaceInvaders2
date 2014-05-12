/*
    Header file for main system requirements
*/

#ifndef system_h
#define system_h

#include <curses.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#ifdef WIN32
#include <windows.h>
#endif // WIN32
#ifdef LINUX
#include <unistd.h>
#endif // LINUX

using namespace std;

const bool DEBUG = false;

struct missle
{
    int x, y;
    char enabled, direction, posture, type;
};

struct barrierpiece
{
    int x, y;
    int lives;
    char enable;
};

struct barrier
{
    int x, y;
    barrierpiece pieces[24];
    char totalEnable;
    //pieces 0,5 19-22 must be black
};

struct createPlayer
{
    int x;
    int y;
    int score;
    int lives;
    missle bullet;
};

struct aliens
{
    bool moveTowards;
    bool reachedEdge;
    int missles;
    missle alienBullet1;
    missle alienBullet2;
    missle alienBullet3;
    missle alienBullet4;
};

struct boss
{

};

class Game
{
    public:
        Game();
        // Start system.cpp functions
        bool cursesMain();
        bool drawBorders(WINDOW*);
        bool buildGame(char[22][80]);
        bool keyHit();
        bool writeToFile(const char*, ...);
        // end system.cpp functions
        // start movement.cpp functions
        bool moveAliens(time_t);
        bool movePlayer(char);
        bool movePos(char);
        // end movement.cpp functions
        ~Game();
    private:
        createPlayer player;
        boss bossShip;
        aliens alienGroup;
        const char ALIEN = '@';
        const char PLAYER = 'A';
        const char BARRIERMAIN = '#';
        const char BARRIERCORNER1 = '/';
        const char BARRIERCORNER2 = '\\';
        const char BULLET = '!';
        static const int BOARDROWSIZE = 22;
        static const int BOARDCOLUMNSIZE = 80;
        const int ALIENSTARTY = 29;
        const int ALIENENDY = 50;
        char board[BOARDROWSIZE][BOARDCOLUMNSIZE];
        int ch = 0, parent_x = 0, parent_y = 0, new_x = 0, new_y = 0, scoreInt = 0, score_size = 3, key = 0;
        char buffer[256];
        time_t rawtime;
        //nothing here atm
};

#endif // system_h
