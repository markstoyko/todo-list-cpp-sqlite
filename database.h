#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

struct Task {
    int id;
    std::string title;
    std::string description;
    int is_done;
    std::string created_at;

    Task() : id(0), title(""), description(""), is_done(0), created_at("") {}
};

void initDatabase();
bool addTask(const std::string& title, const std::string& description);
std::vector<Task> getAllTasks();
std::vector<Task> getTasksByStatus(int status);
bool markTaskDone(int id);
bool deleteTask(int id);
int getTaskCount();

#endif