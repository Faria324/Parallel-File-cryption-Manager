#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"
#include <sys/mman.h>
#include <semaphore.h>
#include <thread>

ProcessManagement::ProcessManagement(unsigned int threadPoolSize) {
    // Use hardware threads concurrency if poolvsize not provided
    if (threadPoolSize == 0) {
        threadPoolSize = std::thread::hardware_concurrency();
        if (threadPoolSize == 0) threadPoolSize = 4; // fallback
    }
    poolSize = threadPoolSize;

    // Initialize semaphores
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, QUEUE_SIZE);

    if (itemsSemaphore == SEM_FAILED || emptySlotsSemaphore == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    // Shared memory
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    ftruncate(shmFd, sizeof(SharedMemory));

    sharedMem = static_cast<SharedMemory*>(
        mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0)
    );

    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);

    // START THREAD POOL dynamically
    for (unsigned int i = 0; i < poolSize; ++i) {
        workers.emplace_back(&ProcessManagement::executeTask, this);
    }

    std::cout << "Thread pool started with " << poolSize << " threads.\n";
}

ProcessManagement::~ProcessManagement() {
    stopPool.store(true); //Sets a flag that tells threads to stop

    // Wake all workers so they can exit
    for (unsigned int i = 0; i < poolSize; ++i) {
        sem_post(itemsSemaphore);
    }

    for (auto& t : workers) {
        if (t.joinable())
            t.join();  //wait for each thread to finish exe
    }

    munmap(sharedMem, sizeof(SharedMemory)); //unmaps shared memory
    shm_unlink(SHM_NAME);

    sem_close(itemsSemaphore);
    sem_close(emptySlotsSemaphore);
    sem_unlink("/items_semaphore");
    sem_unlink("/empty_slots_semaphore");
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    sem_wait(emptySlotsSemaphore);

    std::unique_lock<std::mutex> lock(queueLock);

    if (sharedMem->size.load() >= QUEUE_SIZE) {
        return false;
    }

   // Use strncpy to safely copy, prevent overflow
strncpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str(), 1023);
sharedMem->tasks[sharedMem->rear][1023] = '\0';  // ensure null termination

sharedMem->rear = (sharedMem->rear + 1) % QUEUE_SIZE;
sharedMem->size.fetch_add(1);

    lock.unlock();
    sem_post(itemsSemaphore); // wake ONE worker

    return true;
}

void ProcessManagement::executeTask() {
    while (!stopPool.load()) {
        // Wait for an available item
        if (sem_wait(itemsSemaphore) == -1) {
            if (errno == EINTR) continue;
            perror("sem_wait failed");
            break;
        }

        if (stopPool.load()) break;

        char taskStr[1024] = {0};

        {
            std::unique_lock<std::mutex> lock(queueLock);

            if (sharedMem->size.load() == 0)
                continue;

            strncpy(taskStr, sharedMem->tasks[sharedMem->front], 1023);
            taskStr[1023] = '\0';

            sharedMem->front = (sharedMem->front + 1) % QUEUE_SIZE;
            sharedMem->size.fetch_sub(1);
        }

        // Signal empty slot
        sem_post(emptySlotsSemaphore);

        // Execute the task using the member userKey
       int result = executeCryption(taskStr, userKey);
bool success = (result == 0);


        // Call the callback if set
        if (onTaskComplete)
            onTaskComplete(success);
    }
}
bool ProcessManagement::hasPendingTasks() const {
    return sharedMem->size.load() > 0;
}