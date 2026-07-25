#include "task.h"
#include <iostream>
#include <iomanip>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef max
#undef max
#endif

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void printTasksTable(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "[Empty] No tasks found\n";
        return;
    }

    std::cout << "\n+----+--------------------------------------+-------------+\n";
    std::cout << "| ID | Title                                | Status      |\n";
    std::cout << "+----+--------------------------------------+-------------+\n";

    for (const auto& task : tasks) {
        std::string status = task.is_done ? "DONE" : "ACTIVE";
        std::string title = task.title;
        if (title.length() > 36) title = title.substr(0, 34) + "..";

        std::cout << "| " << std::setw(2) << task.id << " | "
            << std::setw(36) << std::left << title << " | "
            << std::setw(11) << status << " |\n";
    }
    std::cout << "+----+--------------------------------------+-------------+\n";
    std::cout << "Total: " << tasks.size() << " tasks\n";
}

void showMenu() {
    clearScreen();
    std::cout << "\n";
    std::cout << "+=========================================+\n";
    std::cout << "|           TASK MANAGER                  |\n";
    std::cout << "+=========================================+\n";
    std::cout << "|  1. Add task                           |\n";
    std::cout << "|  2. Show all tasks                    |\n";
    std::cout << "|  3. Show active tasks                 |\n";
    std::cout << "|  4. Show done tasks                   |\n";
    std::cout << "|  5. Mark task as done                 |\n";
    std::cout << "|  6. Delete task                       |\n";
    std::cout << "|  7. Statistics                        |\n";
    std::cout << "|  0. Exit                              |\n";
    std::cout << "+=========================================+\n";
    std::cout << "Choose action: ";
}

void handleAddTask() {
    clearScreen();
    std::cout << "=== ADD TASK ===\n\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string title, description;
    std::cout << "Title: ";
    std::getline(std::cin, title);

    std::cout << "Description: ";
    std::getline(std::cin, description);

    if (title.empty()) {
        std::cout << "[ERROR] Title cannot be empty!\n";
    }
    else {
        if (addTask(title, description)) {
            std::cout << "[OK] Task added successfully!\n";
        }
        else {
            std::cout << "[ERROR] Failed to add task\n";
        }
    }
    waitForEnter();
}

void handleShowAll() {
    clearScreen();
    std::cout << "=== ALL TASKS ===\n";
    auto tasks = getAllTasks();
    printTasksTable(tasks);
    waitForEnter();
}

void handleShowActive() {
    clearScreen();
    std::cout << "=== ACTIVE TASKS ===\n";
    auto tasks = getTasksByStatus(0);
    printTasksTable(tasks);
    waitForEnter();
}

void handleShowDone() {
    clearScreen();
    std::cout << "=== DONE TASKS ===\n";
    auto tasks = getTasksByStatus(1);
    printTasksTable(tasks);
    waitForEnter();
}

void handleMarkDone() {
    clearScreen();
    std::cout << "=== MARK AS DONE ===\n\n";
    auto tasks = getTasksByStatus(0);

    if (tasks.empty()) {
        std::cout << "[Empty] No active tasks\n";
        waitForEnter();
        return;
    }

    printTasksTable(tasks);
    std::cout << "\nEnter task ID to mark as done: ";

    int id;
    std::cin >> id;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[ERROR] Invalid input\n";
        waitForEnter();
        return;
    }

    if (markTaskDone(id)) {
        std::cout << "[OK] Task marked as done!\n";
    }
    else {
        std::cout << "[ERROR] Task not found or error\n";
    }
    waitForEnter();
}

void handleDeleteTask() {
    clearScreen();
    std::cout << "=== DELETE TASK ===\n\n";
    auto tasks = getAllTasks();

    if (tasks.empty()) {
        std::cout << "[Empty] No tasks to delete\n";
        waitForEnter();
        return;
    }

    printTasksTable(tasks);
    std::cout << "\nEnter task ID to delete: ";

    int id;
    std::cin >> id;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[ERROR] Invalid input\n";
        waitForEnter();
        return;
    }

    std::cout << "Are you sure? (y/n): ";
    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        if (deleteTask(id)) {
            std::cout << "[OK] Task deleted!\n";
        }
        else {
            std::cout << "[ERROR] Task not found or error\n";
        }
    }
    else {
        std::cout << "Operation cancelled\n";
    }
    waitForEnter();
}

void handleStats() {
    clearScreen();
    std::cout << "=== STATISTICS ===\n\n";

    auto allTasks = getAllTasks();
    auto activeTasks = getTasksByStatus(0);
    auto doneTasks = getTasksByStatus(1);

    std::cout << "Total tasks: " << allTasks.size() << "\n";
    std::cout << "Active: " << activeTasks.size() << "\n";
    std::cout << "Done: " << doneTasks.size() << "\n";
    if (!allTasks.empty()) {
        std::cout << "Progress: " << (doneTasks.size() * 100 / allTasks.size()) << "%\n";
    }
    else {
        std::cout << "Progress: 0%\n";
    }

    waitForEnter();
}