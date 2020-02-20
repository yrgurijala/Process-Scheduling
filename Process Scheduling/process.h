//
//  Yashwanth Reddy Gurijala
//  COSC 3360 - Operating System Fundamentals
//  Due Date - 2/19/2020
//
//  process.h
//
//  Created by Yashwanth Reddy Gurijala on 2/3/2020.
//  Copyright © 2020 Yashwanth Reddy Gurijala. All rights reserved.
//

#pragma once
#pragma once
#include "command.h"
using namespace std;

//This class holds all the data related to process including
//  a Linked List named commands which has all the commands related
//  to the process
class process
{
public:
    int processId;
    int startingTime;
    int totalTime = 0;
    bool active = false;
    string status = "";
    commandList commands;
    process* next;
};

//This class is a Linked list which holds all processes
class processList
{
public:
    process* head, * tail;

    //This constructor sets the Linked List to null
    processList()
    {
        head = nullptr;
        tail = nullptr;
    }

    //This method inserts a new process into the bottom of the list
    void push(commandList c)
    {
        process* temp = new process;
        temp->commands = c;
        temp->next = nullptr;

        temp->startingTime = temp->commands.returnCommandTime();
        temp->commands.pop();
        temp->processId = temp->commands.returnCommandTime();
        temp->commands.pop();

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

    //This method prints all the process which are active in the linked list
    void print()
    {
        process* temp = new process();
        temp = head;
        int check = 0;

        cout << "Process Table: \n";

        while (temp != nullptr)
        {
            if (temp->active)
            {
                cout << "Process " << temp->processId << " is " << temp->status << '\n';
                check++;
            }

            temp = temp->next;
        }

        if (check == 0)
            cout << "There are no active processes \n";
    }

    //This method sets the status of a process,with process id a, to bool value b
    void setActive(int a, bool b)
    {
        process* temp = new process();
        temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
                temp->active = b;

            temp = temp->next;
        }
    }

    //This method clears the entire list
    void clear()
    {
        head = nullptr;
        tail = nullptr;
    }

    //This method removed the head of the list
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

    //This method removes the head command of a process with process id a
    void deleteCommand(int a)
    {
        process* temp = new process();
        temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
            {
                temp->commands.pop();
            }

            temp = temp->next;
        }
    }

    //This method checks to see if the list is empty
    bool isItEmpty()
    {
        process* temp = new process();
        temp = head;

        if (temp == nullptr)
            return true;
        else
            return false;
    }

    //This method returns the top command of a process specified by process id a
    string returnCommand(int a)
    {
        process* temp = new process();
        temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
                return temp->commands.returnCommandLine();

            temp = temp->next;
        }

        return "EMPTY";
    }

    // This method returns the time of the top command of a process specified by process id a
    int returnTime(int a)
    {
        process* temp = new process();
        temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
                return temp->commands.returnCommandTime();

            temp = temp->next;
        }

        return -1;
    }

    // This method sets the status of process, specified by processID a, to string value b
    void setStatus(int a, string b)
    {
        process* temp = new process();
        temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
            {
                temp->status = b;
                return;
            }

            temp = temp->next;
        }
    }

    //This method returns the status of the process specified by process id a
    string returnStatus(int a)
    {
        process* temp = new process();
        temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
                return(temp->status);

            temp = temp->next;
        }

        return "";
    }

    //This method checks if a process has completed all its commands
    bool checkProcessEnd(int a)
    {
        process* temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
                return temp->commands.isItEmpty();

            temp = temp->next;
        }

        return true;
    }

    //This method returns the starting time of a process
    int returnStartingTime(int a)
    {
        process* temp = head;

        while (temp != nullptr)
        {
            if (temp->processId == a)
                return temp->startingTime;

            temp = temp->next;
        }
    }
};