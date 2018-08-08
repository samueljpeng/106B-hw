/*
 * CS106B Assignment 5-B
 * Cache.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/08/2018
 *
 * Basic feature:
 * A type representing a cache.
 */

#define CacheCPP_Included
#include "Cache.h"
#include <iostream>

template <typename ValueType>
Cache<ValueType>::Cache(int maxSize) {
    root = last = nullptr;
    cur_Size = 0;
    max_Size = maxSize;
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
        //The item already exists in Cache.
        //Just move the item to the back.
        if(hash[key] == root) {
            //The item is the first item in the double-linked list.
            root = root->next;
            root->prev = nullptr;
            hash[key]->next = nullptr;
            hash[key]->prev = last;
            last->next = hash[key];
            hash[key]->prev = last;
            last = hash[key];
            return;
        } else if (hash[key] == last) {
            //The item is the last item in the double-linked list.
            return;
        } else {
            //The item is between the first and the last item.
            hash[key]->prev->next = hash[key]->next;
            hash[key]->next->prev = hash[key]->prev;
            hash[key]->next = nullptr;
            last->next = hash[key];
            hash[key]->prev = last;
            last = hash[key];
            return;
        }
    }
    //The item doesn't exist in Cache.
    if (cur_Size >= max_Size) {
        //Need to remove the first item.
        Cache::Node *trash = root;
        root = root->next;
        root->prev = nullptr;
        hash.remove(trash->key);
        delete trash;
    }
    //Adding the item into cache.
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
}

template <typename ValueType>
bool Cache<ValueType>::containsKey(const std::string& key) {
    return hash.containsKey(key);
}

template <typename ValueType>
ValueType Cache<ValueType>::get(const std::string& key) {
    if(!containsKey(key)) {
        error("Key DNE!");
    } else {
        if(hash[key] == root) {
            //The item is the first item in the double-linked list
            if (root != last) {
                //Move item to the back
                root = root->next;
                root->prev = nullptr;
                hash[key]->next = nullptr;
                hash[key]->prev = last;
                last->next = hash[key];
                last = hash[key];
            }
            return last->value;
        } else if (hash[key] == last) {
            //The item is the last item in the double-linked list
            //Just return the value of the item.
            return last->value;
        } else {
            //The item is between the first and the last item.
            //Move the item to the back
            hash[key]->prev->next = hash[key]->next;
            hash[key]->next->prev = hash[key]->prev;
            hash[key]->next = nullptr;
            last->next = hash[key];
            last = hash[key];
            return last->value;
        }
    }

    return {};
}
