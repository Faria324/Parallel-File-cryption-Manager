#include <iostream>
#include "Cryption.hpp"
#include <cstdlib> // for std::stoi

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./cryption <task_data> <key>" << std::endl;
        return 1;
    }

    std::string taskData = argv[1];
    int key = 0;
    try {
        key = std::stoi(argv[2]);
    } catch (...) {
        std::cerr << "Invalid key provided" << std::endl;
        return 1;
    }

    int result = executeCryption(taskData, key);
    return result == 0 ? 0 : 1;
}
