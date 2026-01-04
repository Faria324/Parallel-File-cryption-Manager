#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <iostream> 

// Cryption.cpp
int executeCryption(const std::string& taskData, int key) {
    Task task = Task::fromString(taskData);

    // Read file into memory
    std::ifstream in(task.filePath, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Failed to open file: " << task.filePath << std::endl;
        return -1;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // Encrypt/decrypt using the provided key
    for (auto& ch : buffer) {
        ch = (task.action == Action::ENCRYPT) ? (ch + key) % 256 : (ch - key + 256) % 256;
    }

    // Write back
    std::ofstream out(task.filePath, std::ios::binary | std::ios::trunc);
    if (!out.is_open() || !out.write(buffer.data(), buffer.size())) {
        std::cerr << "Failed to write file: " << task.filePath << std::endl;
        return -1;
    }

    return 0;
}
