#include "Autocomplete.h"
#include <cctype>
using namespace std;

Autocomplete::Autocomplete() {
    cout << "in autocomplete\n";
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

void Autocomplete::addHelper(const string& text, Autocomplete::Node *node) {
    if (text.length() == 0) return;
    else if (node->letters.containsKey(text[0])) {
        if (text.length() == 1) node->letters[text[0]]->endPoint = true;
        else Autocomplete::addHelper(text.substr(1, text.length() - 1), node->letters[text[0]]);
    } else {
        node->letters.put(text[0], new Autocomplete::Node);
        node->letters[text[0]]->endPoint = false;
        node->letters[text[0]]->letters.clear();
        if (text.length() == 1) node->letters[text[0]]->endPoint = true;
        else Autocomplete::addHelper(text.substr(1, text.length() - 1), node->letters[text[0]]);
    }
}

void Autocomplete::add(const string& text) {
    addHelper(text, Autocomplete::root);
}

Autocomplete::Node *Autocomplete::getPrefixNode(string prefix, Autocomplete::Node *node) const {
    if (prefix.length() == 0) return Autocomplete::root;
    else if (prefix.length() == 1) {
        if (node->letters.containsKey(prefix[0])) return node->letters[prefix[0]];
        else return nullptr;
    } else {
        if (node->letters.containsKey(prefix[0]))
            return Autocomplete::getPrefixNode(prefix.substr(1, prefix.length() - 1), node->letters[prefix[0]]);
        else return nullptr;
    }
}

void Autocomplete::suggestHelper(const string& prefix, string &process, int maxHits, Autocomplete::Node *node, Set<string> &suggests) const {
    if (suggests.size() == maxHits) return;
    if (node->endPoint == true) suggests.add(prefix + process);
    for (char nextLetter : node->letters) {
        process += nextLetter;
        suggestHelper(prefix, process, maxHits, node->letters[nextLetter], suggests);
        process = process.substr(0, process.length() - 1);
    }
}

/* Find some recommendations! */
Set<string> Autocomplete::suggestionsFor(const string& prefix, int maxHits) const {
    Set<string> suggestions = {};
    Autocomplete::Node *start;
    start = Autocomplete::getPrefixNode(prefix, Autocomplete::root);
    string processTracker;
    if (start != nullptr) suggestHelper(prefix, processTracker, maxHits, start, suggestions);
    return suggestions;
}
