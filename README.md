# b-tree-engine
This project implements a disk-based key-value store using a B+ Tree data structure in C++. The store is designed for efficient storage and retrieval of data, utilizing disk storage to handle large datasets that exceed memory capacity. The B+ Tree is a balanced tree data structure optimized for reading and writing operations on disk-based storage systems.

# Features
B+ Tree Data Structure: implements a balanced B+ Tree with support for insertion and search operations.
Disk-Based Storage: nodes are stored on disk, enabling the handling of large datasets.
Page Cache: uses a simple Least Recently Used (LRU) cache to store recently accessed nodes in memory for improved performance.
