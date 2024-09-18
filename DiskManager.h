#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <string>
#include <fstream>

class DiskManager {
public:
    DiskManager(const std::string& filename);
    
    template <typename Key, typename Value>
    void writeNodeToDisk(const std::shared_ptr<BPlusTreeNode<Key, Value>>& node, int page_id);

    template <typename Key, typename Value>
    std::shared_ptr<BPlusTreeNode<Key, Value>> readNodeFromDisk(int page_id);

    void open();
    void close();

private:
    std::string filename;
    std::fstream file;
};

#endif // DISKMANAGER_H
