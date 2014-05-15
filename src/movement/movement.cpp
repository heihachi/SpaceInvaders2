/*
 * Main movement functions for aliens and player
 */

#include "../Invaders2.h"

bool Game::moveAliens(time_t rawtime)
{
    /*
        this function will take true or false
        if true we will go to the right false to the left
    */
    time_t temp = time(NULL);
    if(alienGroup.reachedEdge == false)
    {
        int tempAlienCount = 0;
        if(temp > rawtime)
        {
            for(int i = 0;i<BOARDROWSIZE;i++)
            {
                if(alienGroup.moveTowards == true)
                {
                    for(int j = BOARDCOLUMNSIZE-1;j>=0;j--)
                    {
                        if(board[i][j] == ALIEN1 || board[i][j] == ALIEN2 || board[i][j] == ALIEN3)
                        {
                            tempAlienCount++;
                            if(board[i][j+1] != '|')
                            {
                                if(board[i][j] == ALIEN1)
                                    board[i][j+1] = ALIEN1;
                                else if(board[i][j] == ALIEN2)
                                    board[i][j+1] = ALIEN2;
                                else if(board[i][j] == ALIEN3)
                                    board[i][j+1] = ALIEN3;
                                board[i][j] = ' ';
                            }
                            else
                            {
                                // add the code to change direction
                                alienGroup.moveTowards = false;
                                alienGroup.reachedEdge = true;
                                return true;
                            }
                        }
                    }
                }
                else if(alienGroup.moveTowards == false)
                {
                    for(int j = 0;j<BOARDCOLUMNSIZE;j++)
                    {
                        if(board[i][j] == ALIEN1 || board[i][j] == ALIEN2 || board[i][j] == ALIEN3)
                        {
                            tempAlienCount++;
                            if(board[i][j-1] != '|')
                            {
                                if(board[i][j] == ALIEN1)
                                    board[i][j-1] = ALIEN1;
                                else if(board[i][j] == ALIEN2)
                                    board[i][j-1] = ALIEN2;
                                else if(board[i][j] == ALIEN3)
                                    board[i][j-1] = ALIEN3;
                                board[i][j] = ' ';
                            }
                            else
                            {
                                // add the code to change direction
                                alienGroup.moveTowards = true;
                                alienGroup.reachedEdge = true;
                                return true;
                            }
                        }
                    }
                }
            }
            this->rawtime = temp;
            numberOfAliens = tempAlienCount;
        }
    }
    else if(alienGroup.reachedEdge == true)
    {
//        if(temp > rawtime)
//        {
            for(int i = BOARDROWSIZE;i>0;i--)
            {
                for(int j = BOARDCOLUMNSIZE-1;j>=0;j--)
                {
                    if(board[i][j] == ALIEN1 || board[i][j] == ALIEN2 || board[i][j] == ALIEN3)
                    {
                        if(board[i][j] == ALIEN1)
                            board[i+1][j] = ALIEN1;
                        else if(board[i][j] == ALIEN2)
                            board[i+1][j] = ALIEN2;
                        else if(board[i][j] == ALIEN3)
                            board[i+1][j] = ALIEN3;
                        board[i][j] = ' ';
                    }
                }
            }
            alienGroup.reachedEdge = false;
//        }
    }
    return true;
}

bool Game::movePlayer(char movement)
{
    if(movement == 'R')
    {

    }
    else if(movement == 'L')
    {
        if(board[player.x][player.y-1] != '|')
        {
            board[player.x][player.y-1] = PLAYER;
            board[player.x][player.y] = ' ';
            player.y = player.y-1;
        }
    }
    else
    {
        // errors
        writeToFile("We got a movement error.\n");
    }
}

bool Game::movePos(char movement)
{
    if(movement == 'R') // Fixed
    {
        if(board[player.y+1][player.x+1] == ' ' || board[player.y][player.x+1] == BULLET)
        {
            player.x++;
            board[player.y][player.x-1] = ' ';
            board[player.y][player.x] = PLAYER;
            return true;
        }
        else
        {
            return false;
        }
    }
    if(movement == 'L')
    {
        if(board[player.y][player.x-1] == ' ' || board[player.y][player.x-1] == BULLET)
        {
            player.x--;
            board[player.y][player.x+1] = ' ';
            board[player.y][player.x] = PLAYER;
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}
