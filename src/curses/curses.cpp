/*
 * Main Curses functions
 */

#include "../Invaders2.h"

bool Game::introStart()
{
    initscr();
    noecho();
    nodelay(stdscr, true);
    curs_set(FALSE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    getmaxyx(stdscr, parent_y, parent_x);

    vector<string> tempOne;
    vector<string> tempTwo;
    char animationOne[ROWS][COLUMNS] = {0}, animationTwo[ROWS][COLUMNS] = {0};
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
            tempOne.push_back(tempStr);
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
            tempTwo.push_back(tempStr);
        }
        file1.close();
        fileExists2 = true;
    }
    else
    {
        writeToFile("intro1.txt not found!\n");
    }
    if(fileExists1 == true || fileExists2 == true)
    {
        for(int x = 0;x<tempOne.size();x++)
        {
            for(int y = 0;y<tempOne[x].length();y++)
            {
                animationOne[x][y] = tempOne[x][y];
                animationTwo[x][y] = tempTwo[x][y];
            }
        }
        bool animation = true;
        while(1)
        {
            bool loop = true;
            while(loop == true)
            {
                time_t newtime = time(NULL);
                char piece = ' ';
                if(DEBUG == true)
                    writeToFile("Animation: %i | Time: %i > %i\n", animation, newtime, rawtime);
                for(int x = 0;x<ROWS;x++)
                {
                    for(int y = 0;y<COLUMNS;y++)
                    {
                        wmove(stdscr, x, y);
                        if(animation == true)
                        {
                            piece = animationOne[x][y];
                        }
                        else if(animation == false)
                        {
                            piece = animationTwo[x][y];
                        }
                        switch(piece)
                        {
                            //border
                            case '+':
                            case '-':
                            case '|':
                                waddch(stdscr, piece | COLOR_PAIR(1));
                                break;
                            case '%':
                                waddch(stdscr, piece | COLOR_PAIR(2));
                                break;
                            case '#':
                            case ' ':
                                waddch(stdscr, piece);
                                break;
                            default:
                                waddch(stdscr, piece | COLOR_PAIR(3));
                                break;
                        }
                            //waddch(stdscr, animationTwo[x][y]);
                    }
                }
                if(newtime > rawtime)
                {
                    writeToFile("animation = %i\n", animation);
                    if(animation == true)
                        animation = false;
                    else if(animation == false)
                        animation = true;
                    rawtime = newtime;
                }
                char ch = getch();
                switch(ch)
                {
                    case '\n':
                        loop = false;
                        break;
                    case KEY_UP:
                        wrefresh(stdscr);
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

/*
 * Main function for the game.
 */

bool Game::cursesMain()
{
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr,TRUE);
    srand(time(NULL));

    // Coloring the screen
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED); // border
    init_pair(2, COLOR_GREEN, COLOR_GREEN); // barrier
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // player
    init_pair(4,   COLOR_RED, COLOR_BLACK); // boss

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

    int alienCount = 0;
    for(int i = 0; i < BOARDROWSIZE;i++)
    {
        for(int j = 0; j < BOARDCOLUMNSIZE;j++)
        {
            char temp = board[i][j];
            wmove(field, i, j);
            waddch(field, temp);
            if(temp == ALIEN1 || temp == ALIEN2 || temp == ALIEN3)
                alienCount++;
//            mvwprintw(field, i, j, "%c", board[i][j]);
        }
    }
    numberOfAliens = alienCount;
    rawtime = time(NULL);
    alienGroup.waitTime = rawtime+8;
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

//        board[20][18] = 'O';
        // draw to our windows
        for(int i = 0; i < BOARDROWSIZE;i++)
        {
            for(int j = 0; j < BOARDCOLUMNSIZE;j++)
            {
                char piece = board[i][j];
//                writeToFile("%c", piece);
                wmove(field, i, j);
                // check if our bullet should be seen
                if(board[i][j] == BULLET && player.bullet.enabled == true)
                    waddch(field, board[i][j]);
                // if it shouldnt be seen then we remove it
                else if(board[i][j] == BULLET && player.bullet.enabled == false)
                {
                    waddch(field, ' ');
                    board[i][j] = ' ';
                    wmove(field, i-1, j);
                    waddch(field, ' ');
                    board[i-1][j] = ' ';
                    wmove(field, i, j);
                }
                // print everything else
                else
                {
                    // color certain pieces
                    switch(piece)
                    {
                        //border
                        case '+':
                        case '-':
                        case '|':
                            waddch(field, piece | COLOR_PAIR(1));
                            break;
                        // barriers
                        case '/':
                        case '\\':
                        case '#':
                            waddch(field, piece | COLOR_PAIR(2));
                            break;
                        //player
                        case 'A':
                            waddch(field, piece | COLOR_PAIR(3));
                        default:
                            waddch(field, piece);
                    }
                }
            }
//            writeToFile("\n");
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
        alienShoot();
        if(player.bullet.enabled == true)
        {
            if(player.bullet.direction == 'U') // safety check
            {
                timerCheck = clock()-timer;
                if(timerCheck > 15)
                {
                    if(DEBUG == true)
                        writeToFile("Current: %i,%i | New: %i,%i\n", player.bullet.x, player.bullet.y, player.bullet.x-1, player.bullet.y);
                    char temp = board[player.bullet.x-1][player.bullet.y];
                    char temp2 = board[player.bullet.x-1][player.bullet.y+1];
                    switch(temp)
                    {
                        // all these will trigger the last case
                        case 'S':
                        case '@':
                        case '%':
                        case '&':
                        case '#':
                        case '/':
                        case '\\':
                        case '+': // most likely never reach here
                        case '|':
                            if(DEBUG == true)
                                writeToFile("Collision with [%c]\n", temp);
                            if(temp == ALIEN1)
                            {
                                player.score += 10;
                                numberOfAliens--;
                            }
                            else if(temp == ALIEN2)
                            {
                                player.score += 20;
                                numberOfAliens--;
                            }
                            else if(temp == ALIEN3)
                            {
                                player.score += 40;
                                numberOfAliens--;
                            }
                            else if(temp == BOSS)
                            {
                                player.score += 150;
                            }
                            temp = ' ';
                            temp2 = ' ';
                            player.bullet.enabled = false;
                            break;
                        case '-':
                            if(DEBUG == true)
                                writeToFile("Bullet hits wall\n");
                            player.bullet.enabled = false;
                            board[player.bullet.x][player.bullet.y] = ' ';
                            break;
                        default: // spaces and whatnot
                            if(DEBUG == true)
                                writeToFile("%i,%i = %c\n%i,%i\n",player.bullet.x,player.bullet.y-1,board[player.bullet.x][player.bullet.y],player.bullet.x,player.bullet.y);
                            board[player.bullet.x-1][player.bullet.y] = BULLET;
                            board[player.bullet.x][player.bullet.y] = ' ';
                            player.bullet.x = player.bullet.x-1;
                            break;
                    }
                    timer = clock();
                }
            }
        }
        for(int x = 1;x<5;x++)
        {
            timerCheck = clock()-timer;
            if(timerCheck > 50)
            {
                char temp = ' ';
                switch(x)
                {
                    case 1:
                        temp = board[alienGroup.alienBullet1.x+1][alienGroup.alienBullet1.y];
                        if(alienGroup.alienBullet1.enabled == true)
                        {
                            switch(temp)
                            {
                                case '/':
                                case '\\':
                                case '#':
                                    temp = ' ';
                                    alienGroup.alienBullet1.enabled = false;
                                    board[alienGroup.alienBullet1.x][alienGroup.alienBullet1.y] = ' ';
                                    alienGroup.missles--;
                                    break;
                                case 'A':
                                    player.lives--;
                                    board[player.x][player.y]= ' ';
                                    player.x = 14;
                                    player.y = parent_y - (score_size + 3);
                                    board[player.x][player.y] = PLAYER;
                                    alienGroup.missles--;
                                    break;
                                case '-':
                                    if(DEBUG == true)
                                        writeToFile("Bullet hits wall\n");
                                    alienGroup.alienBullet1.enabled = false;
                                    board[alienGroup.alienBullet1.x][alienGroup.alienBullet1.y] = ' ';
                                    alienGroup.missles--;
                                    break;
                                default:
                                    if(DEBUG == true)
                                        writeToFile("%i,%i = %c\n%i,%i\n",player.bullet.x,player.bullet.y-1,board[player.bullet.x][player.bullet.y],player.bullet.x,player.bullet.y);
                                    board[alienGroup.alienBullet1.x+1][alienGroup.alienBullet1.y] = ALIENBULLET;
                                    board[alienGroup.alienBullet1.x][alienGroup.alienBullet1.y] = ' ';
                                    alienGroup.alienBullet1.x = alienGroup.alienBullet1.x+1;
                                    break;
                            }
                            /*
                            if(temp == PLAYER)
                            {
                                player.lives--;
                                player.x = 14;
                                player.y = parent_y - (score_size + 3);
                                board[player.x][player.y] = PLAYER;
                                alienGroup.alienBullet1.enabled = false;
                                alienGroup.missles--;
                            }
                            else if(temp == BARRIERCORNER1 || temp == BARRIERCORNER2 || temp == BARRIERMAIN)
                            {
                                temp = ' ';
                                alienGroup.alienBullet1.enabled = false;
                                alienGroup.missles--;
                            }
                            else if(temp == '-')
                            {
                                alienGroup.alienBullet1.enabled = false;
                                alienGroup.missles--;
                            }
                            else
                            {
                                temp = BULLET;
                                board[alienGroup.alienBullet1.x][alienGroup.alienBullet1.y] = ' ';
                                alienGroup.alienBullet1.y += 1;
                            }*/
                        }
//                    case 2:

//                    case 3:

//                    case 4:
                    default:
                        break;
                }
            }
        }
        if(keyHit())
        {
            key = getch();
            keyChecker = keyCheck(key);
            if(keyChecker == true)
                break;
            // have code in CP
        }

        // refresh each window
        wrefresh(field);
        wrefresh(score);
        if(numberOfAliens == 0)
            break;
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
    wmove(screen, 0, 0);
    waddch(screen, '+' | COLOR_PAIR(1));
    wmove(screen, y-1, 0);
    waddch(screen, '+' | COLOR_PAIR(1));
    wmove(screen, 0, x-1);
    waddch(screen, '+' | COLOR_PAIR(1));
    wmove(screen, y-1, x-1);
    waddch(screen, '+' | COLOR_PAIR(1));

    // sides
    for (i = 1; i < (y - 1); i++)
    {
        wmove(screen, i, 0);
        waddch(screen, '|' | COLOR_PAIR(1));
        wmove(screen, i, x-1);
        waddch(screen, '|' | COLOR_PAIR(1));
    }

    // top and bottom
    for (i = 1; i < (x - 1); i++)
    {
        wmove(screen, 0, i);
        waddch(screen, '-' | COLOR_PAIR(1));
        wmove(screen, y-1, i);
        waddch(screen, '-' | COLOR_PAIR(1));
    }
}
