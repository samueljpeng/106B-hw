// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
using namespace std;

bool humanWordSearchHelper(Grid<char> &board, string word, string cur, int cRow, int cCol) {
    if(board[cRow][cCol] != word[cur.length()]) {
        return false;
    } else {
        cur += board[cRow][cCol];

        cur = cur.substr(0, cur.length() - 1);
    }
}

bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    if(!dictionary.contains(word)) return false;
    else
    // TODO: write this function
    return false;   // this is only here so it will compile
}

Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    // TODO: write this function
    Set<string> todo;   // this is only here so it will compile
    return todo;        // this is only here so it will compile
}
