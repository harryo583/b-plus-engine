// This file hasn't been fully implemented yet

#ifndef BPLUSENGINE_H
#define BPLUSENGINE_H

#include <memory>
#include <string>
#include "DiskManager.h"

template <typename Key, typename Value>
class BPlusEngine {
public:
    BPlusEngine(const std::string& filename, int cache_size);
    
    bool insert(const Key& key, const Value& value);
    bool remove(const Key& key);
    bool search(const Key& key, Value& result);
    
    void loadFromDisk();
    void saveToDisk();

private:
    std::shared_ptr<BPlusTreeNode<Key, Value>> root;
    DiskManager diskManager;
    int cacheSize;
    
    // Utility functions
    void splitNode(std::shared_ptr<BPlusTreeNode<Key, Value>> node);
    void mergeNode(std::shared_ptr<BPlusTreeNode<Key, Value>> node);
};

#endif // BPLUSENGINE_H
