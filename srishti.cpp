#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib") // Link with Psapi.lib
#include<bits/stdc++.h>
#include<vector>
using namespace std;
int size;
vector<pair<int, int>> arr[100000];
map<int, int> mp;
void Buddy(int s)
{
    int n = ceil(log(s) / log(2));
    size = n + 1;
    for(int i = 0; i <= n; i++)
    {
        arr[i].clear();
    }
    arr[n].push_back(make_pair(0, s - 1));
}
void allocation(int s)
{
    int x = ceil(log(s) / log(2));
    if (arr[x].size() > 0)
    {
        pair<int, int> temp = arr[x][0];
        arr[x].erase(arr[x].begin());
        cout << "Memory from " << temp.first << " to " << temp.second << " allocated" << "\n";
        mp[temp.first] = temp.second - temp.first + 1;
    }
    else
    {
        int i;
        for(i = x + 1; i < size; i++)
        {
            if (arr[i].size() != 0)
            {
                break;
            }
        }
        if (i == size)
        {
            cout << "Sorry, failed to allocate memory\n";
        }
        else
        {
            pair<int, int> temp;
            temp = arr[i][0];
            arr[i].erase(arr[i].begin());
            i--;
            for(;i >= x; i--)
            {
                pair<int, int> pair1, pair2;
                pair1 = make_pair(temp.first,  temp.first + (temp.second -  temp.first) / 2);
                pair2 = make_pair(temp.first + (temp.second -  temp.first + 1) / 2,  temp.second);
                arr[i].push_back(pair1);
                arr[i].push_back(pair2);
                temp = arr[i][0];
                arr[i].erase(arr[i].begin());
            }
            cout << "Memory from " << temp.first << " to " << temp.second << " allocate" << "\n";
            mp[temp.first] = temp.second - temp.first + 1;
        }
    }
}
void deallocation(int id)
{
    if(mp.find(id) == mp.end())
    {
        cout << "Sorry, invalid free request\n";
        return;
    }
    int n = ceil(log(mp[id]) / log(2));
    int i;
    int buddyNumber;
    int buddyAddress;
    arr[n].push_back(make_pair(id,   id + pow(2, n) - 1));
    cout << "Memory block from " << id << " to "<< id + pow(2, n) - 1 << " freed\n";
    if (buddyNumber % 2 != 0)
    {
        buddyAddress = id - pow(2, n);
    }
    else
    {
        buddyAddress = id + pow(2, n);
    }
    for(i = 0; i < arr[n].size(); i++)
    {
        if (arr[n][i].first == buddyAddress)
        {
            if (buddyNumber % 2 == 0)
            {
                arr[n + 1].push_back(make_pair(id, id + 2 * pow(2, n) - 1));
                cout << "Coalescing of blocks starting at " << id << " and " << buddyAddress << " was done" << "\n";
            }
            else
            {
                arr[n + 1].push_back(make_pair( buddyAddress, buddyAddress + 2 * pow(2, n) - 1));
                cout << "Coalescing of blocks starting at " << buddyAddress << " and " << id << " was done" << "\n";
            }
            arr[n].erase(arr[n].begin() + i);
            arr[n].erase(arr[n].begin() + arr[n].size() - 1);
            break;
        }
    }
    mp.erase(id);
}
int API_INCLUSION()
{
    STARTUPINFO si1 = { 0 }, si2 = { 0 } , si3 = {0};
    PROCESS_INFORMATION pi1 = { 0 }, pi2 = { 0 }, pi3 = {0};
    LPCSTR command1 = "notepad.exe";
    LPCSTR command2 = "calc.exe";
    LPCSTR command3 = "cmd.exe";
    if (!CreateProcess(
        NULL,
        const_cast<LPSTR>(command1),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si1,
        &pi1)
        )
    {
        cout << "Failed to create process 1: " << GetLastError() << endl;
        return 1;
    }
    cout << "Child process 1 created successfully!" << endl;

    PROCESS_MEMORY_COUNTERS pmc1 = { 0 };
    if (GetProcessMemoryInfo(pi1.hProcess, &pmc1, sizeof(pmc1)))
    {
        cout << "Process 1 memory usage: " << pmc1.WorkingSetSize << " bytes" << endl;
    }
    float n1=pmc1.WorkingSetSize/1000;//to convert into kilobytes
    allocation(n1);

    if (!CreateProcess(
        NULL,
        const_cast<LPSTR>(command2),
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si2,
        &pi2)
        )
    {
        cout << "Failed to create process 2: " << GetLastError() << endl;
        return 1;
    }
    cout << "Child process 2 created successfully!" << endl;

    PROCESS_MEMORY_COUNTERS pmc2 = { 0 };
    if (GetProcessMemoryInfo(pi2.hProcess, &pmc2, sizeof(pmc2)))
    {
        cout << "Process 2 memory usage: " << pmc2.WorkingSetSize << " bytes" << endl;
    }
    int n2=pmc2.WorkingSetSize/1000;//to convert into kilobytes
    allocation(n2);


    if (!CreateProcess(
        NULL,
        const_cast<LPSTR>(command3),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si3,
        &pi3)
        )
    {
        cout << "Failed to create process 3: " << GetLastError() << endl;
        return 1;
    }
    cout << "Child process 3 created successfully!" << endl;

    PROCESS_MEMORY_COUNTERS pmc3 = { 0 };
    if (GetProcessMemoryInfo(pi1.hProcess, &pmc3, sizeof(pmc1)))
    {
        cout << "Process 3 memory usage: " << pmc3.WorkingSetSize << " bytes" << endl;
    }
    float n3=pmc3.WorkingSetSize/1000;//to convert into kilobytes
    allocation(n3);

    WaitForSingleObject(pi1.hProcess, INFINITE);
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    cout << "Child process 1 finished executing and released its resources!" << endl;
    deallocation(0);

    WaitForSingleObject(pi2.hProcess, INFINITE);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
    cout << "Child process 2 finished executing and released its resources!" << endl;
    deallocation(n1);

    WaitForSingleObject(pi3.hProcess, INFINITE);
    CloseHandle(pi3.hProcess);
    CloseHandle(pi3.hThread);
    cout << "Child process 3 finished executing and released its resources!" << endl;
    deallocation(n2);

    return 0;
}
