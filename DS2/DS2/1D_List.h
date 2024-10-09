#pragma once
#include<iostream>
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
        if (temp == NULL)
            return;
        do
        {
            cout << '(' << temp->x << ',' << temp->y << ')';
            temp = temp->next;
        } while (temp != NULL);
    }

};
