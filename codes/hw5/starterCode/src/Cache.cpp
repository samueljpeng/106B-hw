#define CacheCPP_Included
#include "Cache.h"
#include "iostream"

template <typename ValueType>
Cache<ValueType>::Cache(int maxSize) {
    root = last = nullptr;
    cur_Size = 0;
    max_Size = maxSize;
    (void) maxSize;
}

template <typename ValueType>
Cache<ValueType>::~Cache() {
    while(root != nullptr) {
        Cache::Node *trash = root;
        root = root -> next;
        delete trash;
    }
}

template <typename ValueType>
void Cache<ValueType>::put(const std::string& key, const ValueType& value) {
    if(hash.containsKey(key)) {
        if(hash[key] == root) {
            root = root->next;
            root->prev = nullptr;
            hash[key]->next = nullptr;
            hash[key]->prev = last;
            last->next = hash[key];
            last = hash[key];
            return;
        } else if (hash[key] == last) {
            return;
        } else {
            hash[key]->prev->next = hash[key]->next;
            hash[key]->next->prev = hash[key]->prev;
            hash[key]->next = nullptr;
            last->next = hash[key];
            last = hash[key];
            return;
        }
    }
    /*Cache::Node *temp = root;
    while (temp != nullptr) {
        if (temp->key == key) {
            if (temp == root) {
                root = root->next;
                root->prev = nullptr;
                temp->next = nullptr;
                temp->prev = last;
                last->next = temp;
                last = temp;
                return;
            } else if (temp == last) {
                return;
            } else {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                temp->next = nullptr;
                temp->prev = last;
                last->next = temp;
                last = temp;
                return;
            }
        }
        temp = temp->next;
    }
    */
    if (cur_Size >= max_Size) {
        Cache::Node *trash = root;
        root = root->next;
        root->prev = nullptr;
        hash.remove(trash->key);
        delete trash;
    }
    Cache::Node *newNode = new Cache::Node;
    newNode->key = key;
    newNode->value = value;
    newNode->prev = last;
    newNode->next = nullptr;
    if (last != nullptr) last->next = newNode;
    else root = newNode;
    last = newNode;
    hash.add(key, newNode);
    cur_Size ++;
    (void) key;
    (void) value;
}

template <typename ValueType>
bool Cache<ValueType>::containsKey(const std::string& key) {
    return hash.containsKey(key);
    (void) key;
    return false;
}

template <typename ValueType>
ValueType Cache<ValueType>::get(const std::string& key) {
    if(!containsKey(key)) {
        error("Key DNE!");
    } else {
        if(hash[key] == root) {
            root = root->next;
            root->prev = nullptr;
            hash[key]->next = nullptr;
            hash[key]->prev = last;
            last->next = hash[key];
            last = hash[key];
            return last->value;
        } else if (hash[key] == last) {
            return last->value;
        } else {
            hash[key]->prev->next = hash[key]->next;
            hash[key]->next->prev = hash[key]->prev;
            hash[key]->next = nullptr;
            last->next = hash[key];
            last = hash[key];
            return last->value;
        }
    }

    (void) key;
    return {};
}
