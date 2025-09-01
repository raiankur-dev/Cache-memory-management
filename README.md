Cache Memory Management using Splay Trees
📌 Overview

This project implements a cache memory manager using Splay Trees to simulate how operating systems optimize memory access. It demonstrates efficient cache operations, eviction policies, and concurrency handling, making it relevant for both software systems and embedded applications.

✨ Features

OS-level Simulation: Models how cache memory improves data access speed in constrained environments.

LRU Eviction: Implements a Least Recently Used (LRU) policy to manage limited cache capacity.

Splay Tree Optimization: Uses self-adjusting binary search trees for faster lookups, insertions, and deletions.

Thread-Safe Design: Employs multithreading, mutex locks, and condition variables to ensure correctness under concurrent access.

🛠️ Technologies Used

C++ (Data structures, Multithreading)

Splay Trees (Self-adjusting search trees)

Mutex & Condition Variables (Concurrency control)

🚀 How to Run

Clone the repository:

git clone https://github.com/yourusername/cache-memory-management.git
cd cache-memory-management


Compile and run the single-threaded version:

g++ Memory\ amangemnt\ using\ splay\ tress.cpp -o cache
./cache


Compile and run the multi-threaded version:

g++ CMM_Multithread_mutex.cpp -o cache_mt -pthread
./cache_mt

📊 Example Usage
Single-threaded
Enter the capacity of the cache: 3
1. Add an entry
2. Retrieve an entry
3. Remove an entry
4. Print cache contents
5. Exit

Multi-threaded

Thread 1 inserts multiple key-value pairs.

Thread 2 inserts additional pairs concurrently.

The final cache contents are printed after synchronization.

🎯 Learning Outcomes

Understanding cache design in operating systems.

Applying LRU eviction and splay trees to optimize access time.

Implementing safe multithreading for concurrent memory operations.
