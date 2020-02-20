//
//  Yashwanth Reddy Gurijala
//  COSC 3360 - Operating System Fundamentals
//  Due Date - 2/19/2020
//  
//  Description:
//  This assingment simulates the execution of processess with
//  multi-core processing unit and one Solid-State-Drive
//
//  Main.cpp
//
//  Created by Yashwanth Reddy Gurijala on 2/3/2020.
//  Copyright © 2020 Yashwanth Reddy Gurijala. All rights reserved.
//

#include <fstream>
#include <iomanip>
#include "event.h"
#include "process.h"
using namespace std;

processList myProcesses;
commandList myCommands;
eventList myEvents;
eventList iQueue;
eventList niQueue;
eventList ssdQueue;

int timeTaken = 0;
int process = 0;
int noOfFreeCores = 0;
int ssdAccess = 0;
int coreTime = 0;
int ssdTime = 0;
int ssdBusyTime = 0;

string noOfFreeSSD = "free";

//This function checks for the next command of the process which just completed an event
void checkNextCommand(int checkProcID)
{
    string checkCommand = myProcesses.returnCommand(checkProcID);
    int checkTime = myProcesses.returnTime(checkProcID);

    if (checkCommand == "CORE")
    {
        if (noOfFreeCores > 0)
        {
            coreTime += checkTime;
            checkTime = timeTaken + checkTime;
            myProcesses.setStatus(checkProcID, "RUNNING");
            noOfFreeCores--;
            myEvents.sortPush(checkTime, checkCommand, checkProcID, "non-interactive", myProcesses.returnStartingTime(checkProcID));
        }
        else
        {
            if (myEvents.getTopStatus() == "non-interactive")
                niQueue.push(checkTime, checkCommand, checkProcID, "non-interactive", myProcesses.returnStartingTime(checkProcID));
            else
                iQueue.push(checkTime, checkCommand, checkProcID, "non-interactive", myProcesses.returnStartingTime(checkProcID));

            myProcesses.setStatus(checkProcID, "READY");
        }
    }
    else
    {
        checkTime = timeTaken + checkTime;
        myProcesses.setStatus(checkProcID, "BLOCKED");

        if (checkCommand == "TTY")
            myEvents.sortPush(checkTime, checkCommand, checkProcID, "interactive", myProcesses.returnStartingTime(checkProcID));
        else
        {
            ssdTime += checkTime - timeTaken;

            if (noOfFreeSSD == "free")
            {
                ssdAccess++;
                myEvents.sortPush(checkTime, checkCommand, checkProcID, "non-interactive", myProcesses.returnStartingTime(checkProcID));
                if (checkTime - timeTaken != 0)
                    noOfFreeSSD = "busy";
            }
            else
                ssdQueue.push(checkTime - timeTaken, checkCommand, checkProcID, "non-interactive", myProcesses.returnStartingTime(checkProcID));
        }
    }
}

int main()
{
    string checkFileLine;
    int checkFileTime;

    cin >>checkFileLine >> checkFileTime;
    noOfFreeCores = checkFileTime;

    while (cin)
    {
        cin >> checkFileLine;

        if (checkFileLine == "END")
        {
            myProcesses.push(myCommands);
            myCommands.clear();
        }
        else if(checkFileLine == "START")
        {
            string a1;
            int b1;

            if (process != 0)
            {
                myProcesses.push(myCommands);
                myCommands.clear();
            }
            
            cin >> checkFileTime >> a1 >> b1;
            myCommands.push(checkFileLine, checkFileTime);
            myCommands.push(a1, b1);
            myEvents.sortPush(checkFileTime, checkFileLine, b1, "non-interactive",checkFileTime);
            process++;
        }
        else
        {
            cin >> checkFileTime;
            myCommands.push(checkFileLine, checkFileTime);
        }
    }

    while (!myEvents.isItEmpty())
    {
        string checkCommand = myEvents.getTopCommand();
        int checkTime = myEvents.getTopTime();
        int checkProcID = myEvents.getTopProcessID();

        if (checkTime == timeTaken)
        {
            myEvents.pop();

            if (checkCommand == "START" && !myProcesses.checkProcessEnd(checkProcID))
            {
                cout << "Process " << checkProcID << " starts at time " << checkTime << " ms \n";
                myProcesses.print();
                cout << '\n';

                myProcesses.setActive(checkProcID, true);
                checkNextCommand(checkProcID);
            }
            else if (checkCommand == "CORE" && !myProcesses.checkProcessEnd(checkProcID))
            {
                noOfFreeCores++;
                int check1ProcID = 0;
                int checkTimeTaken = timeTaken;

                if (!iQueue.isItEmpty())
                {
                    check1ProcID = iQueue.getTopProcessID();
                    checkTime = iQueue.getTopTime();
                    coreTime += checkTime;
                    checkTime = checkTime + checkTimeTaken;
                    myProcesses.setStatus(check1ProcID, "RUNNING");
                    myEvents.sortPush(checkTime, checkCommand, check1ProcID, "non-interactive",myProcesses.returnStartingTime(check1ProcID));
                    iQueue.pop();
                    noOfFreeCores--;
                }
                else if (!niQueue.isItEmpty())
                {
                    check1ProcID = niQueue.getTopProcessID();
                    checkTime = niQueue.getTopTime();
                    coreTime += checkTime;
                    checkTime = checkTime + checkTimeTaken;
                    myProcesses.setStatus(check1ProcID, "RUNNING");
                    myEvents.sortPush(checkTime, checkCommand, check1ProcID, "non-interactive",myProcesses.returnStartingTime(check1ProcID));
                    niQueue.pop();
                    noOfFreeCores--;
                }

                checkNextCommand(checkProcID);
            }
            else if ((checkCommand == "SSD" or checkCommand == "TTY") && !myProcesses.checkProcessEnd(checkProcID))
            {
                if (checkCommand == "SSD")
                    noOfFreeSSD = "free";

                int check1ProcID = 0;
                int checkTimeTaken = timeTaken;

                if (!ssdQueue.isItEmpty() && checkCommand == "SSD")
                {
                    check1ProcID = ssdQueue.getTopProcessID();
                    checkTime = ssdQueue.getTopTime();
                    checkTime = checkTime + checkTimeTaken;
                    myEvents.sortPush(checkTime, checkCommand, check1ProcID, "non-interactive",myProcesses.returnStartingTime(check1ProcID));
                    ssdQueue.pop();
                    ssdAccess++;

                    if (checkTime != checkTimeTaken)
                        noOfFreeSSD="busy";
                }

                checkNextCommand(checkProcID);
            }
            else if (myProcesses.checkProcessEnd(checkProcID))
            {
                myProcesses.setStatus(checkProcID, "TERMINATED");
                cout << "Process " << checkProcID << " terminates at time " << timeTaken << " ms \n";

                noOfFreeCores++;
                noOfFreeSSD = "free";
                int check1ProcID = 0;
                int checkTimeTaken = timeTaken;

                if (!iQueue.isItEmpty())
                {
                    check1ProcID = iQueue.getTopProcessID();
                    checkTime = iQueue.getTopTime();
                    coreTime += checkTime;
                    checkTime = checkTime + checkTimeTaken;
                    myEvents.sortPush(checkTime, checkCommand, check1ProcID, "non-interactive", myProcesses.returnStartingTime(check1ProcID));
                    iQueue.pop();
                    noOfFreeCores--;
                    myProcesses.setStatus(check1ProcID, "RUNNING");
                }
                else if (!niQueue.isItEmpty())
                {
                    check1ProcID = niQueue.getTopProcessID();
                    checkTime = niQueue.getTopTime();
                    coreTime += checkTime;
                    checkTime = checkTime + checkTimeTaken;
                    myEvents.sortPush(checkTime, checkCommand, check1ProcID, "non-interactive", myProcesses.returnStartingTime(check1ProcID));
                    niQueue.pop();
                    noOfFreeCores--;
                    myProcesses.setStatus(check1ProcID, "RUNNING");
                }

                if (!ssdQueue.isItEmpty() && noOfFreeSSD == "free")
                {
                    check1ProcID = ssdQueue.getTopProcessID();
                    checkTime = ssdQueue.getTopTime();
                    checkTime = checkTime + checkTimeTaken;
                    myEvents.sortPush(checkTime, checkCommand, check1ProcID, "non-interactive", myProcesses.returnStartingTime(checkProcID));
                    ssdQueue.pop();
                    ssdAccess++;
                    if (checkTime - timeTaken != 0)
                        noOfFreeSSD = "busy";
                }

                myProcesses.print();
                cout << '\n';
                myProcesses.setActive(checkProcID, false);
            }

            myProcesses.deleteCommand(checkProcID);
        }

        if(myEvents.getTopTime() != timeTaken)
            timeTaken++;
    }

    cout << "SUMMARY: \n";
    cout << "Total Elapsed Time: " << timeTaken-1 << "ms \n";
    cout << "Number of processes that completed: " << process << '\n';
    cout << "Total number of SSD accesses: " << ssdAccess << '\n';
    cout << "Average number of busy cores: " << fixed << setprecision(3) << double(coreTime) / double(timeTaken) << '\n';
    cout << "SSD Utilization " << fixed << setprecision(3) << double(ssdTime)/double(timeTaken) << '\n';
}