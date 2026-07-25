#include "database.h"
#include "task.h"
#include <iostream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef max
#undef max
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    std::cout << "========================================\n";
    std::cout << "   TASK MANAGER v1.0                   \n";
    std::cout << "========================================\n";

    initDatabase();

    int choice = -1;

    while (choice != 0) {
        showMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid input. Try again.\n";
            waitForEnter();
            continue;
        }

        switch (choice) {
        case 1: handleAddTask(); break;
        case 2: handleShowAll(); break;
        case 3: handleShowActive(); break;
        case 4: handleShowDone(); break;
        case 5: handleMarkDone(); break;
        case 6: handleDeleteTask(); break;
        case 7: handleStats(); break;
        case 0:
            std::cout << "\nGoodbye! Data saved to tasks.db\n";
            break;
        default:
            std::cout << "[ERROR] Unknown command. Try again.\n";
            waitForEnter();
            break;
        }
    }

    return 0;
}