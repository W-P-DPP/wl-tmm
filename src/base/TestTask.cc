#include "Task.h"
#include <iostream>
#include "TaksMgr.h"
#include <thread>
#include <chrono>
using namespace tmms::base;

void TestTask()
{
    TaskPtr task1 = std::make_shared<Task>([](const TaskPtr &prt)
                                           { std::cout << "task1:" << 1000 << std::endl; },
                                           1000);
    TaskPtr task2 = std::make_shared<Task>([](const TaskPtr &prt)
                                           { std::cout << "task2:" << 1500 << std::endl;
                                            prt->Restart(); },
                                           1500);
    TaskPtr task3 = std::make_shared<Task>([](const TaskPtr &prt)
                                           { std::cout << "task3:" << 1500 << std::endl;prt->Restart(); },
                                           1500);
    TaskPtr task4 = std::make_shared<Task>([](const TaskPtr &prt)
                                           { std::cout << "task4:" << 30000 << std::endl;prt->Restart(); },
                                           30000);
    sTaskMsg->Add(task1);
    sTaskMsg->Add(task2);
    sTaskMsg->Add(task3);
    sTaskMsg->Add(task4);
}

// int main(int argc, char *argv[])
// {
//     TestTask();
//     while (1)
//     {
//         sTaskMsg->OnWork();
//         std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     }
// }