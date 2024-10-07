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
    }

    void printGrid(char levelName)
    {
        gridnode* rowStart = head;
        clear();
        if (levelName == 'E')
            mvprintw(0, 10, "EASY");
        else if (levelName == 'M')
            mvprintw(0, 14, "MEDIUM");
        else if (levelName == 'H')
            mvprintw(0, 14, "HARD");

        int gridStartRow = 3;
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
                    mvaddch(i + gridStartRow, j * 2, temp->tile);
                    if (temp)
                        temp = temp->right;
                }
            }
            if (i > 0 && i < size + 1)
            {
                rowStart = rowStart->down;
            }
        }

        refresh();
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
    void drawCoins(int pX, int pY, int kX, int kY, int dX, int dY, list& coins)
    {
        int coinX, coinY;
        int n = rand() % 5 + 5;
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
    gridnode* getHead()
    {
        return head;
    }
};
