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
    int x, y; // where its at
    char direction; // where its going / who owns it U for player D for aliens
    bool enabled; // is it visible?
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
    time_t waitTime;
    time_t timedShots;
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
        bool introStart();
        bool drawBorders(WINDOW*);
        bool buildGame(char[22][80]);
        bool keyHit();
        bool keyCheck(int);
        bool writeToFile(const char*, ...);
        bool alienShoot();
        bool bossTimer();
        // end system.cpp functions
        // start movement.cpp functions
        bool moveAliens(time_t);
        bool movePlayer(char);
        bool movePos(char);
        // end movement.cpp functions
        int numberOfAliens;
        ~Game();
    private:
        createPlayer player;
        boss bossShip;
        aliens alienGroup;
        const char BOSS = 'S';
        const char ALIEN1 = '@';
        const char ALIEN2 = '%';
        const char ALIEN3 = '&';
        const char PLAYER = 'A';
        const char BARRIERMAIN = '#';
        const char BARRIERCORNER1 = '/';
        const char BARRIERCORNER2 = '\\';
        const char BULLET = '!';
        const char ALIENBULLET = 'i';
        const char BORDER1 = '+';
        const char BORDER2 = '-';
        const char BORDER3 = '|';
        static const int ROWS = 25;
        static const int COLUMNS = 80;
        static const int BOARDROWSIZE = 22;
        static const int BOARDCOLUMNSIZE = 80;
        const int ALIENSTARTY = 29;
        const int ALIENENDY = 50;
        char board[BOARDROWSIZE][BOARDCOLUMNSIZE];
        int ch = 0, parent_x = 0, parent_y = 0, new_x = 0, new_y = 0, scoreInt = 0, score_size = 3, key = 0;
        char buffer[256];
        time_t rawtime;
        int timer;
        int timerCheck;
        time_t bossTimer;
        int movementSpeeds[5] = { 100, 80, 60, 40, 20};
        int movementSetSpeed;
        bool keyChecker;
        //nothing here atm
};

#endif // system_h
