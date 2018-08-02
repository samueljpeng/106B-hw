#include "History.h"
#include "error.h"
using namespace std;

History::History() {
    cout << "inside History function\n";
    root = curr = nullptr;
//    root = new History::Node;
//    root->next = root->prev = nullptr;
//    curr = root;
}

History::~History() {
    cout << "inside ~History function\n";
    if(curr != nullptr) delete curr;
    while(root != nullptr) {
        History::Node *trash = root;
        root = root -> next;
        delete trash;
    }
}

bool History::hasNext() const {
    cout << "inside hasNext function\n";
    if(curr == nullptr) return false;
    if(curr -> next != nullptr) return true;
    return false;
}

bool History::hasPrevious() const {
    cout << "inside hasPrevious function\n";
    if(curr == nullptr) return false;
    if(curr -> prev != nullptr) return true;
    return false;
}

string History::next() {
    cout << "inside next function\n";
    cout << curr->title << " " ;
    if (curr->next != nullptr) cout << curr->next->title << " ";
    else cout << "next=null ";
    if (curr->prev != nullptr) cout << curr->prev->title << endl;
    else cout << "prev=null \n";
    if(History::hasNext()) {
        curr = curr -> next;
        return curr -> title;
    } else error("There is no next page.");
    return "";
}

string History::previous() {
    cout << "inside previous function\n";
    cout << curr->title << " " ;
    if (curr->next != nullptr) cout << curr->next->title << " ";
    else cout << "next=null ";
    if (curr->prev != nullptr) cout << curr->prev->title << endl;
    else cout << "prev=null \n";
    if(History::hasPrevious()) {
        curr = curr -> prev;
        return curr -> title;
    } else error("There is no previous page.");
    return "";
}

void History::goTo(const string& page) {
    cout << "inside goTo " << page << " function\n";
    History::Node *newNode = new History::Node;
    newNode->title = page;
    newNode->prev = curr;
    newNode->next = nullptr;
    if(hasNext()) {
        cout << "Destructing" << endl;
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
    History::Node *printing = root;
    while(printing != nullptr) {
        cout << printing->title << " -> ";
        printing = printing -> next;
    }
    cout << endl;
    (void) page;
}
