#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <fstream>
#include "BPlusTreeNode.h"

class DiskManager {
public:
    // Constructor to initialize the DiskManager with a file name
    DiskManager(const std::string& filename);

    // Destructor to close the file
    ~DiskManager();

    // Write a node to disk
    void writeNodeToDisk(const BPlusTreeNode<int, std::string>& node, std::streampos position);

    // Read a node from disk
    BPlusTreeNode<int, std::string> readNodeFromDisk(std::streampos position);

    // Optional: method to flush all pending writes to disk
    void flush();

private:
    // File stream for reading and writing nodes
    std::fstream file;

    // File name for the disk storage
    std::string filename;

    // Optional: buffer for pending writes
    // std::unordered_map<std::streampos, BPlusTreeNode<int, std::string>> buffer;
};

#endif // DISKMANAGER_H
