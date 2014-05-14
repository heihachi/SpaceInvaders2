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

    getmaxyx(stdscr, parent_y, parent_x);

    vector<string> animationOne;
    vector<string> animationTwo;
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
            animationOne.push_back(tempStr);
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
            animationTwo.push_back(tempStr);
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
        bool animation = true;
        while(1)
        {
            bool loop = true;
            while(loop == true)
            {
                time_t newtime = time(NULL);
                for(int x = 0;x<ROWS;x++)
                {
                    if(animation == true)
                    {
                        mvwprintw(stdscr, x, 0, "%s", animationOne[x].c_str());
                    }
                    else if(animation == false)
                    {
                        mvwprintw(stdscr, x, 0, "%s", animationTwo[x].c_str());
                    }
                }
                if(newtime > rawtime)
                {
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

    for(int i = 0; i < BOARDROWSIZE;i++)
    {
        for(int j = 0; j < BOARDCOLUMNSIZE;j++)
        {
            char temp = board[i][j];
            wmove(field, i, j);
            waddch(field, temp);
//            mvwprintw(field, i, j, "%c", board[i][j]);
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

        board[20][18] = 'O';
        // draw to our windows
        for(int i = 0; i < BOARDROWSIZE;i++)
        {
            for(int j = 0; j < BOARDCOLUMNSIZE;j++)
            {
                char piece = board[i][j];
                //writeToFile("%c", piece);
                wmove(field, i, j);
                // check if our bullet should be seen
                if(board[i][j] == BULLET && player.bullet.enabled == true)
                    waddch(field, board[i][j]);
                // if it shouldnt be seen then we remove it
                else if(board[i][j] == BULLET && player.bullet.enabled == false)
                    waddch(field, ' ');
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
        if(player.bullet.enabled == true)
        {
            if(player.bullet.direction == 'U') // safety check
            {
                writeToFile("Current: %i,%i | New: %i,%i\n", player.bullet.x, player.bullet.y, player.bullet.x, player.bullet.y-1);
                if(board[player.bullet.x][player.bullet.y-1] == '-')
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
                    player.bullet.enabled = true;
                    player.bullet.x = 63;
                    player.bullet.y = 18;
                    player.bullet.direction = 'U';
                    writeToFile("Shot bullet\n");
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
