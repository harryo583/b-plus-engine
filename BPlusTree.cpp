#include "BPlusTree.h"

// Constructor definition
template <typename TKey, typename TValue>
BPlusTree<TKey, TValue>::BPlusTree() {
    root = std::make_shared<BPlusTreeNode<TKey, TValue>>(true);
}

// Insert method definition
template <typename TKey, typename TValue>
void BPlusTree<TKey, TValue>::insert(const TKey& key, const TValue& value) {
    // Split root if full
    if (root->keys.size() == 2 * ORDER - 1) {
        auto newRoot = std::make_shared<BPlusTreeNode<TKey, TValue>>(); // Declare pointer newRoot
        newRoot->children.push_back(root); // New root has the old root as first child
        splitInternal(newRoot); // Split the full root
        root = newRoot; // Update the root to the new root
    }

    // Traverse the tree to find the appropriate leaf node
    auto leaf = root;
    while (!leaf->isLeaf) {
        int i = 0;
        while (i < leaf->keys.size() && key >= leaf->keys[i]) i++;
        leaf = std::dynamic_pointer_cast<BPlusTreeNode<TKey, TValue>>(leaf->children[i]);
    }

    // Insert the key-value pair into the found leaf node
    int i = 0;
    while (i < leaf->keys.size() && key >= leaf->keys[i]) i++;
    leaf->keys.insert(leaf->keys.begin() + i, key);
    leaf->values.insert(leaf->values.begin() + i, value);

    // Split leaf if full
    if (leaf->keys.size() == 2 * ORDER - 1) {
        splitLeaf(leaf);
    }
}

// Search method definition
template <typename TKey, typename TValue>
std::optional<TValue> BPlusTree<TKey, TValue>::search(const TKey& key) const {
    // Traverse to the leaf that contains the key
    auto current = root;
    while (!current->isLeaf) {
        int i = 0;
        while (i < current->keys.size() && key >= current->keys[i]) i++;
        current = std::dynamic_pointer_cast<BPlusTreeNode<TKey, TValue>>(current->children[i]);
    }

    // Binary search in leaf
    int left = 0, right = current->keys.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        TKey midKey = current->keys[mid];
        if (midKey == key) {
            return current->values[mid];
        } else if (midKey < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return std::nullopt; // Key not found
}

// Remove method definition
template <typename TKey, typename TValue>
bool BPlusTree<TKey, TValue>::remove(const TKey& key) {
    // Traverse to the leaf that might contain the key
    auto current = root;
    while (!current->isLeaf) {
        int i = 0;
        while (i < current->keys.size() && key >= current->keys[i]) i++;
        current = std::dynamic_pointer_cast<BPlusTreeNode<TKey, TValue>>(current->children[i]);
    }

    // Search for the key
    int i = 0;
    while (i < current->keys.size() && current->keys[i] != key) i++;

    if (i == current->keys.size()) return false; // Key not found

    current->keys.erase(current->keys.begin() + i);
    current->values.erase(current->values.begin() + i);

    // Handle underflow
    while (current != root && current->keys.size() < ORDER - 1) {
        auto parent = getParent(root, current);

        if (parent) {
            // Borrow from sibling or merge with sibling
            int pos = 0;
            while (pos < parent->children.size() && parent->children[pos] != current) pos++;

            if (pos > 0 && parent->children[pos - 1]->keys.size() > ORDER - 1) {
                // Borrow from left sibling
                auto leftSibling = parent->children[pos - 1];
                current->keys.insert(current->keys.begin(), parent->keys[pos - 1]);
                parent->keys[pos - 1] = leftSibling->keys.back();
                leftSibling->keys.pop_back();

                if (!leftSibling->isLeaf) {
                    current->children.insert(current->children.begin(), leftSibling->children.back());
                    leftSibling->children.pop_back();
                }
            } else if (pos < parent->children.size() - 1 && parent->children[pos + 1]->keys.size() > ORDER - 1) {
                // Borrow from right sibling
                auto rightSibling = parent->children[pos + 1];
                current->keys.push_back(parent->keys[pos]);
                parent->keys[pos] = rightSibling->keys.front();
                rightSibling->keys.erase(rightSibling->keys.begin());

                if (!rightSibling->isLeaf) {
                    current->children.push_back(rightSibling->children.front());
                    rightSibling->children.erase(rightSibling->children.begin());
                }
            } else {
                // Merge with sibling
                if (pos > 0) {
                    // Merge with left sibling
                    auto leftSibling = parent->children[pos - 1];
                    leftSibling->keys.push_back(parent->keys[pos - 1]);
                    leftSibling->keys.insert(leftSibling->keys.end(), current->keys.begin(), current->keys.end());
                    leftSibling->values.insert(leftSibling->values.end(), current->values.begin(), current->values.end());

                    if (!current->isLeaf) {
                        leftSibling->children.insert(leftSibling->children.end(), current->children.begin(), current->children.end());
                    }

                    parent->keys.erase(parent->keys.begin() + pos - 1);
                    parent->children.erase(parent->children.begin() + pos);
                } else {
                    // Merge with right sibling
                    auto rightSibling = parent->children[pos + 1];
                    current->keys.push_back(parent->keys[pos]);
                    current->keys.insert(current->keys.end(), rightSibling->keys.begin(), rightSibling->keys.end());
                    current->values.insert(current->values.end(), rightSibling->values.begin(), rightSibling->values.end());

                    if (!rightSibling->isLeaf) {
                        current->children.insert(current->children.end(), rightSibling->children.begin(), rightSibling->children.end());
                    }

                    parent->keys.erase(parent->keys.begin() + pos);
                    parent->children.erase(parent->children.begin() + pos + 1);
                }

                current = parent;
            }
        } else {
            break;
        }
    }

    return true;
}

// In-order traversal display method definition
template <typename TKey, typename TValue>
void BPlusTree<TKey, TValue>::display() const {
    std::function<void(std::shared_ptr<BPlusTreeNode<TKey, TValue>>, int)> printNode;
    printNode = [&](std::shared_ptr<BPlusTreeNode<TKey, TValue>> node, int level) {
        if (node == nullptr) return;
        std::cout << "Level " << level << ": ";
        for (const auto& key : node->keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;

        if (!node->isLeaf) {
            for (const auto& child : node->children) {
                printNode(child, level + 1);
            }
        }
    };
    printNode(root, 0);
}

// findMin method definition
template <typename TKey, typename TValue>
std::optional<TValue> BPlusTree<TKey, TValue>::findMin() const {
    auto current = root;
    while (!current->isLeaf) {
        current = std::dynamic_pointer_cast<BPlusTreeNode<TKey, TValue>>(current->children[0]);
    }

    if (!current->values.empty()) {
        return current->values[0];
    }

    return std::nullopt;
}

// findMax method definition
template <typename TKey, typename TValue>
std::optional<TValue> BPlusTree<TKey, TValue>::findMax() const {
    auto current = root;
    while (!current->isLeaf) {
        current = std::dynamic_pointer_cast<BPlusTreeNode<TKey, TValue>>(current->children[current->children.size() - 1]);
    }

    if (!current->values.empty()) {
        return current->values[current->values.size() - 1];
    }

    return std::nullopt;
}

// rangeSearch method definition
template <typename TKey, typename TValue>
std::vector<TValue> BPlusTree<TKey, TValue>::rangeSearch(const TKey& lowerBound, const TKey& upperBound) const {
    std::vector<TValue> result;
    auto current = root;
    while (!current->isLeaf) {
        int i = 0;
        while (i < current->keys.size() && lowerBound >= current->keys[i]) i++;
        current = std::dynamic_pointer_cast<BPlusTreeNode<TKey, TValue>>(current->children[i]);
    }

    // Collect values in the range
    while (current) {
        for (size_t i = 0; i < current->keys.size(); ++i) {
            if (current->keys[i] >= lowerBound && current->keys[i] <= upperBound) {
                result.push_back(current->values[i]);
            }
        }
        current = current->next;
    }

    return result;
}

// Private splitLeaf method definition
template <typename TKey, typename TValue>
void BPlusTree<TKey, TValue>::splitLeaf(std::shared_ptr<BPlusTreeNode<TKey, TValue>> leaf) {
    auto newLeaf = std::make_shared<BPlusTreeNode<TKey, TValue>>(true);
    int mid = (2 * ORDER - 1) / 2;

    // Move half the keys and values to the new leaf
    newLeaf->keys.assign(leaf->keys.begin() + mid, leaf->keys.end());
    newLeaf->values.assign(leaf->values.begin() + mid, leaf->values.end());

    // Adjust current leaf
    leaf->keys.resize(mid);
    leaf->values.resize(mid);

    // Link the new leaf
    newLeaf->next = leaf->next;
    leaf->next = newLeaf;

    // Insert new leaf to the parent
    insertInternal(newLeaf->keys[0], leaf, newLeaf);
};

// Private splitInternal method definition
template <typename TKey, typename TValue>
void BPlusTree<TKey, TValue>::splitInternal(std::shared_ptr<BPlusTreeNode<TKey, TValue>> internal) {
    auto child = internal->children.back(); // last child of the internal node
    if (child->isLeaf) {
        splitLeaf(child); // split it as a leaf node if the child is a leaf
        return;
    }

    // Not a child; split as internal node
    auto newInternal = std::make_shared<BPlusTreeNode<TKey, TValue>>();
    int mid = (2 * ORDER - 1) / 2;

    // Move second half of the keys to the new internal node
    newInternal->keys.assign(child->keys.begin() + mid + 1, child->keys.end());
    TKey upKey = child->keys[mid];

    // Move second half of the children to the new internal node
    newInternal->children.assign(child->children.begin() + mid + 1, child->children.end());

    // Adjust the last child
    child->keys.resize(mid);
    child->children.resize(mid + 1);

    // Insert the new upKey and the newInternal
    internal->keys.push_back(upKey);
    internal->children.push_back(newInternal);

    // If the internal node (parent) is now overfull (has 2*ORDER-1 keys), split it too
    if (internal->keys.size() == 2 * ORDER - 1) {
        if (internal == root) {
            auto newRoot = std::make_shared<BPlusTreeNode<TKey, TValue>>();
            newRoot->children.push_back(root);
            splitInternal(newRoot);
            root = newRoot;
        } else {
            // Recursively split the parent
            auto parent = getParent(root, internal);
            insertInternal(upKey, parent, newInternal);
        }
    }
};


// Private getParent method definition
template <typename TKey, typename TValue>
std::shared_ptr<BPlusTreeNode<TKey, TValue>> BPlusTree<TKey, TValue>::getParent(std::shared_ptr<BPlusTreeNode<TKey, TValue>> current, std::shared_ptr<BPlusTreeNode<TKey, TValue>> child) {

    if (current->isLeaf || current == root) return nullptr;

    // Traverse the current node's children to find the parent of 'child'
    for (auto& c : current->children) {
        if (c == child) {
            return current;
        } else if (!c->isLeaf) {
            auto parent = getParent(c, child);
            if (parent) return parent;
        }
    }

    return nullptr;
};


// Private insertInternal method definition
template <typename TKey, typename TValue>
void BPlusTree<TKey, TValue>::insertInternal(const TKey& key, std::shared_ptr<BPlusTreeNode<TKey, TValue>> node, std::shared_ptr<BPlusTreeNode<TKey, TValue>> child) {
    
    // Find the correct position in the internal node to insert the new key
    int pos = 0;
    while (pos < node->keys.size() && key > node->keys[pos]) pos++;

    // Insert the key at the found position
    node->keys.insert(node->keys.begin() + pos, key);
    node->children.insert(node->children.begin() + pos + 1, child);

    // Split if overfull
    if (node->keys.size() == 2 * ORDER - 1) {
        if (node == root) {
            auto newRoot = std::make_shared<BPlusTreeNode<TKey, TValue>>();
            newRoot->children.push_back(root);
            splitInternal(newRoot);
            root = newRoot;
        } else {
            // Recursively split the node's parent if it's not the root
            auto parent = getParent(root, node);
            splitInternal(parent);
        }
    }
};

// Explicit template instantiation
template class BPlusTree<int, std::string>;