#pragma once
#include"1D_List.h"
#include"multi_Dim_list.h"
class player
{
    int undoMoves , playerX , playerY , initialX , initialY , score;
public:
    player(gridlist l1,  int undo)
    {
        undoMoves = undo;
        score = 0;
        gridnode* temp = l1.head;
        
        playerX = rand() % l1.size;
        initialX = playerX;
        playerY = rand() % l1.size;
        initialY = playerY;
        for (int i = 0; i < playerX; i++)
        {
            temp = temp->down;
        }
        for (int j = 0; j < playerY; j++)
        {
            temp = temp->right;
        }
        temp->tile = 'P';
        
    }
    int getScore()
    {
        return score;
    }
    void setScore(int val)
    {
        score += val;
    }
    void setPlayer(int x, int y , int currX , int currY , gridlist &l1)
    {
        gridnode* temp = l1.head;
            for (int i = 1; i <= x; i++)
            {
                temp = temp->down;
            }
            for (int j = 1; j <= y; j++)
            {
                temp = temp->right;
            }
        temp->tile = 'P';
        temp = l1.head;
        for (int i = 1; i <= currX; i++)
        {
            temp = temp->down;
        }
        for (int j = 1; j <= currY; j++)
        {
            temp = temp->right;
        }
        temp->tile = '.';
    }
    int checkKey(int playerX, int playerY, int keyX, int keyY)
    {
        if (playerX == keyX && playerY == keyY)
            return 1;
        else
            return 0;
    }
    int checkdoor( int doorX, int doorY, int check)
    {
        if ((playerX == doorX && playerY == doorY) && (check))
            return 1;
        else
            return 0;
    }
    int checkCoin(int playerX, int playerY, node* coins, list& saved)
    {
        node* temp = coins;
        while (temp != NULL)
        {
            int x = temp->x;
            int y = temp->y;
            if (playerX == x && playerY == y)
            {
                saved.insetAtEnd(x, y);
                return 1;
            }
            temp = temp->next;
        }
        return 0;
    }
    int getMoves(int playerX, int playerY, int doorX, int doorY, int keyX, int keyY)
    {
        int moves = 0;
        if (playerX >= keyX)
        {
            moves += playerX - keyX;
        }
        else
        {
            moves += keyX - playerX;
        }
        if (playerY >= keyY)
        {
            moves += playerY - keyY;
        }
        else
        {
            moves += keyY - playerY;
        }
        if (keyX >= doorX)
        {
            moves += keyX - doorX;
        }
        else
        {
            moves += doorX - keyX;
        }
        if (keyY >= doorY)
        {
            moves += keyY - doorY;
        }
        else
        {
            moves += doorY - keyY;
        }
        return moves;
    }
    int getHint(int playerX, int playerY, int keyX, int keyY, int doorX, int doorY, int keyCheck, int& previous)
    {
        int check = 0;
        if (keyCheck)
        {
            if (playerX >= doorX)
            {
                check += playerX - doorX;
            }
            else
            {
                check += doorX - playerX;
            }
            if (playerY >= doorY)
            {
                check += playerY - doorY;
            }
            else
            {
                check += doorY - playerY;
            }
        }
        else
        {
            if (playerX >= keyX)
            {
                check += playerX - keyX;
            }
            else
            {
                check += keyX - playerX;
            }
            if (playerY >= keyY)
            {
                check += playerY - keyY;
            }
            else
            {
                check += keyY - playerY;
            }
        }
        if (check < previous)
        {
            previous = check;
            return 1;
        }
        else
        {
            previous = check;
            return 0;
        }
    }
    int checkBomb( node* temp)
    {
        while (temp != NULL)
        {
            if (temp->x == playerX && temp->y == playerY)
            {
                return 1;
            }
            temp = temp->next;
        }
        return 0;
    }
    int getPlayerX()
    {
        return playerX;
    }
    int getPlayerY()
    {
        return playerY;
    }
    int getInitialPlayerX()
    {
        return initialX;
    }
    int getInitialPlayerY()
    {
        return initialY;
    }
    void setPlayerX(const int &val)
    {
        playerX = val;
    }
    void setPlayerY(const int &val)
    {
        playerY = val;
    }
};
