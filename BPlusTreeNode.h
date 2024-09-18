#ifndef BPLUSTREENODE_H
#define BPLUSTREENODE_H

#include <vector>
#include <memory>

// Forward declaration of BPlusTreeNode for template usage
template <typename TKey, typename TValue>
class BPlusTreeNode;

template <typename TKey, typename TValue>
class BPlusTreeNode {
public:
    BPlusTreeNode(bool leaf = false);

    bool isLeaf;
    std::vector<TKey> keys;
    std::vector<std::shared_ptr<BPlusTreeNode<TKey, TValue>>> children; // for internal nodes
    std::vector<TValue> values; // for leaf nodes
    std::shared_ptr<BPlusTreeNode<TKey, TValue>> next; // for leaf nodes
};

#include "BPlusTreeNode.cpp" // Include the implementation file

#endif // BPLUSTREENODE_H
