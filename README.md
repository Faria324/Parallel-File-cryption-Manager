Parallel File Encryption / Decryption Manager
Team Members

Faria Fatima

Aqsa Rehman

Muhammad Hasnat Fakhar

Project Overview

This project is a parallel file encryption and decryption manager developed as part of an Operating Systems course. The application is designed to handle multiple files simultaneously by leveraging a dynamic thread pool, thereby enhancing performance while maintaining data integrity through robust synchronization mechanisms.

The system is implemented in C++ on a Linux environment (WSL) and includes both a GTK-based graphical user interface and a command-line interface, offering a practical demonstration of how fundamental operating system concepts are applied in real-world concurrent applications.

Operating System Concepts Implemented

Multithreading using std::thread

Thread pool architecture

Producer–Consumer synchronization model

Mutex locks for mutual exclusion

Counting semaphores

Shared memory using POSIX (shm_open, mmap)

Atomic operations

Race condition avoidance

Deadlock and starvation prevention

Clean and graceful termination of threads

Technologies Used

C++

POSIX threads and semaphores

GTK for graphical user interface

Linux / Windows Subsystem for Linux (WSL)

Build and Execution Instructions
Build the Project
make

Run the GUI Version
./encrypt_decrypt_gui

Run the Console Version
./encrypt_decrypt
