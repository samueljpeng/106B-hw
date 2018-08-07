#include "History.h"
#include "error.h"
using namespace std;

History::History() {
    cout << "in history\n";
    root = curr = nullptr;
}

History::~History() {
    if(curr != nullptr) delete curr;
    while(root != nullptr) {
        History::Node *trash = root;
        root = root -> next;
        delete trash;
    }
}

bool History::hasNext() const {
    if(curr == nullptr) return false;
    if(curr -> next != nullptr) return true;
    return false;
}

bool History::hasPrevious() const {
    if(curr == nullptr) return false;
    if(curr -> prev != nullptr) return true;
    return false;
}

string History::next() {
    if(History::hasNext()) {
        curr = curr -> next;
        return curr -> title;
    } else error("There is no next page.");
    return "";
}

string History::previous() {
    if(History::hasPrevious()) {
        curr = curr -> prev;
        return curr -> title;
    } else error("There is no previous page.");
    return "";
}

void History::goTo(const string& page) {
    History::Node *newNode = new History::Node;
    newNode->title = page;
    newNode->prev = curr;
    newNode->next = nullptr;
    if(hasNext()) {
        History::Node *trash = curr->next;
        while(trash != nullptr) {
            History::Node *temp = trash;
            trash = trash -> next;
            delete temp;
        }
    }
    if(curr != nullptr) curr->next = newNode;
    curr = newNode;
    if(root == nullptr) root = curr;
}
