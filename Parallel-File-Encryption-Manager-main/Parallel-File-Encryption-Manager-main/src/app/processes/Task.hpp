#ifndef TASK_HPP
#define TASK_HPP

#include "../fileHandling/IO.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>

enum class Action { ENCRYPT, DECRYPT };

struct Task {
    std::string filePath;
    Action action;

    // Constructor
    Task(std::string filePath, Action action)
        : filePath(std::move(filePath)), action(action) {}

    // Disable copy
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    // Enable move
    Task(Task&&) = default;
    Task& operator=(Task&&) = default;

    // Convert Task to string
    std::string toString() const {
        return filePath + "," + (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
    }

    // Create Task from string
    static Task fromString(const std::string& taskData) {
        std::istringstream iss(taskData);
        std::string filePath;
        std::string actionStr;

        if (std::getline(iss, filePath, ',') && std::getline(iss, actionStr)) {
            Action action = (actionStr == "ENCRYPT") ? Action::ENCRYPT : Action::DECRYPT;
            return Task(filePath, action);
        }

        throw std::runtime_error("Invalid task data format: " + taskData);
    }
};

#endif // TASK_HPP
