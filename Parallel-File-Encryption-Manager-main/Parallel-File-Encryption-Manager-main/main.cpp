#include <iostream>
#include <filesystem>
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/processes/Task.hpp"
#include <ctime>
#include <iomanip>
#include <thread>
#include <algorithm>

namespace fs = std::filesystem;

int main() {
    std::string directory, action;

    //Get user input
    std::cout << "Enter directory: ";
    std::getline(std::cin, directory);

    std::cout << "Encrypt or decrypt: ";
    std::getline(std::cin, action);

    //Normalize action
    std::transform(action.begin(), action.end(), action.begin(), ::tolower);
    if (action != "encrypt" && action != "decrypt") {
        std::cerr << "Invalid action. Use 'encrypt' or 'decrypt'.\n";
        return 1;
    }

    // Validate directory
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Invalid directory path!\n";
        return 1;
    }

    // 4️⃣ Count tasks to dynamically size thread pool
    int fileCount = 0;
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (!entry.is_regular_file()) continue;
        auto ext = entry.path().extension().string();
        if (ext != ".txt" && ext != ".log" && ext != ".csv" && ext != ".pdf") continue;
        fileCount++;
    }

    if (fileCount == 0) {
        std::cout << "No valid files found for encryption/decryption.\n";
        return 0;
    }

    unsigned int cores = std::thread::hardware_concurrency();
    unsigned int threadPoolSize = std::min(static_cast<unsigned int>(fileCount), (cores > 0 ? cores : 4));
    std::cout << "Thread pool size: " << threadPoolSize << " for " << fileCount << " tasks.\n";

    // 5️⃣ Initialize process manager with dynamic thread pool
    ProcessManagement processManagement(threadPoolSize);

    // 6️⃣ Scan files and submit tasks
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            if (!entry.is_regular_file())
                continue;

            auto ext = entry.path().extension().string();
            if (ext != ".txt" && ext != ".log" && ext != ".csv" && ext != ".pdf")
                continue;

            std::string filePath = entry.path().string();
            Action taskAction = (action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;

            auto task = std::make_unique<Task>(filePath, taskAction);
            if (!processManagement.submitToQueue(std::move(task))) {
                std::cerr << "Queue full. Skipping: " << filePath << "\n";
            } else {
                std::time_t t = std::time(nullptr);
                std::tm* now = std::localtime(&t);
                std::cout << "Submitted task for: " << filePath
                          << " at " << std::put_time(now, "%Y-%m-%d %H:%M:%S")
                          << std::endl;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    }

    // 7️⃣ Wait until all tasks are processed
    while (processManagement.hasPendingTasks()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "All tasks completed!\n";
    return 0;
}
