#pragma once
#include<iostream>
class stackNode
{
public:
	int x, y;
	stackNode* next;
	stackNode()
	{
		x = y  = 0;
		next = NULL;
	}
};
class stackList
{
public:
	stackNode* head;
	int count;
	stackList()
	{
		head = NULL;
		count = 0;
	}
	void pushBack(int valX , int valY)
	{
		stackNode* temp = new stackNode();
		temp->x = valX;
		temp->y = valY;
		if (head == NULL)
			head = temp;
		else
		{
			stackNode* traverse = head;
			while (traverse->next != NULL)
			{
				traverse = traverse->next;
			}
			traverse->next = temp;
		}
		count++;
	}
	int isEmpty()
	{
		if (head == NULL)
			return 1;
		else
			return 0;
	}
	int getX()
	{
		stackNode* temp = head;
		for (int i = 0; i < count-1; i++)
		{
			temp = temp->next;
		}
		return temp->x;
	}
	int getY()
	{
		stackNode* temp = head;
		for (int i = 0; i < count-1; i++)
		{
			temp = temp->next;
		}
		return temp->y;
	}
	void pop()
	{
		stackNode* temp = head;
		stackNode* curr = temp;
		for (int i = 0; i < count - 1; i++)
		{
			curr = temp;
			temp = temp->next;
		}
		delete temp;
		curr->next = NULL;
		count -= 1;
		if (count == 0)
			head = NULL;
	}
};
