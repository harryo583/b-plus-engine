#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <list>
#include <cstddef> // for size_t

template<typename KeyType, typename ValueType>
class LRUCache {
public:
    explicit LRUCache(size_t capacity); // constructor to initialize the cache with a specified capacity
    ValueType* get(const KeyType& key); // get a value from the cache, return nullptr if key not found
    void put(const KeyType& key, const ValueType& value); // put value into cache with LRU policy
    void clear(); // clear the cache

private:
    size_t capacity;
    std::list<KeyType> order; // doubly linked list to maintain the order of access
    std::unordered_map<KeyType, ValueType> cache; // Hash map to store key-value pairs
    std::unordered_map<KeyType, typename std::list<KeyType>::iterator> iterators; // Hash map to track positions in the linked list
    void moveToHead(const KeyType& key); // Move the accessed key to the head of the list
    void evict(); // Evict the least recently used item
};

// #include "LRUCache.tpp" // Include implementation file if using templates

#endif // LRUCACHE_H
