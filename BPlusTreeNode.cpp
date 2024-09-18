#include "BPlusTreeNode.h"

// BPlusTreeNode constructor
template <typename TKey, typename TValue>
BPlusTreeNode<TKey, TValue>::BPlusTreeNode(bool leaf)
    : isLeaf(leaf), next(nullptr) {}

template class BPlusTreeNode<int, int>;