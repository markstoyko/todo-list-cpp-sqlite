#ifndef TASK_H
#define TASK_H

#include "database.h"
#include <vector>

void clearScreen();
void waitForEnter();
void printTasksTable(const std::vector<Task>& tasks);
void showMenu();
void handleAddTask();
void handleShowAll();
void handleShowActive();
void handleShowDone();
void handleMarkDone();
void handleDeleteTask();
void handleStats();

#endif