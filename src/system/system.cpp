/*
 * Main System functions
 */

#include "../Invaders2.h"

Game::Game()
{
    //we need to build the game
    alienGroup.missles = 0;
    alienGroup.moveTowards = true;
    alienGroup.reachedEdge = false;
    Game::cursesMain();
}

Game::~Game()
{
    //nothing
}

bool Game::cursesMain()
{
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr,TRUE);
    srand(time(NULL));

    // set up initial windows
    getmaxyx(stdscr, parent_y, parent_x);

    //set default space
    player.lives = 3;
    player.score = 0;
    player.x = 14;
    player.y = parent_y - (score_size + 3);

    WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0);
    WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0);

    drawBorders(score);
    buildGame(board);

    for(int i = 0; i < BOARDROWSIZE;i++)
    {
        for(int j = 0; j < BOARDCOLUMNSIZE;j++)
        {
            mvwprintw(field, i, j, "%c", board[i][j]);
        }
    }

    rawtime = time(NULL);
    // uncomment for cout
    // /*
    while(1)
    {
        getmaxyx(stdscr, new_y, new_x);

        if (new_y != parent_y || new_x != parent_x)
        {
            parent_x = new_x;
            parent_y = new_y;

            wresize(field, new_y - score_size, new_x);
            wresize(score, score_size, new_x);
            mvwin(score, new_y - score_size, 0);

            wclear(stdscr);
            wclear(field);
            wclear(score);

            drawBorders(field);
            drawBorders(score);
        }

        // draw to our windows
        for(int i = 0; i < BOARDROWSIZE;i++)
        {
            for(int j = 0; j < BOARDCOLUMNSIZE;j++)
            {
                wmove(field, i, j);
                waddch(field, board[i][j]);
            }
        }

        // Draw score board
        mvwprintw(score, 1, 2, "Score: ");
        mvwprintw(score, 1, 9, itoa(player.score, buffer, 10));
        mvwprintw(score, 1, 70, "Lives: ");
        mvwprintw(score, 1, 77, itoa(player.lives, buffer, 10));

        if(DEBUG == true)
        {
            string screensizex = itoa(new_x, buffer, 10);
            string screensizey = itoa(parent_y, buffer, 10);
            mvwprintw(score, 1, 30, "X: ");
            mvwprintw(score, 1, 34, screensizex.c_str());
            mvwprintw(score, 1, 40, "Y: ");
            mvwprintw(score, 1, 44, screensizey.c_str());
        }

        bool aliensMovement = true;
        aliensMovement = moveAliens(rawtime);

        if(keyHit())
        {
            bool allowed = false;
            key = getch();
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
                    break;
                default:
                    break;
            }
            if(exit == true)
                break;
        }

        // refresh each window
        wrefresh(field);
        wrefresh(score);
    }
    endwin();
    // */
}

bool Game::drawBorders(WINDOW *screen)
{
    int x=0,y=0,i=0;
    getmaxyx(screen, y, x);

    // corners
    mvwprintw(screen, 0, 0, "+");
    mvwprintw(screen, y - 1, 0, "+");
    mvwprintw(screen, 0, x - 1, "+");
    mvwprintw(screen, y - 1, x - 1, "+");

    // sides
    for (i = 1; i < (y - 1); i++)
    {
        mvwprintw(screen, i, 0, "|");
        mvwprintw(screen, i, x - 1, "|");
    }

    // top and bottom
    for (i = 1; i < (x - 1); i++)
    {
        mvwprintw(screen, 0, i, "-");
        mvwprintw(screen, y - 1, i, "-");
    }
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


