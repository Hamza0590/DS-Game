# pragma once
#include<ctime>
#include<cstdlib>
#include<curses.h>
#include"1D_List.h"
//#include"player.h"
using namespace std;

class gridnode {
public:
    gridnode* up, * down, * right, * left;
    char tile;

    gridnode() {
        up = down = right = left = NULL;
        tile = '.';
    }
};

class gridlist {

public:
    gridnode* head, * curr;
    int size , doorX , doorY , keyX , keyY;
    char level;
    list coins, bomb;
    gridlist() {
        head = curr = NULL;
        size = doorX = doorY = keyX = keyY =0;
        level = '\0';
    }
    void makeGrid(int n , char lev) {
        size = n;
        level = lev;
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
    void checkGrid(int x, int y)
    {
        gridnode*  temp = head;
        for (int i = 1; i <=x ; i++)
        {
            temp = temp->down;
        }
        for (int i = 1; i <=y ; i++)
        {
            temp = temp->right;
        }
        if (temp->right != NULL)
            cout << "R: T" <<  endl;
        if (temp->left != NULL)
            cout << "L: T"  << endl;
        if (temp->up != NULL)
            cout << "UP: T"  << endl;
        if (temp->down != NULL)
            cout << "D: T"  << endl;
    }
    void checkPrint()
    {
        gridnode* rowStart = head;
        for (int i = 0; i <= size + 1; i++)
        {
            gridnode* temp = rowStart;
            for (int j = 0; j <= size + 1; j++) {
                if (i == 0 || i == size + 1 || j == 0 || j == size + 1)
                {

                    cout<<'#'<<' ';
                }
                else
                {
                    cout << temp->tile << ' ';
                    if (temp)
                        temp = temp->right;
                }
            }
            cout << endl;
            if (i > 0 && i < size + 1)
            {
                rowStart = rowStart->down;
            }
        }
    }
    void updateTile(int x, int y, char newTile) {
        gridnode* temp = head;
        for (int i = 0; i < x; i++) {
            temp = temp->down;
        }

       
        for (int j = 0; j < y; j++) {
            temp = temp->right;
        }
        temp->tile = newTile;
    }
    void setDoorCor(int x, int y)
    {
        doorX = x;
        doorY = y;
    }
    void setKeyCor(int x, int y)
    {
        keyX = x;
        keyY = y;
    }
    void drawCoins(int pX, int pY,  int level)
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
            } while ((coinX == pX && coinY == pY) || (coinX == doorX && coinY == doorY) || (coinX == keyX && coinY == keyY));
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
    void drawBomb(int pX, int pY, int level)
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
            } while ((bombX == pX && bombY == pY) || (bombX == doorX && bombY == doorY) || (bombX == keyX && bombY == keyY) || check);
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
    void printGrid(int moves, int undo, int score, int hint, int keyCheck , int stackX , int stackY)
    {
        gridnode* rowStart = head;
        clear();
        if (level == 'E')
            mvprintw(0, 4, "Mode: EASY");
        else if (level == 'M')
            mvprintw(0, 8, "Mode: MEDIUM");
        else if (level == 'H')
            mvprintw(0, 12, "Mode: HARD");
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
        refresh();
    }
    void printEndDisplay(int playerInitialX, int playerInitialY, int pX, int pY,  int score, list initialCoins, list  savedCoins)
    {

        clear();
        int moveForward = 0, gridStartRow = 0;
        node* coinPrint = savedCoins.head;
        if (level == 'E')
            mvprintw(gridStartRow, 4, "Mode: EASY");
        else if (level == 'M')
            mvprintw(gridStartRow, 8, "Mode: MEDIUM");
        else if (level == 'H')
            mvprintw(gridStartRow, 12, "Mode: HARD");
        gridStartRow += 3;
        mvprintw(gridStartRow, moveForward, "Collected Coins: ");
        moveForward += 17;
        while (coinPrint != NULL)
        {
            int x = coinPrint->getX(), y = coinPrint->getY();
            mvprintw(gridStartRow, moveForward, "(%d", x);
            mvprintw(gridStartRow, moveForward + 2, ",%d", y);
            mvprintw(gridStartRow, moveForward + 4, ")");
            moveForward += 5;
            coinPrint = coinPrint->next;
        }
        gridStartRow += 1;
        mvprintw(gridStartRow, 0, "Score: %d", score);
        gridStartRow += 2;
        mvprintw(gridStartRow, 0, "This was the initial state of the game.");
        gridStartRow += 1;
        
        updateTile(pX, pY, '.');
        updateTile(playerInitialX, playerInitialY, 'P');
        updateTile(doorX, doorY, 'D');
        updateTile(keyX, keyY, 'K');
        node* temp = coins.head;
        node* coinNode = initialCoins.head;
        
            while (temp != NULL)
            {
                updateTile(temp->getX(), temp->getY(), '.');
                temp = temp->next;
            }
        coinNode = initialCoins.head;
        while (coinNode != NULL)
        {
                updateTile(coinNode->getX(), coinNode->getY(), 'C');
                coinNode = coinNode->next;
        }
       
        
        gridnode* rowStart = head;
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
        refresh();
    }
    node* getCoins()
    {
        return coins.head;
    }
    node* getBomb()
    {
        return bomb.head;
    }
    void delCoins()
    {
        node* temp = coins.head;
        while (temp != NULL)
        {
            node* curr = temp;
            gridnode* traverseforDeletion = head;
            for (int i = 1; i <= curr->x; i++)
            {
                traverseforDeletion = traverseforDeletion->down;
            }
            for (int j = 1; j <= curr->y; j++)
            {
                traverseforDeletion = traverseforDeletion->right;
            }
            traverseforDeletion->tile = '.';
            temp = temp->next;
            delete curr;
            curr = NULL;
        }
        coins.head = NULL;
    }
    
};
