// BPlusTree.h

#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <fstream>
#include <optional>

// B+ tree order
const int ORDER = 4;

// BPlusTree class forward declaration
template <typename TKey, typename TValue>
class BPlusTree;

// BPlusTreeNode class
template <typename TKey, typename TValue>
class BPlusTreeNode {
public:

    BPlusTreeNode(bool leaf = false);

    bool isLeaf;
    std::vector<TKey> keys;
    std::vector<std::shared_ptr<BPlusTreeNode>> children; // for internal nodes
    std::vector<TValue> values; // for leaf nodes
    std::shared_ptr<BPlusTreeNode> next; // for leaf nodes
};

// BPlusTreeNode constructor
template <typename TKey, typename TValue>
BPlusTreeNode<TKey, TValue>::BPlusTreeNode(bool leaf) 
    : isLeaf(leaf), next(nullptr) {}

// BPlusTree class
template <typename TKey, typename TValue>
class BPlusTree {
public:
    BPlusTree();
    ~BPlusTree() = default; // default destructor

    void insert(const TKey& key, const TValue& value);
    std::optional<TValue> search(const TKey& key) const;
    bool remove(const TKey& key);
    void display() const;
    std::optional<TValue> findMin() const;
    std::optional<TValue> findMax() const;
    std::vector<TValue> rangeSearch(const TKey& lowerBound, const TKey& upperBound) const;

private:
    std::shared_ptr<BPlusTreeNode<TKey, TValue>> root; // shared pointer to the root

    void insertInternal(const TKey& key, std::shared_ptr<BPlusTreeNode<TKey, TValue>> node, std::shared_ptr<BPlusTreeNode<TKey, TValue>> child);
    void splitLeaf(std::shared_ptr<BPlusTreeNode<TKey, TValue>> leaf);
    std::shared_ptr<BPlusTreeNode<TKey, TValue>> getParent(std::shared_ptr<BPlusTreeNode<TKey, TValue>> current, std::shared_ptr<BPlusTreeNode<TKey, TValue>> child);
    void splitInternal(std::shared_ptr<BPlusTreeNode<TKey, TValue>> internal);
};

#endif // BPLUSTREE_H