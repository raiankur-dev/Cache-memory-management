#include <iostream>
#include <unordered_map>
#include <ctime>

using namespace std;

struct CacheEntry {
    string key;
    string value;
    time_t timestamp;
    CacheEntry* left;
    CacheEntry* right;
};

class SplayTreeCache {
private:
    int capacity;
    CacheEntry* root;
    unordered_map<string, CacheEntry*> cacheMap;

    CacheEntry* newNode(string key, string value) {
        CacheEntry* node = new CacheEntry();
        node->key = key;
        node->value = value;
        node->timestamp = time(nullptr);
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }

    CacheEntry* rightRotate(CacheEntry* x) {
        CacheEntry* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    CacheEntry* leftRotate(CacheEntry* x) {
        CacheEntry* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    CacheEntry* splay(CacheEntry* node, string key) {
        if (node == nullptr || node->key == key) {
            return node;
        }

        if (key < node->key) {
            if (node->left == nullptr) {
                return node;
            }

            if (key < node->left->key) {
                node->left->left = splay(node->left->left, key);
                node = rightRotate(node);
            }
            else if (key > node->left->key) {
                node->left->right = splay(node->left->right, key);
                if (node->left->right != nullptr) {
                    node->left = leftRotate(node->left);
                }
            }

            if (node->left == nullptr) {
                return node;
            }
            else {
                return rightRotate(node);
            }
        }
        else {
            if (node->right == nullptr) {
                return node;
            }

            if (key < node->right->key) {
                node->right->left = splay(node->right->left, key);
                if (node->right->left != nullptr) {
                    node->right = rightRotate(node->right);
                }
            }
            else if (key > node->right->key) {
                node->right->right = splay(node->right->right, key);
                node = leftRotate(node);
            }

            if (node->right == nullptr) {
                return node;
            }
            else {
                return leftRotate(node);
            }
        }
    }

    CacheEntry* find(CacheEntry* node, string key) {
        if (node == nullptr || node->key == key) {
            return node;
        }

        if (key < node->key) {
            if (node->left == nullptr) {
                return node;
            }
            return find(node->left, key);
        }
        else {
            if (node->right == nullptr) {
                return node;
            }
            return find(node->right, key);
        }
    }

    CacheEntry* insert(CacheEntry* node, string key, string value) {
        if (node == nullptr) {
            return newNode(key, value);
        }

        node = splay(node, key);

        if (node->key == key) {
            // Update existing entry
            node->value = value;
            node->timestamp = time(nullptr);
            return node;
        }

        CacheEntry* newNodePtr = newNode(key, value);

        if (key < node->key) {
            newNodePtr->right = node;
            newNodePtr->left = node->left;
            node->left = nullptr;
        }
        else {
            newNodePtr->left = node;
            newNodePtr->right = node->right;
            node->right = nullptr;
        }

        return newNodePtr;
    }

    CacheEntry* erase(CacheEntry* node, string key) {
        if (node == nullptr) {
            return nullptr;
        }

        node = splay(node, key);

        if (node->key != key) {
            return node;
        }

        if (node->left == nullptr) {
            root = node->right;
        }
        else {
            CacheEntry* maxLeft = findMax(node->left);
            maxLeft->right = node->right;
            root = maxLeft;
        }

        delete node;
        cacheMap.erase(key);

        return root;
    }

    CacheEntry* findMax(CacheEntry* node) {
        if (node == nullptr) {
            return nullptr;
        }

        while (node->right != nullptr) {
            node = node->right;
        }

        return node;
    }

public:
    SplayTreeCache(int capacity) {
        this->capacity = capacity;
        root = nullptr;
    }

    string get(string key) {
        CacheEntry* entry = find(root, key);
        if (entry != nullptr && entry->key == key) {
            entry->timestamp = time(nullptr); // Update the timestamp to mark it as recently used
            root = splay(root, key);
            return entry->value;
        }
        return "";
    }

    void put(string key, string value) {
        CacheEntry* existingEntry = find(root, key);
        if (existingEntry != nullptr && existingEntry->key == key) {
            existingEntry->value = value;
            existingEntry->timestamp = time(nullptr);
            root = splay(root, key);
            return;
        }

        if (cacheMap.size() >= capacity) {
            // Find the least recently used entry by traversing the entire tree
            CacheEntry* lruEntry = findLeastRecentlyUsed(root);
            root = erase(root, lruEntry->key);
        }

        root = insert(root, key, value);
        cacheMap[key] = root;
    }

    void remove(string key) {
        root = erase(root, key);
    }

    void printCache() {
        cout << "Cache Contents:" << endl;
        printCacheHelper(root);
        cout << endl;
    }

private:
    void printCacheHelper(CacheEntry* node) {
        if (node == nullptr) {
            return;
        }

        printCacheHelper(node->left);
        cout << "Key: " << node->key << ", Value: " << node->value << endl;
        printCacheHelper(node->right);
    }

    CacheEntry* findLeastRecentlyUsed(CacheEntry* node) {
        if (node == nullptr) {
            return nullptr;
        }

        CacheEntry* lruEntry = nullptr;
        time_t lruTime = time(nullptr);

        traverseTree(node, lruEntry, lruTime);

        return lruEntry;
    }

    void traverseTree(CacheEntry* node, CacheEntry*& lruEntry, time_t& lruTime) {
        if (node == nullptr) {
            return;
        }

        traverseTree(node->left, lruEntry, lruTime);

        if (node->timestamp < lruTime) {
            lruEntry = node;
            lruTime = node->timestamp;
        }

        traverseTree(node->right, lruEntry, lruTime);
    }
};

int main() {
    int capacity;
    cout << "Enter the capacity of the cache: ";
    cin >> capacity;

    SplayTreeCache cache(capacity);

    char choice;
    do {
        cout << "\nSelect an option:\n";
        cout << "1. Add an entry\n";
        cout << "2. Retrieve an entry\n";
        cout << "3. Remove an entry\n";
        cout << "4. Print cache contents\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1': {
                string key, value;
                cout << "Enter key: ";
                cin >> key;
                cout << "Enter value: ";
                cin >> value;
                cache.put(key, value);
                break;
            }
            case '2': {
                string key;
                cout << "Enter key: ";
                cin >> key;
                string value = cache.get(key);
                if (!value.empty()) {
                    cout << "Value: " << value << endl;
                } else {
                    cout << "Entry not found in cache." << endl;
                }
                break;
            }
            case '3': {
                string key;
                cout << "Enter key: ";
                cin >> key;
                cache.remove(key);
                cout << "Entry removed from cache." << endl;
                break;
            }
            case '4':
                cache.printCache();
                break;
            case '5':
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

    } while (choice != '5');

    return 0;
}
