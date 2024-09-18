#include <iostream>
#include <cassert>
#include <optional>
#include <vector>
#include <string>
#include "BPlusTree.h"

void testInsertionAndSearch() {
    BPlusTree<int, std::string> tree;
    
    tree.insert(10, "Value10");
    tree.insert(20, "Value20");
    tree.insert(30, "Value30");

    assert(tree.search(10) == "Value10");
    assert(tree.search(20) == "Value20");
    assert(tree.search(30) == "Value30");
    assert(tree.search(40) == std::nullopt); // Key not found
}


void testLeafNodeSplitting() {
    BPlusTree<int, std::string> tree;

    // Insert values to cause a split
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i, "Value" + std::to_string(i));
    }

    // Verify values
    assert(tree.search(1) == "Value1");
    assert(tree.search(10) == "Value10");
    assert(tree.search(5) == "Value5");
    assert(tree.search(6) == "Value6");

    // Check non-existing keys
    assert(tree.search(15) == std::nullopt);
}


void testInternalNodeSplitting() {
    BPlusTree<int, std::string> tree;

    // Insert values to cause internal node splits
    for (int i = 1; i <= 20; ++i) {
        tree.insert(i, "Value" + std::to_string(i));
    }

    // Verify values
    for (int i = 1; i <= 20; ++i) {
        assert(tree.search(i) == "Value" + std::to_string(i));
    }
}


void testDeletion() {
    BPlusTree<int, std::string> tree;

    // Insert values
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i, "Value" + std::to_string(i));
    }

    // Delete some values
    assert(tree.remove(5));
    assert(tree.remove(10));
    assert(!tree.remove(20)); // Key not found

    // Verify values
    assert(tree.search(5) == std::nullopt);
    assert(tree.search(10) == std::nullopt);
    for (int i = 1; i <= 9; ++i) {
        if (i != 5 && i != 10) {
            assert(tree.search(i) == "Value" + std::to_string(i));
        }
    }
}


void testRangeSearch() {
    BPlusTree<int, std::string> tree;

    // Insert values
    for (int i = 1; i <= 15; ++i) {
        tree.insert(i, "Value" + std::to_string(i));
    }

    auto result = tree.rangeSearch(5, 10);
    std::vector<std::string> expected = {"Value5", "Value6", "Value7", "Value8", "Value9", "Value10"};
    assert(result == expected);

    result = tree.rangeSearch(0, 5);
    expected = {"Value1", "Value2", "Value3", "Value4", "Value5"};
    assert(result == expected);

    result = tree.rangeSearch(11, 15);
    expected = {"Value11", "Value12", "Value13", "Value14", "Value15"};
    assert(result == expected);
}


void testDisplay() {
    BPlusTree<int, std::string> tree;

    // Insert values
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i, "Value" + std::to_string(i));
    }

    // Display the tree structure (You might need to visually inspect the output)
    tree.display();
}


int main() {
    testInsertionAndSearch();
    testLeafNodeSplitting();
    testInternalNodeSplitting();
    testDeletion();
    testRangeSearch();
    testDisplay();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

