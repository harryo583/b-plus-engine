# b-plus-engine

**b-plus-engine** is a disk-based key-value store implemented in C++ using a B+ Tree data structure. The store is designed to efficiently handle large datasets that exceed in-memory capacity by utilizing disk storage for both insertion and retrieval of data. The B+ Tree is optimized for reading and writing operations on disk-based storage systems.

## Features

### 1. B+ Tree Data Structure

The project implements a balanced B+ Tree that supports efficient insertion and search operations, making it ideal for disk-based storage systems where performance is crucial.
- **Balanced B+ Tree**: Ensures logarithmic time complexity for operations like insertion and search.
- **Efficient for large datasets**: Optimized to handle datasets that are too large for in-memory storage.

### 2. Disk-Based Storage

The key-value store is designed to store B+ Tree nodes on disk, allowing the system to manage large datasets efficiently without running out of memory.
- **Disk-based nodes**: Tree nodes are written to and read from disk.
- **Scalable storage**: Suitable for handling datasets that exceed the capacity of available memory.

### 3. Page Cache

To enhance performance, the project includes a simple Least Recently Used (LRU) cache. This cache stores recently accessed nodes in memory to reduce disk I/O operations.
- **LRU cache**: Improves performance by caching frequently accessed nodes in memory.
- **Reduced disk I/O**: Minimizes the need to read from disk for frequently accessed data.

## License

This project is licensed under the MIT License.
