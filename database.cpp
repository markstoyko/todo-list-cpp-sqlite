#include "database.h"
#include <iostream>

static sqlite3* db = nullptr;

static bool executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void initDatabase() {
    int rc = sqlite3_open("tasks.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open DB: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            is_done INTEGER DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    if (executeSQL(createTableSQL)) {
        std::cout << "Database ready\n";
    }
}

bool addTask(const std::string& title, const std::string& description) {
    std::string sql = "INSERT INTO tasks (title, description) VALUES ('" + title + "', '" + description + "');";
    return executeSQL(sql);
}

static int callbackReadTasks(void* data, int argc, char** argv, char** azColName) {
    auto* tasks = static_cast<std::vector<Task>*>(data);
    Task task;
    task.id = std::stoi(argv[0] ? argv[0] : "0");
    task.title = argv[1] ? argv[1] : "";
    task.description = argv[2] ? argv[2] : "";
    task.is_done = std::stoi(argv[3] ? argv[3] : "0");
    task.created_at = argv[4] ? argv[4] : "";
    tasks->push_back(task);
    return 0;
}

std::vector<Task> getAllTasks() {
    std::vector<Task> tasks;
    std::string sql = "SELECT id, title, description, is_done, created_at FROM tasks ORDER BY id;";
    char* errMsg = nullptr;
    sqlite3_exec(db, sql.c_str(), callbackReadTasks, &tasks, &errMsg);
    if (errMsg) {
        std::cerr << "Read error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    return tasks;
}

std::vector<Task> getTasksByStatus(int status) {
    std::vector<Task> tasks;
    std::string sql = "SELECT id, title, description, is_done, created_at FROM tasks WHERE is_done = " + std::to_string(status) + " ORDER BY id;";
    char* errMsg = nullptr;
    sqlite3_exec(db, sql.c_str(), callbackReadTasks, &tasks, &errMsg);
    if (errMsg) {
        std::cerr << "Read error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    return tasks;
}

bool markTaskDone(int id) {
    std::string sql = "UPDATE tasks SET is_done = 1 WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

bool deleteTask(int id) {
    std::string sql = "DELETE FROM tasks WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

int getTaskCount() {
    int count = 0;
    std::string sql = "SELECT COUNT(*) FROM tasks;";
    char* errMsg = nullptr;
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        if (argc > 0 && argv[0]) {
            *static_cast<int*>(data) = std::stoi(argv[0]);
        }
        return 0;
        };
    sqlite3_exec(db, sql.c_str(), callback, &count, &errMsg);
    if (errMsg) {
        std::cerr << "Count error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    return count;
}