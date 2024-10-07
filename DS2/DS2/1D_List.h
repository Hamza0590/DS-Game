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
        do
        {
            cout << '(' << temp->x << ',' << temp->y << ')';
            temp = temp->next;
        } while (temp != NULL);
    }
    /*void deleteNodes(gridlist l1)
    {
        node* temp;
        temp = head;
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
            traverseforDeletion->coin = '.';
            temp = temp->next;
            delete curr;
        }
        head = NULL;
    }*/

};
