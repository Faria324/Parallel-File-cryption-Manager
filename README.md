# Parallel File Encryption / Decryption Manager

## Team Members
- Faria Fatima  
- Aqsa Rehman  
- Muhammad Hasnat Fakhar  

## Project Overview
This project is a **parallel file encryption and decryption manager** developed as part of an **Operating Systems course**. The application is designed to process **multiple files concurrently** using a **dynamic thread pool**, improving performance while ensuring data integrity through proper synchronization mechanisms.

The system is implemented in **C++ on a Linux environment (WSL)** and provides both a **GTK-based graphical user interface** and a **command-line interface**, demonstrating real-world application of core operating system concepts in concurrent systems.

## Operating System Concepts Implemented
- Multithreading using `std::thread`
- Thread pool architecture
- Producer–Consumer synchronization model
- Mutex locks for mutual exclusion
- Counting semaphores
- Shared memory using POSIX (`shm_open`, `mmap`)
- Atomic operations
- Race condition avoidance
- Deadlock and starvation prevention
- Clean and graceful termination of threads

## Technologies Used
- C++
- POSIX threads and semaphores
- GTK for graphical user interface
- Linux / Windows Subsystem for Linux (WSL)

## Build and Execution Instructions

### Build the Project
```bash
make
Run GUI Version
./encrypt_decrypt_gui

Run Console Version
./encrypt_decrypt
