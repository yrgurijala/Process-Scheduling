//
//  Yashwanth Reddy Gurijala
//  COSC 3360 - Operating System Fundamentals
//  Due Date - 2/19/2020
//
//  command.h
//
//  Created by Yashwanth Reddy Gurijala on 2/3/2020.
//  Copyright © 2020 Yashwanth Reddy Gurijala. All rights reserved.
//

#pragma once
#include <iostream>
#include <stdlib.h>
using namespace std;

//This class holds all the data related to a command
class command
{
public:
    string line;
    int timeConstraint;
    command* next;
};

//This class is a linked list which stores all commands sequentially
class commandList
{
public:
    command* head, * tail;

    //This constructor sets the list to null
    commandList()
    {
        head = nullptr;
        tail = nullptr;
    }

    //This method inserts the command to the bottom of the list
    void push(string a, int b)
    {
        command* temp = new command;

        temp->line = a;
        temp->timeConstraint = b;
        temp->next = nullptr;

        if (head == nullptr)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }
    }

    //This method prints all the commands that are in the list
    void print()
    {
        command* temp = new command();
        temp = head;

        while (temp != nullptr)
        {
            cout << temp->line << " " << temp->timeConstraint << "\n";
            temp = temp->next;
        }
    }

    //This method deleted the entire list
    void clear()
    {
        head = nullptr;
        tail = nullptr;
    }

    //This method deletes the top command of the list
    void pop()
    {
        if (head == tail)
        {
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            head = head->next;
        }
    }

    //This method returns the line of the top command
    string returnCommandLine()
    {
        if (head == nullptr)
            return "";
        else
            return head->line;
    }

    //This method returns the time of the top command
    int returnCommandTime()
    {
        if (head == nullptr)
            return 0;
        else
            return head->timeConstraint;
    }

    //This method checks if the list is empty
    bool isItEmpty()
    {
        command* temp = new command();
        temp = head;

        if (temp == nullptr)
            return true;
        else
            return false;
    }
};