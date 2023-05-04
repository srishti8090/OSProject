#include<windows.h>
#include<Psapi.h>
#include <iostream>
#include <queue>
#include <vector>
#pragma comment(lib,"Psapi.lib")
using namespace std;
class process
{
public:
    size_t size;
    pid_t no;
};
class memory
{
public:
    size_t size;
    pid_t no;
    queue<process> space_occupied;
    void push(const process p)
    {
        if (p.size <= size)
        {
            space_occupied.push(p);
            size -= p.size;
        }
    }
    process pop()
    {
        process p;
        if (!space_occupied.empty())
        {
            p = space_occupied.front();
            space_occupied.pop();
            size += p.size;
            return p;
        }
        return p;
    }
    bool empty()
    {
        return space_occupied.empty();
    }
};
vector<memory> best_fit(vector<memory> memory_blocks, queue<process> processess)
{
    int i = 0, min, index = 0;
    memory na;
    na.no = -10;
    while (!processess.empty())
    {
        min = 0;
        for (i = 0; i < memory_blocks.size(); i++)
        {
            if (memory_blocks.at(i).size >= processess.front().size && (min == 0 || memory_blocks.at(i).size < min))
            {
                min = memory_blocks.at(i).size;
                index = i;
            }
        }
        if (min != 0)
        {
            memory_blocks.at(index).push(processess.front());
        }
        else
        {
            na.size += processess.front().size;
            na.push(processess.front());
        }
        processess.pop();
    }
    if (!na.space_occupied.empty())
    {
        memory_blocks.push_back(na);
    }
    return memory_blocks;
}
void display(vector<memory> memory_blocks)
{
    int i = 0;
    int temp = 0;
    process p;
    cout << "+-------------+--------------+--------------+" << endl;
    cout << "| Process no. | Process size | Memory block |" << endl;
    cout << "+-------------+--------------+--------------+" << endl;
    for (i = 0; i < memory_blocks.size(); i++)
    {
        while (!memory_blocks.at(i).empty())
        {
            p = memory_blocks.at(i).pop();
            temp = to_string(p.no).length();
            cout << "|" << string(7 - temp / 2 - temp % 2, ' ') << p.no << string(6 - temp / 2, ' ') << "|";
            temp = to_string(p.size).length();
            cout << string(7 - temp / 2 - temp % 2, ' ') << p.size << string(7 - temp / 2, ' ') << "|";
            temp = to_string(memory_blocks.at(i).no).length();
            cout << string(7 - temp / 2 - temp % 2, ' ');
            if (memory_blocks.at(i).no != -10)
            {
                cout << memory_blocks.at(i).no;
            }
            else
            {
                cout << "N/A";
            }
            cout << string(7 - temp / 2, ' ')<< "|" << endl;
        }
    }
    cout << "+-------------+--------------+--------------+"<< endl;
}
int api_inclusion()
{
    vector<memory> memory_blocks(5);
    vector<memory> best_fit_blocks;
    queue<process> processess;
    process temp;
    memory_blocks[0].no = 1;
    memory_blocks[0].size = 4000;
    memory_blocks[1].no = 2;
    memory_blocks[1].size = 5000;
    memory_blocks[2].no = 3;
    memory_blocks[2].size = 3000;
    memory_blocks[3].no = 4;
    memory_blocks[3].size = 2000;
    memory_blocks[4].no = 5;
    memory_blocks[4].size = 1000;

    STARTUPINFO si1 = { 0 }, si2 = { 0 }, si3 = { 0 };
    PROCESS_INFORMATION pi1 = { 0 }, pi2 = { 0 }, pi3 = { 0 };
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
    temp.no = 1;
    temp.size = n1;
    processess.push(temp);
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
    temp.no = 2;
    temp.size = n2;
    processess.push(temp);

    if (!CreateProcess(
        NULL,
        const_cast<LPSTR>(command3),
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
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
    if (GetProcessMemoryInfo(pi3.hProcess, &pmc3, sizeof(pmc3)))
    {
        cout << "Process 3 memory usage: " << pmc3.WorkingSetSize << " bytes" << endl;
    }
    int n3=pmc3.WorkingSetSize/1000;//to convert into kilobytes
    temp.no = 3;
    temp.size = n3;
    processess.push(temp);

    WaitForSingleObject(pi1.hProcess, INFINITE);
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    cout << "Child process 1 finished executing and released its resources!" << endl;

    WaitForSingleObject(pi2.hProcess, INFINITE);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
    cout << "Child process 2 finished executing and released its resources!" << endl;

    WaitForSingleObject(pi3.hProcess, INFINITE);
    CloseHandle(pi3.hProcess);
    CloseHandle(pi3.hThread);
    cout << "Child process 3 finished executing and released its resources!" << endl;

    best_fit_blocks = best_fit(memory_blocks,processess);
    display(best_fit_blocks);
    memory_blocks.clear();
    memory_blocks.shrink_to_fit();
    best_fit_blocks.clear();
    best_fit_blocks.shrink_to_fit();
    return 0;
}
