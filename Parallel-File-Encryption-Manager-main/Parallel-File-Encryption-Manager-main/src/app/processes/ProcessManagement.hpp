#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <memory>
#include <mutex>
#include <atomic>
#include <semaphore.h>
#include <thread>
#include <vector>
#include <iostream>
#include <functional>

class ProcessManagement
{
    sem_t* itemsSemaphore;
    sem_t* emptySlotsSemaphore;

public:
    ProcessManagement(unsigned int threadPoolSize = 0);
    ~ProcessManagement();

    bool submitToQueue(std::unique_ptr<Task> task);
    bool hasPendingTasks() const;

    // Callback when a task completes
    std::function<void(bool success)> onTaskComplete;

    // Key from GUI
    int userKey{0};

private:
    void executeTask();   // worker function

    static constexpr int QUEUE_SIZE = 1000;

    struct SharedMemory {
        std::atomic<int> size;
        char tasks[QUEUE_SIZE][1024];
        int front;
        int rear;
    };

    SharedMemory* sharedMem;
    int shmFd;
    const char* SHM_NAME = "/my_queue";

    std::mutex queueLock;
    std::vector<std::thread> workers;
    std::atomic<bool> stopPool{false};

    unsigned int poolSize;
};

#endif
