#define WIN32
//#define LINUX
#include "Invaders2.h"

// these are incase the linker doesnt link correctly
//#include "system/system.cpp"
//#include "movement/movement.cpp"


/*
 * To-Do list:
 * DONE - create animation - convert vectors to character arrays and wmove waddch for easier management and add color
 * SEMI_DONE Fix bullets
 * DONE Get score working
 * boss
 * SEMI random alien bullets
*/
int main()
{
    Game SpaceInvaders;
    if(SpaceInvaders.numberOfAliens == 0)
        cout << "YOU WIN!" << endl;
    else
        cout << "GAME OVER!" << endl;
    return 0;
}

/*
    0000000000111111111122222222223333333333444444444455555555556666666666777777777788
    0123456789012345678901234567890123456789012345678901234567890123456789012345678901
00  +--------------------------------------------------------------------------------+
01  |                                                                                |
02 !| SSS                                                                            |
03  |                                                                                |
04  |                                                                                |
05 !|                           @ @ @ @ @ @ @ @ @ @ @                                |
06 !|                           @ @ @ @ @ @ @ @ @ @ @                                |
07 !|                           @ @ @ @ @ @ @ @ @ @ @                                |
08 !|                           @ @ @ @ @ @ @ @ @ @ @                                |
09 !|                           @ @ @ @ @ @ @ @ @ @ @                                |
10 !|                           @ @ @ @ @ @ @ @ @ @ @                                |
11  |                                                                                |
12  |                                                                                |
13  |                                                                                |
14  |                                                                                |
15 !|          /#####\          /#####\          /#####\          /#####\            |
16 !|          ##   ##          ##   ##          ##   ##          ##   ##            |
17 !|          ##   ##          ##   ##          ##   ##          ##   ##            |
18  |                                                                                |
19  |                                                                                |
20  |             P                                                                  |
21 !|                                                                                |
22  +--------------------------------------------------------------------------------+
23  +--------------------------------------------------------------------------------+
24  | Score: 0                                                              Lives: 3 +
25  +--------------------------------------------------------------------------------+
*/
