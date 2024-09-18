#include "DiskManager.h"
#include "BPlusTreeNode.h"
#include <iostream>
#include <vector>

DiskManager::DiskManager(const std::string& filename) : filename(filename) {
    open();
}

void DiskManager::open() {
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        // If file doesn't exist, create it
        file.clear();
        file.open(filename, std::ios::out);
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
}

void DiskManager::close() {
    if (file.is_open()) {
        file.close();
    }
}

template <typename Key, typename Value>
void DiskManager::writeNodeToDisk(const std::shared_ptr<BPlusTreeNode<Key, Value>>& node, int page_id) {
    // Go to the specific position in the file (page_id * page_size)
    file.seekp(page_id * sizeof(BPlusTreeNode<Key, Value>));

    // Write if node is a leaf
    file.write(reinterpret_cast<const char*>(&node->isLeaf()), sizeof(bool));

    // Write keys
    int num_keys = node->getKeys().size();
    file.write(reinterpret_cast<const char*>(&num_keys), sizeof(int));
    for (const auto& key : node->getKeys()) {
        file.write(reinterpret_cast<const char*>(&key), sizeof(Key));
    }

    // Write values if it's a leaf node
    if (node->isLeaf()) {
        for (const auto& value : node->getValues()) {
            file.write(reinterpret_cast<const char*>(&value), sizeof(Value));
        }
    } else {
        // Write children pointers (page ids)
        for (const auto& child : node->getChildren()) {
            int child_id = child->getPageId();
            file.write(reinterpret_cast<const char*>(&child_id), sizeof(int));
        }
    }

    // For leaf nodes, write next pointer
    if (node->isLeaf() && node->getNext()) {
        int next_page_id = node->getNext()->getPageId();
        file.write(reinterpret_cast<const char*>(&next_page_id), sizeof(int));
    }
}

template <typename Key, typename Value>
std::shared_ptr<BPlusTreeNode<Key, Value>> DiskManager::readNodeFromDisk(int page_id) {
    // Go to the specific position in the file (page_id * page_size)
    file.seekg(page_id * sizeof(BPlusTreeNode<Key, Value>));

    bool is_leaf;
    file.read(reinterpret_cast<char*>(&is_leaf), sizeof(bool));

    auto node = std::make_shared<BPlusTreeNode<Key, Value>>(is_leaf);

    // Read keys
    int num_keys;
    file.read(reinterpret_cast<char*>(&num_keys), sizeof(int));
    for (int i = 0; i < num_keys; ++i) {
        Key key;
        file.read(reinterpret_cast<char*>(&key), sizeof(Key));
        node->insertKey(key);
    }

    // Read values if it's a leaf node
    if (is_leaf) {
        for (int i = 0; i < num_keys; ++i) {
            Value value;
            file.read(reinterpret_cast<char*>(&value), sizeof(Value));
            node->insertValue(value);
        }

        // Read next pointer
        int next_page_id;
        file.read(reinterpret_cast<char*>(&next_page_id), sizeof(int));
        if (next_page_id != -1) {
            auto next_node = readNodeFromDisk(next_page_id);
            node->setNext(next_node);
        }
    } else {
        // Read children pointers (page ids)
        for (int i = 0; i < num_keys + 1; ++i) {
            int child_page_id;
            file.read(reinterpret_cast<char*>(&child_page_id), sizeof(int));
            auto child_node = readNodeFromDisk(child_page_id);
            node->addChild(child_node);
        }
    }
    return node;
}