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
    Game::introStart();
}

Game::~Game()
{
    //nothing
}

bool Game::introStart()
{
    initscr();
    noecho();
    nodelay(stdscr, true);
    curs_set(FALSE);

    getmaxyx(stdscr, parent_y, parent_x);
    WINDOW *intro = newwin(COLUMNS, ROWS, 0, 0);

    vector<string> tempVector;
    vector<string> tempVector1;
    string tempStr = "";

    ifstream file;
    ifstream file1;
    file.open("src/intro.txt");
    file1.open("src/intro1.txt");
    bool fileExists1 = false, fileExists2 = false;
    if(file.good())
    {
        int counter = 0;
        while(getline(file, tempStr))
        {
            tempVector.push_back(tempStr);
        }
        file.close();
        fileExists1 = true;
    }
    else
    {
        writeToFile("intro.txt not found!\n");
    }
    if(file1.good())
    {
        while(getline(file1, tempStr))
        {
            tempVector1.push_back(tempStr);
        }
        file1.close();
        fileExists2 = true;
    }
    else
    {
        writeToFile("intro1.txt not found!\n");
    }
    writeToFile("%i,%i\n",fileExists1,fileExists2);
    if(fileExists1 == true || fileExists2 == true)
    {
        bool animation = true;
        while(1)
        {
            bool loop = true;
            while(loop == true)
            {
                if(animation == true)
                {
                    for(int x = 0;x<tempVector.size();x++)
                    {
                        wclear(intro);
                        printw("%s", tempVector[x].c_str());
                        writeToFile("%s\n", tempVector[x].c_str());
                    }

                    animation = false;
                }
                else if(animation == false)
                {
                    for(int x = 0;x<tempVector1.size();x++)
                    {
                        wclear(intro);
                        printw("%s", tempVector1[x].c_str());
                        writeToFile("%s\n", tempVector1[x].c_str());
                    }
                    animation = true;
                }
                char ch = getch();
                switch(ch)
                {
                    case '\n':
                        loop = false;
                        break;
                    default:
                        break;
                }
            }
            break;
        }
        endwin();
        Game::cursesMain();
    }
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

            drawBorders(score);
        }

        // draw to our windows
        for(int i = 0; i < BOARDROWSIZE;i++)
        {
            for(int j = 0; j < BOARDCOLUMNSIZE;j++)
            {
                wmove(field, i, j);
                // check if our bullet should be seen
                if(board[i][j] == BULLET && player.bullet.enabled == true)
                    waddch(field, board[i][j]);
                // if it shouldnt be seen then we remove it
                else if(board[i][j] == BULLET && player.bullet.enabled == false)
                    waddch(field, ' ');
                // print everything else
                else
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
        if(player.bullet.enabled == true)
        {
            if(player.bullet.direction == 'U') // safety check
            {
                if(board[player.bullet.x][player.bullet.y-1] > 0)
                {
                    char temp = board[player.bullet.x][player.bullet.y-1];
                    switch(temp)
                    {
                        // all these will trigger the last case
                        case '@':
                        case '#':
                        case '/':
                        case '\\':
                        case '+': // most likely never reach here
                        case '-':
                        case '|':
                            temp = ' ';
                            player.bullet.enabled = false;
                            break;
                        default: // spaces and whatnot
                            writeToFile("%i,%i = %c\n%i,%i",player.bullet.x,player.bullet.y-1,player.bullet.x,player.bullet.y);
                            board[player.bullet.x][player.bullet.y-1] = BULLET;
                            board[player.bullet.x][player.bullet.y] = ' ';
                            player.bullet.y = player.bullet.y-1;
//                            player.bullet.enabled = false;
                            break;
                    }
                }
                else
                {
                    writeToFile("Bullet hits wall\n");
                    player.bullet.enabled = false;
                }
            }
        }
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
                    if(player.bullet.enabled != true)
                    {
                        player.bullet.enabled = true;
                        player.bullet.x = player.x;
                        player.bullet.y = player.y-1;
                        player.bullet.direction = 'U';
                        board[player.x][player.y-1] = BULLET;
                        writeToFile("Player: %i,%i | Bullet: %i,%i\n",player.x,player.y,player.bullet.x,player.bullet.y);
                    }
                    else
                        cout << ""; // nothing
                    break;
                case KEY_UP:
                    writeToFile("Here1\n");
                    player.bullet.enabled = true;
                    writeToFile("Here2\n");
                    player.bullet.x = 63;
                    writeToFile("Here3\n");
                    player.bullet.y = 18;
                    writeToFile("Here4\n");
                    player.bullet.direction = 'U';
                    writeToFile("Here5\n");
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
    return true;
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


