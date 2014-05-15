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
    alienGroup.timedShots = time(NULL);
    player.bullet.enabled = false;
    rawtime = time(NULL);
    timer = clock();
    movementSetSpeed = 0;
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
    int alienType = 5;
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
            else if(i>=5 && i<=9)
            {
                if(j>=ALIENSTARTY && j<=ALIENENDY)
                {
                    if(isAlien == true)
                    {
                        if(alienType == i)
                            board[i][j] = ALIEN3;
                        else if(alienType+1 == i || alienType+2 == i)
                            board[i][j] = ALIEN2;
                        else if(alienType+3 == i || alienType+4 == i)
                            board[i][j] = ALIEN1;

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

bool Game::keyCheck(int key)
{
    bool allowed = false;
    bool exit= false;
    switch(key)
    {
        case KEY_LEFT:
            allowed = movePos('L');
            break;
        case KEY_RIGHT:
            allowed = movePos('R');
            break;
        case 27: // esc to quit
            exit = true;
            break;
        case 32: // space
            // shoot
            if(player.bullet.enabled != true)
            {
                player.bullet.x = player.y;
                player.bullet.y = player.x;
                if(player.bullet.x-1 > 0)
                {
                    player.bullet.enabled = true;
                    // we need the variables backwards
                    player.bullet.x = player.y-1;
                    player.bullet.y = player.x;
                    player.bullet.direction = 'U';
                    if(DEBUG == true)
                        writeToFile("%i,%i = [%c]\n", player.bullet.x-1,player.bullet.y, board[player.bullet.x][player.bullet.y]);
                    board[player.bullet.x-1][player.bullet.y] = BULLET;
                    timer = clock();
                    if(DEBUG == true)
                        writeToFile("Player: %i,%i | Bullet: %i,%i\n",player.x,player.y,player.bullet.x,player.bullet.y);
                }
            }
            else
                cout << ""; // nothing
            break;
        default:
            break;
    }
    if(exit == true)
        return true;
    else
        return false;
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

bool Game::alienShoot()
{
    if(alienGroup.missles < 5)
    {
        int rowsTurn = 1+rand()%10;
        int rowBackup = rowsTurn;
        time_t now = time(NULL);
        writeToFile("Now: %i | TimedShots: %i | rowTurn: %i | rowBackup: %i | Rand: %i | Missles: %i\n", now, alienGroup.timedShots, rowsTurn, rowBackup, 1+rand()%11, alienGroup.missles);
        if(alienGroup.waitTime > now)
        {
            if(now > alienGroup.timedShots)
            {
                for(int x=ROWS;x>0;x--)
                {
                    for(int y=COLUMNS;y>0;y--)
                    {
                        if(board[x][y] == ALIEN1 || board[x][y] == ALIEN2 || board[x][y] == ALIEN3)
                        {
                            writeToFile("Rows Turn: %i\n", rowsTurn);
                            if(rowsTurn != 0)
                                rowsTurn--;
                            else if(rowsTurn == 0)
                            {
                                alienGroup.missles++;
                                switch(alienGroup.missles)
                                {
                                    case 1:
                                        alienGroup.alienBullet1.direction = 'D';
                                        alienGroup.alienBullet1.x = x+2;
                                        alienGroup.alienBullet1.y = y;
                                        alienGroup.alienBullet1.enabled = true;
                                        board[alienGroup.alienBullet1.x][alienGroup.alienBullet1.y] = 'i';
                                        return true;
                                        break;
                                    case 2:
                                        alienGroup.alienBullet2.direction = 'D';
                                        alienGroup.alienBullet2.x = x+2;
                                        alienGroup.alienBullet2.y = y;
                                        alienGroup.alienBullet2.enabled = true;
                                        board[alienGroup.alienBullet2.x][alienGroup.alienBullet2.y] = 'i';
                                        return true;
                                        break;
                                    case 3:
                                        alienGroup.alienBullet3.direction = 'D';
                                        alienGroup.alienBullet3.x = x+2;
                                        alienGroup.alienBullet3.y = y;
                                        alienGroup.alienBullet3.enabled = true;
                                        board[alienGroup.alienBullet3.x][alienGroup.alienBullet3.y] = 'i';
                                        return true;
                                        break;
                                    case 4:
                                        alienGroup.alienBullet4.direction = 'D';
                                        alienGroup.alienBullet4.x = x+2;
                                        alienGroup.alienBullet4.y = y;
                                        alienGroup.alienBullet4.enabled = true;
                                        board[alienGroup.alienBullet4.x][alienGroup.alienBullet4.y] = 'i';
                                        return true;
                                        break;
                                    default:
                                        //we have a error
                                        break;
                                }
                                alienGroup.timedShots = now + (1 + rand() % 59);
                                alienGroup.waitTime = 1+rand()%59;
                            }
                        }
                    }
                    rowsTurn = rowBackup;
                }
            }
            else
                alienGroup.timedShots = now + (1 + rand() % 59);
        }
    }
}
