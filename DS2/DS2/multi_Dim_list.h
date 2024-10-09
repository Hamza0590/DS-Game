# pragma once
#include<ctime>
#include<cstdlib>
#include<curses.h>
#include"1D_List.h"
using namespace std;

class gridnode {
public:
    gridnode* up, * down, * right, * left;
    char tile, bomb, coin, key, door;

    gridnode() {
        up = down = right = left = NULL;
        tile = bomb = coin = key = door = '.';
    }
};

class gridlist {

public:
    gridnode* head, * curr;
    int size;
    gridlist() {
        head = curr = NULL;
    }

    void makeGrid(int n) {
        size = n;
        gridnode* upper = head;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0) {
                    gridnode* temp = new gridnode();
                    if (head == NULL) {
                        head = temp;
                        curr = temp;
                    }
                    else {
                        curr->right = temp;
                        temp->left = curr;
                        curr = curr->right;
                    }
                }
                else {
                    gridnode* temp = new gridnode();
                    upper->down = temp;
                    temp->up = upper;
                    if (j != 0) {
                        temp->left = curr;
                        curr->right = temp;
                    }
                    curr = temp;
                    upper = upper->right;
                }
            }
            if (i < n - 1) {
                while (curr->left != NULL) {
                    curr = curr->left;
                }
            }
            upper = curr;
        }
        curr = head;
    }

    void updateTile(int x, int y, char newTile) {
        gridnode* rowStart = head;
        for (int i = 1; i <= x; i++) {
            rowStart = rowStart->down;
        }

        gridnode* temp = rowStart;
        for (int j = 1; j <= y; j++) {
            temp = temp->right;
        }
        temp->tile = newTile;
    }
    void Key(int x, int y, char alpha)
    {
        gridnode* temp = head;
        for (int i = 1; i <= x; i++)
        {
            temp = temp->down;
        }
        for (int j = 1; j <= y; j++)
        {
            temp = temp->right;
        }
        temp->key = alpha;
        temp->tile = alpha;
    }
    void door(int x, int y, char alpha)
    {
        gridnode* temp = head;
        if (temp == NULL)
            return;
        for (int i = 1; i <= x; i++)
        {
            temp = temp->down;
        }
        for (int j = 1; j <= y; j++)
        {
            temp = temp->right;
        }
        temp->door = alpha;
        temp->tile = alpha;
    }
    void drawCoins(int pX, int pY, int kX, int kY, int dX, int dY, int level , list& coins)
    {
        int coinX, coinY  , n;
        if (level == 1)
            n = rand() % 3 + 6;
        else if (level == 2)
            n = rand() % 8 + 8;
        else if (level == 3)
            n = rand() % 10 + 10;
        for (int i = 0; i < n; i++)
        {
            gridnode* temp = head;
            do
            {
                coinX = rand() % size, coinY = rand() % size;
            } while ((coinX == pX && coinY == pY) || (coinX == dX && coinY == dY) || (coinX == kX && coinY == kY));
            coins.insetAtEnd(coinX, coinY);
            for (int i = 1; i <= coinX; i++)
            {
                temp = temp->down;
            }
            for (int j = 1; j <= coinY; j++)
            {
                temp = temp->right;
            }
            temp->tile = 'C';
        }
    }
    void drawBomb(int pX, int pY, int kX, int kY, int dX, int dY, int level, list& coins, list& bomb)
    {
        int bombX, bombY, n = 0, check;
        if (level == 1)
            n = rand() % 3 + 2;
        else if (level == 2)
            n = rand() % 5 + 5;
        else if (level == 3)
            n = rand() % 8 + 7;
        for (int i = 0; i < n; i++)
        {
            gridnode* temp = head;
            do
            {
                check = 0;
                bombX = rand() % size, bombY = rand() % size;
                node* temp2 = coins.head;
                while (temp2 != NULL)
                {
                    if (temp2->x == bombX && temp2->y == bombY)
                    {
                        check = 1;
                        break;
                    }
                    temp2 = temp2->next;
                }
            } while ((bombX == pX && bombY == pY) || (bombX == dX && bombY == dY) || (bombX == kX && bombY == kY) || check);
            bomb.insetAtEnd(bombX, bombY);
            for (int i = 1; i <= bombX; i++)
            {
                temp = temp->down;
            }
            for (int j = 1; j <= bombY; j++)
            {
                temp = temp->right;
            }
            temp->tile = 'B';
        }
    }
    void printGrid(char levelName, int moves, int undo, int score, int hint, int keyCheck , int stackX , int stackY)
    {
        gridnode* rowStart = head;
        clear();
        if (levelName == 'E')
            mvprintw(0, 10, "EASY");
        else if (levelName == 'M')
            mvprintw(0, 14, "MEDIUM");
        else if (levelName == 'H')
            mvprintw(0, 18, "HARD");
        int gridStartRow = 2;
        mvprintw(gridStartRow, 0, "Remaining moves: %d", moves);
        mvprintw(gridStartRow, 22, "Remaining undos: %d", undo);
        gridStartRow += 1;
        mvprintw(gridStartRow, 0, "Score: %d", score);
        if (hint == 1)
            mvprintw(gridStartRow, 22, "Hint: Moving Close");
        else if (hint == 0)
            mvprintw(gridStartRow, 22, "Hint: Moving Away");
        else
            mvprintw(gridStartRow, 22, "Hint: To Be Decided");
        gridStartRow += 1;
        if (keyCheck)
            mvprintw(gridStartRow, 0, "key Status: True");
        else
            mvprintw(gridStartRow, 0, "Key Status: False");
        gridStartRow += 2;
        for (int i = 0; i <= size + 1; i++)
        {
            gridnode* temp = rowStart;
            for (int j = 0; j <= size + 1; j++) {
                if (i == 0 || i == size + 1 || j == 0 || j == size + 1)
                {

                    mvaddch(gridStartRow + i, j * 2, '#');
                }
                else
                {
                    init_pair(1, COLOR_GREEN, COLOR_BLACK);
                    init_pair(2, COLOR_RED, COLOR_BLACK);
                    init_pair(3, COLOR_BLUE, COLOR_BLACK);
                    if (temp->tile == 'P')
                    {
                        attron(COLOR_PAIR(1));
                    }
                    if (temp->tile == 'B')
                    {
                        attron(COLOR_PAIR(2));
                    }
                    if (temp->tile == 'C')
                    {
                        attron(COLOR_PAIR(3));
                    }
                    mvaddch(i + gridStartRow, j * 2, temp->tile);
                    attroff(COLOR_PAIR(1));
                    attroff(COLOR_PAIR(2));
                    attroff(COLOR_PAIR(3));
                    if (temp)
                        temp = temp->right;
                }
            }
            if (i > 0 && i < size + 1)
            {
                rowStart = rowStart->down;
            }
        }
        gridStartRow += 17;
        mvprintw(gridStartRow, 0, "x", stackX );
        mvprintw(gridStartRow+1, 0, "y", stackY);
        refresh();
    }
};
