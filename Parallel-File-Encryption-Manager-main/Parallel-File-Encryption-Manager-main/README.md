# Parallel File Encryption / Decryption Manager

## Team Members
- Aqsa Rehman  
- Faria Fatima  
- Muhammad Hasnat Fakhar  

---

## About the Project
This project is a **multithreaded file encryption and decryption system** developed as part of an
**Operating Systems course**. It processes multiple files concurrently using a **dynamic thread pool**
to improve performance while ensuring correctness through proper synchronization.

The system is implemented in **C++** on **Linux (WSL)** and provides both a **GTK-based GUI**
and a **console-based version**, demonstrating real-world usage of core OS concepts.

---

## Operating System Concepts Used
- Multithreading (`std::thread`)
- Thread Pool design
- Producer–Consumer problem
- Mutex locks (mutual exclusion)
- Counting semaphores
- Shared memory (POSIX `shm_open`, `mmap`)
- Atomic operations
- Race condition prevention
- Deadlock and starvation avoidance
- Graceful thread termination

---

## Technologies Used
- C++
- POSIX threads & semaphores
- GTK (GUI)
- Linux / WSL

---

## How to Build and Run

### Build the Project
```bash
make.

Run GUI Version
./encrypt_decrypt_gui

Run Console Version
./encrypt_decrypt