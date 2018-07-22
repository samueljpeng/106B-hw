// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "point.h"
using namespace std;

bool humanWordSearchHelper(Grid<char> &board, string word, string cur, int cRow, int cCol, Set<Point> &history) {
    cout << "currently at cRow = " << cRow << ", cCol = " << cCol << " ..." << endl;
    if (board[cRow][cCol] != word[cur.length()]) {
        cout << "Character doesn't match, returning false ..." << endl;
        return false;
    } else {
        cur += board[cRow][cCol];
        Point current(cRow, cCol);
        history.add(current);
        if (word == cur) {
            cout << "Word matched, returning true ..." << endl;
            return true;
        }
        cout << "looping through neighobrs ... " << endl;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if(!(i == 0 && j == 0) && !history.contains({cRow + i, cCol + j}) && board.inBounds(cRow + i, cCol + j) &&
                        humanWordSearchHelper(board, word, cur, cRow + i, cCol+ j, history)) return true;
            }
        }
        cur = cur.substr(0, cur.length() - 1);
    }
    return false;
}

bool humanWordSearch(const Grid<char>& board, const Lexicon& dictionary, const string& word) {
    if (!dictionary.contains(word)) {
        cout << "Dictionary doesn't contain this word..." << endl;
        return false;
    } else {
        cout << "Looping through the board..." << endl;
        for (int i = 0; i < board.numRows(); i++) {
            for (int j = 0; j < board.numCols(); j++) {
                cout << "Stepping in row = " << i << ", col = " << j << " ..." << endl;
                Set<Point> process;
                if (humanWordSearchHelper(board, word, "", i, j, process)) return true;
            }
        }
    }
    return false;
}

Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    // TODO: write this function
    Set<string> todo;   // this is only here so it will compile
    return todo;        // this is only here so it will compile
}
