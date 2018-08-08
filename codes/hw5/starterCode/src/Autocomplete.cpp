/*
 * CS106B Assignment 5-D
 * Autocomplete.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/08/2018
 *
 * Basic feature:
 * An object providing autocomplete responses given a string prefix.
 */

#include "Autocomplete.h"
#include <cctype>
using namespace std;

Autocomplete::Autocomplete() {
    root = new Autocomplete::Node;
    root->endPoint = false;
    root->letters.clear();
}

void Autocomplete::destroyTrie(Autocomplete::Node *node) {
    for(char item : node->letters) {
        destroyTrie(node->letters[item]);
    }
    delete node;
}

Autocomplete::~Autocomplete() {
    destroyTrie(Autocomplete::root);
}

//Helper function for add. Add the given text to the structure.
//Takes the string of text, and a node. Recursively add the word to the trie.
void Autocomplete::addHelper(const string& text, Autocomplete::Node *node) {
    if (text.length() == 0) return; //Base case
    else if (node->letters.containsKey(text[0])) {
        //Current letter already exists in the next level of the trie
        if (text.length() == 1) node->letters[text[0]]->endPoint = true; //Marking the end of the word
        else Autocomplete::addHelper(text.substr(1, text.length() - 1), node->letters[text[0]]); //Recurse
    } else {
        //Add current letter to the Map of the next level
        node->letters.put(text[0], new Autocomplete::Node);
        node->letters[text[0]]->endPoint = false;
        node->letters[text[0]]->letters.clear();
        if (text.length() == 1) node->letters[text[0]]->endPoint = true; //Marking the end of the word
        else Autocomplete::addHelper(text.substr(1, text.length() - 1), node->letters[text[0]]); //Recurse
    }
}

void Autocomplete::add(const string& text) {
    addHelper(text, Autocomplete::root);
}

//Helper function for the suggestionsFor function.
//Takes in the existing part of str, and a node, and returns a node, at which
//    the currently existing part of str stops, i.e. the node to start looking
//    for autocomplete contents.
Autocomplete::Node *Autocomplete::getPrefixNode(string prefix, Autocomplete::Node *node) const {
    if (prefix.length() == 0) return Autocomplete::root; //if the prefix is empty.
    else if (prefix.length() == 1) {
        if (node->letters.containsKey(prefix[0])) return node->letters[prefix[0]]; //Base case, found the node.
        else return nullptr;
    } else {
        if (node->letters.containsKey(prefix[0]))
            return Autocomplete::getPrefixNode(prefix.substr(1, prefix.length() - 1), node->letters[prefix[0]]);
        else return nullptr;
    }
}

//Helper function for the suggestionsFor function.
//Takes in the existing part of str, a string saving the current process, an int
//    saving the max amount of items, a node, and a set of strings by reference
//    to store the items to be found.
//Void functions, all found items stored into the set of strings passed in by reference.
void Autocomplete::suggestHelper(const string& prefix, string &process, int maxHits, Autocomplete::Node *node, Set<string> &suggests) const {
    if (suggests.size() == maxHits) return; //Reached size limit.
    if (node->endPoint == true) suggests.add(prefix + process); //Found a completed item.
    for (char nextLetter : node->letters) { //Traverse all the existing "next letters" of the given node
        process += nextLetter;
        suggestHelper(prefix, process, maxHits, node->letters[nextLetter], suggests);
        process = process.substr(0, process.length() - 1);
    }
}

Set<string> Autocomplete::suggestionsFor(const string& prefix, int maxHits) const {
    Set<string> suggestions = {}; //The resulting set to be returned
    Autocomplete::Node *start;
    start = Autocomplete::getPrefixNode(prefix, Autocomplete::root); //Find the node from which to start looking.
    string processTracker;
    if (start != nullptr) suggestHelper(prefix, processTracker, maxHits, start, suggestions);
    return suggestions;
}
