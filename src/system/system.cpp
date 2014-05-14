/*
 * Main System functions
 */

#include "../Invaders2.h"

Game::Game()
{
    //we need to build the game and set some variables
    alienGroup.missles = 0;
    alienGroup.moveTowards = true;
    alienGroup.reachedEdge = false;
    player.bullet.enabled = false;
    writeToFile("<-----------------------------------------New Game----------------------------------------->\n");
    Game::introStart();
}

Game::~Game()
{
    //nothing
}

bool Game::buildGame(char board[BOARDROWSIZE][BOARDCOLUMNSIZE])
{
    bool isAlien = true;
    for(int i=0;i<BOARDROWSIZE;i++)
    {
        for(int j=0;j<BOARDCOLUMNSIZE;j++)
        {
            if(i == 0 || i == BOARDROWSIZE-1)
            {
                // make borders
                if(j == 0 || j == BOARDCOLUMNSIZE-1)
                {
                    board[i][j] = '+';
                }
                else
                {
                    board[i][j] = '-';
                }
            }
            else if(i>=5 && i<=10)
            {
                if(j>=ALIENSTARTY && j<=ALIENENDY)
                {
                    if(isAlien == true)
                    {
                        board[i][j] = ALIEN;
                        isAlien = false;
                    }
                    else if(isAlien == false)
                    {
                        board[i][j] = ' ';
                        isAlien = true;
                    }
                }
                else if(j == 0 || j == BOARDCOLUMNSIZE-1)
                {
                    board[i][j] = '|';
                }
                else
                {
                    board[i][j] = ' ';
                }
            }
            else
            {
                if(j == 0 || j == BOARDCOLUMNSIZE-1)
                {
                    board[i][j] = '|';
                }
                else
                {
                    board[i][j] = ' ';
                }
            }
            //writeToFile("i = %i & player.x = %i | j = %i & player.y = %i\n", i, player.x, j, player.y);
            if(i == player.y && j == player.x)
            {
                //writeToFile("Player is at %i,%i | %i,%i\n", i, j, player.x, player.y);
                board[i][j] = PLAYER;
            }
        }
        isAlien = true;
    }
    //build barrier here
    for(int i = 15;i<=17;i++)
    {
        for(int j = 0;j<BOARDCOLUMNSIZE;j++)
        {
            switch(j)
            {
                case 11:
                    if(i == 15)
                    {
                        board[i][j] = BARRIERCORNER1;
                        board[i][j+1] = BARRIERMAIN;
                        board[i][j+2] = BARRIERMAIN;
                        board[i][j+3] = BARRIERMAIN;
                        board[i][j+4] = BARRIERMAIN;
                        board[i][j+5] = BARRIERMAIN;
                        board[i][j+6] = BARRIERCORNER2;
                    }
                    else if(i == 16 || i == 17)
                    {
                        for(int x = 0;x<7;x++)
                        {
                            if(x == 2 || x == 3 || x == 4)
                            {
                                board[i][j+x] = ' ';
                            }
                            else
                            {
                                board[i][j+x] = BARRIERMAIN;
                            }
                        }
                    }
                    break;
                case 28:
                    if(i == 15)
                    {
                        board[i][j] = BARRIERCORNER1;
                        board[i][j+1] = BARRIERMAIN;
                        board[i][j+2] = BARRIERMAIN;
                        board[i][j+3] = BARRIERMAIN;
                        board[i][j+4] = BARRIERMAIN;
                        board[i][j+5] = BARRIERMAIN;
                        board[i][j+6] = BARRIERCORNER2;
                    }
                    else if(i == 16 || i == 17)
                    {
                        for(int x = 0;x<7;x++)
                        {
                            if(x == 2 || x == 3 || x == 4)
                            {
                                board[i][j+x] = ' ';
                            }
                            else
                            {
                                board[i][j+x] = BARRIERMAIN;
                            }
                        }
                    }
                    break;
                case 45:
                    if(i == 15)
                    {
                        board[i][j] = BARRIERCORNER1;
                        board[i][j+1] = BARRIERMAIN;
                        board[i][j+2] = BARRIERMAIN;
                        board[i][j+3] = BARRIERMAIN;
                        board[i][j+4] = BARRIERMAIN;
                        board[i][j+5] = BARRIERMAIN;
                        board[i][j+6] = BARRIERCORNER2;
                    }
                    else if(i == 16 || i == 17)
                    {
                        for(int x = 0;x<7;x++)
                        {
                            if(x == 2 || x == 3 || x == 4)
                            {
                                board[i][j+x] = ' ';
                            }
                            else
                            {
                                board[i][j+x] = BARRIERMAIN;
                            }
                        }
                    }
                    break;
                case 62:
                    if(i == 15)
                    {
                        board[i][j] = BARRIERCORNER1;
                        board[i][j+1] = BARRIERMAIN;
                        board[i][j+2] = BARRIERMAIN;
                        board[i][j+3] = BARRIERMAIN;
                        board[i][j+4] = BARRIERMAIN;
                        board[i][j+5] = BARRIERMAIN;
                        board[i][j+6] = BARRIERCORNER2;
                    }
                    else if(i == 16 || i == 17)
                    {
                        for(int x = 0;x<7;x++)
                        {
                            if(x == 2 || x == 3 || x == 4)
                            {
                                board[i][j+x] = ' ';
                            }
                            else
                            {
                                board[i][j+x] = BARRIERMAIN;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    if(DEBUG == true)
    {
        for(int i = 0; i < BOARDROWSIZE;i++)
        {
            writeToFile("\"%i\":%i\n{", i, BOARDCOLUMNSIZE);
            for(int j = 0; j < BOARDCOLUMNSIZE;j++)
            {
                writeToFile("\t\"%i\"\n\t{\n\t\t\"%c\"\n\t}\n", j, board[i][j]);
            }
            writeToFile("}\n");
        }
    }
}

bool Game::keyHit()
{
    int ch = getch();

    if(ch != ERR)
    {
        ungetch(ch);
        return true;
    }
    else
    {
        return false;
    }
}

bool Game::writeToFile(const char *message, ...)
{
    ofstream tempFile;
    char buffer[256] = {0};
    tempFile.open("debug.log", ios_base::app | ios_base::out);
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, 256, message, args);
    va_end(args);
    tempFile << buffer;
    tempFile.close();
    return true;
}


