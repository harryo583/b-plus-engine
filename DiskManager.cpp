#include "DiskManager.h"
#include <iostream>

DiskManager::DiskManager(const std::string& filename) : filename(filename) {
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        file.open(filename, std::ios::out | std::ios::binary);
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
}

DiskManager::~DiskManager() {
    if (file.is_open()) {
        file.close();
    }
}

void DiskManager::writeNodeToDisk(const BPlusTreeNode<int, std::string>& node, std::streampos position) {
    file.seekp(position);
    // Serialize the node (code needs adjustments)
    size_t keyCount = node.keys.size();
    file.write(reinterpret_cast<const char*>(&keyCount), sizeof(keyCount));
    for (const auto& key : node.keys) {
        file.write(reinterpret_cast<const char*>(&key), sizeof(key));
    }
    size_t valueCount = node.values.size();
    file.write(reinterpret_cast<const char*>(&valueCount), sizeof(valueCount));
    for (const auto& value : node.values) {
        size_t valueSize = value.size();
        file.write(reinterpret_cast<const char*>(&valueSize), sizeof(valueSize));
        file.write(value.data(), valueSize);
    }
    // Add logic to serialize children and next pointer
}

BPlusTreeNode<int, std::string> DiskManager::readNodeFromDisk(std::streampos position) {
    file.seekg(position);
    BPlusTreeNode<int, std::string> node;
    // Deserialize the node
    size_t keyCount;
    file.read(reinterpret_cast<char*>(&keyCount), sizeof(keyCount));
    node.keys.resize(keyCount);
    for (auto& key : node.keys) {
        file.read(reinterpret_cast<char*>(&key), sizeof(key));
    }
    size_t valueCount;
    file.read(reinterpret_cast<char*>(&valueCount), sizeof(valueCount));
    node.values.resize(valueCount);
    for (auto& value : node.values) {
        size_t valueSize;
        file.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));
        value.resize(valueSize);
        file.read(&value[0], valueSize);
    }
    // Add logic to deserialize children and next pointer if necessary
    return node;
}

void DiskManager::flush() {
    file.flush();
}