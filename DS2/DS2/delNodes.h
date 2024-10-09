#pragma once
#include"multi_Dim_list.h"
#include"1D_List.h"
class delNode
{
public:
    void deleteNodes(gridlist& l1, list& coin)
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