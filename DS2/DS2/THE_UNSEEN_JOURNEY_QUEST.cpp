#include <iostream>
#include <curses.h>
#include <cstdlib>
#include<chrono>
#include<ctime>
#include"multi_Dim_list.h"
#include"1D_List.h"
#include"player.h"
#include"delNodes.h"
#include"UndoStack.h"
using namespace std;

int main() 
{
    int gridSize, undo = 0, level = 0 ,moves =0 , score=0 , keyX = 0, keyY = 0, getKey = 0 , doorX, doorY ,
        leftCheck = 1, rightCheck = 1, upCheck = 1, downCheck = 1 , coinInterval = 10, previous;
    char levelName;
    cout << "Select Difficulty Level:\n1- Easy\n2- Medium\n3- Hard\nSelect: ";
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
        moves = 6;
        levelName = 'E';
    }
    else if (level == 2)
    {
        gridSize = 15;
        undo = 4;
        moves = 2;
        levelName = 'M';
    }
    else if (level == 3)
    {
        gridSize = 20;
        undo = 1;
        moves = 0;
        levelName = 'H';
    }
    int playerX = rand() % gridSize, playerY = rand() % gridSize;
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
    list coins , savedCoins , bombs;
    l1.drawCoins(playerX, playerY, keyX, keyY, doorX , doorY , level, coins);
    l1.drawBomb(playerX, playerY, keyX, keyY, doorX, keyY, level, coins, bombs);
    moves += p1.getMoves(playerX, playerY, doorX, doorY , keyX, keyY);
    l1.printGrid(levelName , moves , undo , score , 3 , getKey,0 ,0);
    delNode d1;
    stackList undoStack;
    
    auto lastUpdate = std::chrono::steady_clock::now();
    p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
    while (true)
    {
        moves -= 1;
        int a = getch();
        l1.door(doorX, doorY, 'D');
        auto now = std::chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::seconds>(now - lastUpdate).count() >= coinInterval) 
        {

            d1.deleteNodes(l1 , coins);
            l1.drawCoins(playerX, playerY, keyX, keyY, doorX, doorY , level , coins);
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
                undoStack.pushBack(playerPreX, playerPreY);
                playerX -= 1;
                l1.updateTile(playerX , playerY, 'P');
                playerPreX = playerX;
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid(levelName , moves , undo , score , awayorClose , getKey , 0 , 0);
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
                undoStack.pushBack(playerPreX, playerPreY);
                playerX += 1;
                l1.updateTile(playerX , playerY , 'P');
                playerPreX = playerX;
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid(levelName , moves , undo, score , awayorClose , getKey, 0 , 0);
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
                undoStack.pushBack(playerPreX, playerPreY);
                playerY -= 1;
                l1.updateTile(playerX, playerY, 'P');
                playerPreY = playerY;
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid(levelName , moves , undo , score , awayorClose , getKey , 0 , 0);
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
                undoStack.pushBack(playerPreX, playerPreY);
                playerY += 1;
                l1.updateTile(playerX, playerY, 'P');
                playerPreY = playerY;
                
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid(levelName , moves , undo , score , awayorClose , getKey , 0 , 0);
            }
        }
        else if ((a == 'U' || a == 'u') && undo>0)
        {

            if (!(undoStack.isEmpty()))
            {
                int stackX = undoStack.getX();
                int stackY = undoStack.getY();
                p1.setPlayer(stackX, stackY, playerX, playerY, l1);
                playerX = playerPreX = stackX;
                playerY = playerPreY = stackY;
                cout << playerX << playerY;
                undoStack.pop();
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                undo -= 1;
                l1.printGrid(levelName, moves, undo, score, awayorClose, getKey, playerX, playerY);
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
            score += 2;
        }
        if ((moves <= 0) ||p1.checkBomb(playerX, playerY, bombs))
        {
            break;
        }
        
    }
    savedCoins.print();
    endwin();
    return 0;
}
