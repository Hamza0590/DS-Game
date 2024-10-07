#include <iostream>
#include <curses.h>
#include <cstdlib>
#include<chrono>
#include<ctime>

using namespace std;


class node
{
public:
    int x, y;
    node* next;
    node()
    {
        x = 0;
        y = 0;
        next = NULL;
    }
};
class list
{
public:
    node* head;
    list()
    {
        head = NULL;
    }
    void insetAtEnd(int xCor, int yCor)
    {
        node* temp = new node();
        temp->x = xCor;
        temp->y = yCor;
        if (head == NULL)
            head = temp;
        else
        {
            node* temp2 = head;
            while (temp2->next != NULL)
            {
                temp2 = temp2->next;
            }
            temp2->next = temp;
        }
        temp = NULL;
        delete temp;
    }
    void print()
    {
        node* temp = head;
        do
        {
            cout << '(' << temp->x << ',' << temp->y << ')';
            temp = temp->next;
        } while (temp != NULL);
    }
};
class gridnode {
public:
    gridnode* up, * down, * right, * left;
    char tile , bomb , coin , key , door;

    gridnode() {
        up = down = right = left = NULL;
        tile = bomb = coin = key = door ='.';
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
        if(levelName=='E')
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

                    mvaddch(gridStartRow+i, j * 2, '#');
                }
                else 
                {
                    mvaddch(i+gridStartRow, j * 2, temp->tile);
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
    void Key(int x, int y , char alpha)
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
    void drawCoins(int pX, int pY, int kX, int kY, int dX, int dY , list &coins)
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
class player
{
    int undoMoves;
public:
    player(gridlist l1 ,int x , int y , int undo)
    {
        undoMoves = undo;
        gridnode* temp = l1.head;       
        for (int i = 0; i < x; i++)
        {
            temp = temp->down;
        }
        for (int j = 0; j < y; j++)
        {
            temp = temp->right;
        }
        temp->tile = 'P';
    }
    int checkKey (int playerX , int playerY , int keyX , int keyY)
    {
        if (playerX == keyX && playerY == keyY)
            return 1;
        else
            return 0;
    }
    int checkdoor (int playerX, int playerY, int doorX, int doorY , int check)
    {
        if ((playerX == doorX && playerY == doorY) && (check))
            return 1;
        else
            return 0;
    }
    int checkCoin(int playerX, int playerY, list& coins , list& saved)
    {
        node* temp = coins.head;
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
};
class delNode
{
public:
    void deleteNodes(gridlist &l1 , list &coin)
    {
        node* temp;
        temp = coin.head;
        while (temp != NULL)
        {
            node* curr = temp;
            gridnode* traverseforDeletion = l1.head;
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
        }
        coin.head = NULL;
    }
};
int main() 
{
    int gridSize, undo = 0, level = 0;
    char levelName;
    cout << "Select Difficulty Level:\n\t1- Easy\n\t2- Medium\n\t3- Hard";
    cin >> level;
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    srand(time(0));
    gridlist l1;   
    if (level == 1)
    {
        gridSize = 10;
        undo = 6;
        levelName = 'E';
    }
    else if (level == 2)
    {
        gridSize = 15;
        undo = 4;
        levelName = 'M';
    }
    else if (level == 3)
    {
        gridSize = 20;
        undo = 1;
        levelName = 'H';
    }
    int playerX = rand() % gridSize, playerY = rand() % gridSize;
    int keyX = 0, keyY = 0, getKey = 0;
    int doorX, doorY;
    int playerPreX = playerX, playerPreY = playerY;
    do
    {
        keyX = rand() & gridSize, keyY = rand() % gridSize;
    } while (keyX == playerX && keyY == playerY);
    do
    {
        doorX = rand() & gridSize, doorY = rand() % gridSize;
    } while ((doorX == playerX && doorY == playerY) && (doorX==keyX && doorY==keyY));


    l1.makeGrid(gridSize);
    player p1(l1 , playerX , playerY , undo);
    l1.Key(keyX , keyY , 'K');
    l1.door(doorX, doorY, 'D');
    list coins , savedCoins;
    l1.drawCoins(playerX, playerY, keyX, keyY, doorX, doorY , coins);
    l1.printGrid(levelName);

    delNode d1;
    
    int leftCheck = 1, rightCheck = 1, upCheck = 1, downCheck = 1;
    auto lastUpdate = std::chrono::steady_clock::now();
    int coinInterval = 5;
    while (true)
    {
        int a = getch();
        l1.door(doorX, doorY, 'D');
        auto now = std::chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::seconds>(now - lastUpdate).count() >= coinInterval) 
        {

            d1.deleteNodes(l1 , coins);
            l1.drawCoins(playerX, playerY, keyX, keyY, doorX, doorY , coins);
            lastUpdate = now;
        }
        if (a == KEY_UP && upCheck)
        {
            downCheck = 0;
            if (playerX>0)
            {
                upCheck = 1;
                rightCheck = 1;
                leftCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                playerX -= 1;
                l1.updateTile(playerX , playerY, 'P');
                playerPreX = playerX;
                l1.printGrid(levelName);
            }
        }
        else if (a == KEY_DOWN && downCheck)
        {
            upCheck = 0;
            if (playerX < gridSize-1)
            {
                downCheck = 1;
                rightCheck = 1;
                leftCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                playerX += 1;
                l1.updateTile(playerX , playerY , 'P');
                playerPreX = playerX;
                l1.printGrid(levelName);
            }
        }
        else if (a == KEY_LEFT && leftCheck)
        {
            rightCheck = 0;
            if (playerY > 0)
            {
                upCheck = 1;
                downCheck = 1;
                leftCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                playerY -= 1;
                l1.updateTile(playerX, playerY, 'P');
                playerPreY = playerY;
                l1.printGrid(levelName);
            }
        }
        else if (a == KEY_RIGHT && rightCheck)
        {
            leftCheck = 0;
            if (playerY < gridSize-1)
            {
                upCheck = 1;
                downCheck = 1;
                rightCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                playerY += 1;
                l1.updateTile(playerX, playerY, 'P');
                playerPreY = playerY;
                l1.printGrid(levelName);
            }
        }
        if (p1.checkKey(playerX, playerY, keyX, keyY))
        {
            l1.Key(keyX, keyY, '.');
            keyX = keyY = 0;
            getKey = 1;
        }
        if (p1.checkdoor(playerX, playerY, doorX, doorY, getKey))
        {
            break;
        }
        if (p1.checkCoin(playerX, playerY, coins, savedCoins))
        {
            undo += 1;
        }
    }
    savedCoins.print();
    endwin();
    return 0;
}
