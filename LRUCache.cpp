#include "LRUCache.h"
#include <stdexcept>

// Constructor
template<typename KeyType, typename ValueType>
LRUCache<KeyType, ValueType>::LRUCache(size_t capacity) {
    this->capacity = capacity;
    if (capacity == 0) throw std::invalid_argument("Cache capacity must be greater than zero.");
}

// Get a value from the cache
template<typename KeyType, typename ValueType>
ValueType* LRUCache<KeyType, ValueType>::get(const KeyType& key) {
    auto it = cache.find(key);
    if (it == cache.end()) return nullptr; // Key not found
    moveToHead(key);
    return &it->second;
}

// Put a value into the cache
template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::put(const KeyType& key, const ValueType& value) {
    auto it = cache.find(key);
    if (it != cache.end()) {
        // Update existing value
        it->second = value;
        moveToHead(key);
    } else {
        if (cache.size() >= capacity) evict();
        // Insert new value
        cache[key] = value;
        order.push_front(key);
        iterators[key] = order.begin();
    }
}

// Clear the cache
template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::clear() {
    cache.clear();
    order.clear();
    iterators.clear();
}

// Move the accessed key to the head of the list
template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::moveToHead(const KeyType& key) {
    auto it = iterators.find(key);
    if (it != iterators.end()) {
        order.erase(it->second); // remove from current position
        order.push_front(key); // add to the front
        it->second = order.begin(); // update iterator
    }
}

// Evict the least recently used item
template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::evict() {
    if (order.empty()) throw std::runtime_error("Cannot evict from an empty cache.");
    KeyType lruKey = order.back();
    order.pop_back();
    cache.erase(lruKey);
    iterators.erase(lruKey);
}

// Explicit template instantiation
template class LRUCache<int, std::string>;