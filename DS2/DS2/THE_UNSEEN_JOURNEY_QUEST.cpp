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
    int gridSize, undo = 0, level = 0 ,moves =0 , keyX = 0, keyY = 0, getKey = 0 , doorX, doorY ,
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
    l1.makeGrid(gridSize , levelName);
    player p1(l1 ,undo);
    int playerX = p1.getPlayerX(), playerY = p1.getPlayerY();
    int playerPreX = playerX, playerPreY = playerY , initialPlayerX = playerX , initialPlayerY = playerY;
    do
    {
        keyX = rand() % gridSize, keyY = rand() % gridSize;
    } while (keyX == playerX && keyY == playerY);
    do
    {
        doorX = rand() % gridSize, doorY = rand() % gridSize;
    } while ((doorX == playerX && doorY == playerY) && (doorX==keyX && doorY==keyY));
    l1.setDoorCor(doorX, doorY);
    l1.setKeyCor(keyX, keyY);
    l1.updateTile(keyX , keyY , 'K');
    l1.updateTile(doorX, doorY, 'D');
    list savedCoins;
    l1.drawCoins(playerX, playerY , level);
    list initialCoins;
    initialCoins = l1.getCoins();
    l1.drawBomb(playerX, playerY,level);
    moves += p1.getMoves(playerX, playerY, doorX, doorY , keyX, keyY);
    l1.printGrid( moves , undo , p1.getScore(), 3, getKey, 0, 0);
    delNode d1;
    stackList undoStack;
    int a;
    auto lastUpdate = std::chrono::steady_clock::now();
    p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
    while (true)
    {
        playerX = p1.getPlayerX(), playerY = p1.getPlayerY();
        a = getch();
        l1.updateTile(doorX, doorY, 'D');
        auto now = std::chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::seconds>(now - lastUpdate).count() >= coinInterval) 
        {
            /*if (initialCoins.isEmpty())
            {
                initialCoins = l1.getCoins();
            };
            cout << "INitial Coins: ";
            initialCoins.print();
            cout << endl;*/
            l1.delCoins();
            l1.drawCoins(playerX, playerY,level);
            lastUpdate = now;
        }
        if (a == 27)
        {
            break;
        }
        if (a == KEY_UP && upCheck)
        {
            
            downCheck = 0;
            if (playerX>0)
            {
                moves -= 1;
                upCheck = 1;
                rightCheck = 1;
                leftCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                undoStack.pushBack(playerPreX, playerPreY);
                playerX -= 1;
                l1.updateTile(playerX , playerY, 'P');
                playerPreX = playerX;
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid( moves , undo , p1.getScore(), awayorClose, getKey, 0, 0);
            }
        }
        else if (a == KEY_DOWN && downCheck)
        {    
            upCheck = 0;
            if (playerX < gridSize-1)
            {
                moves -= 1;
                downCheck = 1;
                rightCheck = 1;
                leftCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                undoStack.pushBack(playerPreX, playerPreY);
                playerX += 1;
                l1.updateTile(playerX , playerY , 'P');
                playerPreX = playerX;
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid( moves , undo, p1.getScore(), awayorClose, getKey, 0, 0);
            }
        }
        else if (a == KEY_LEFT && leftCheck)
        {
            rightCheck = 0;
            if (playerY > 0)
            {
                moves -= 1;
                upCheck = 1;
                downCheck = 1;
                leftCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                undoStack.pushBack(playerPreX, playerPreY);
                playerY -= 1;
                l1.updateTile(playerX, playerY, 'P');
                playerPreY = playerY;
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid( moves , undo , p1.getScore() , awayorClose , getKey , 0 , 0);
            }
        }
        else if (a == KEY_RIGHT && rightCheck)
        {
           
            leftCheck = 0;
            if (playerY < gridSize-1)
            {
                moves -= 1;
                upCheck = 1;
                downCheck = 1;
                rightCheck = 1;
                l1.updateTile(playerPreX, playerPreY, '.');
                undoStack.pushBack(playerPreX, playerPreY);
                playerY += 1;
                l1.updateTile(playerX, playerY, 'P');
                playerPreY = playerY;
                
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                l1.printGrid( moves , undo , p1.getScore(), awayorClose, getKey, 0, 0);
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
                undoStack.pop();
                int awayorClose = p1.getHint(playerX, playerY, keyX, keyY, doorX, doorY, getKey, previous);
                undo -= 1;
                l1.printGrid( moves, undo, p1.getScore(), awayorClose, getKey, playerX, playerY);
            }
        }
        p1.setPlayerX(playerX), p1.setPlayerY(playerY);
        if (p1.checkKey(playerX, playerY, keyX, keyY))
        {
            l1.updateTile(keyX, keyY, '.');
            getKey = 1;
        }
        if (p1.checkdoor( doorX, doorY, getKey) || (moves <= 0) || p1.checkBomb( l1.getBomb()))
        {
            if (moves)
            {
                p1.setScore(moves);
            }   
            if (moves <= 0)
            {
                l1.updateTile(playerX, playerY, '.');
            }
            else if (p1.checkBomb( l1.getBomb()))
            {
                l1.updateTile(playerX, playerY, 'B');
            }
            l1.printEndDisplay( initialPlayerX , initialPlayerY, p1.getPlayerX(), p1.getPlayerY() , p1.getScore(), initialCoins, savedCoins);
            break;
        }
        
        if (p1.checkCoin(playerX, playerY, l1.getCoins(), savedCoins))
        {
            undo += 1;
            p1.setScore(2);
        }
    }
    while (1)
    {
        a = getch();
        if (a)
            break;
    }
    endwin();
    return 0;
}
