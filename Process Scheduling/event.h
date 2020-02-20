//
//  Yashwanth Reddy Gurijala
//  COSC 3360 - Operating System Fundamentals
//  Due Date - 2/19/2020
//
//  event.h
//
//  Created by Yashwanth Reddy Gurijala on 2/8/2020.
//  Copyright © 2020 Yashwanth Reddy Gurijala. All rights reserved.
//

#pragma once
#include <iostream>
#include <stdlib.h>
using namespace std;

//This class holds all the data related to the event
class event
{
public:
    int time;
    string command;
    int processID;
    int startingTime;
    string status;
    event* next;
};

//This class is a Linked List which holds event
class eventList
{
public:
    event* head;
    event* tail;
    int counter = 0;

    //This constructor sets the Linked List to null
    eventList()
    {
        head = nullptr;
        tail = nullptr;
    }

    //This method inserts the event to the bottom of the linked list 
    void push(int a, string b, int c, string d, int e)
    {
        counter++;
        event* temp = new event;

        temp->time = a;
        temp->command = b;
        temp->processID = c;
        temp->status = d;
        temp->startingTime = e;
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

    //This method inserts the event in sorted ascending order depending on their time
    //If two events have the same time then it sorts depending on startingTime
    void sortPush(int a, string b, int c, string d, int e)
    {
        counter++;
        event* temp = new event;

        temp->time = a;
        temp->command = b;
        temp->processID = c;
        temp->status = d;
        temp->startingTime = e;
        temp->next = nullptr;

        if (head == nullptr)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            event* check = head;
            event* checkPrev = nullptr;

            while (check != nullptr)
            {
                if (temp->time < check->time)
                {
                    if (checkPrev == nullptr)
                    {
                        temp->next = check;
                        head = temp;
                    }
                    else
                    {
                        checkPrev->next = temp;
                        temp->next = check;
                    }

                    return;
                }
                else if (temp->time == check->time)
                {
                    if (temp->startingTime < check->startingTime)
                    {
                        if (checkPrev == nullptr)
                        {
                            temp->next = check;
                            head = temp;
                        }
                        else
                        {
                            checkPrev->next = temp;
                            temp->next = check;
                        }

                        return;
                    }
                }

                checkPrev = check;
                check = check->next;
            }

            tail->next = temp;
            tail = temp;
        }
    }

    //This method removes the top event of the linked list
    void pop()
    {
        counter--;

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

    //This method prints the event list from top to bottom
    void print()
    {
        event* temp = new event;
        temp = head;

        while (temp != nullptr)
        {
            cout << temp->time << " " << temp->command << " Process:" << temp->processID << " " << temp->status << "\n";
            temp = temp->next;
        }
    }

    //This method checks if the list is empty
    bool isItEmpty()
    {
        if (head == nullptr)
            return true;
        else
            return false;
    }

    //This method returns the command vairable of the head event
    string getTopCommand()
    {
        event* temp = new event;
        temp = head;

        if (temp != nullptr)
            return head->command;
        else
            return "";
    }

    //This method returns the time vairable of the head event
    int getTopTime()
    {
        event* temp = new event;
        temp = head;

        if (temp != nullptr)
            return head->time;
        else
            return -1;
    }

    //This method returns the ProcessId vairable of the head event
    int getTopProcessID()
    {
        event* temp = new event;
        temp = head;

        if (temp != nullptr)
            return head->processID;
        else
            return -1;
    }

    //This method returns the status vairable of the head event
    string getTopStatus()
    {
        event* temp = new event;
        temp = head;

        if (temp != nullptr)
            return head->status;
        else
            return "";
    }

    //This method clears the entire linked list
    void clear()
    {
        head = nullptr;
        tail = nullptr;
    }
};