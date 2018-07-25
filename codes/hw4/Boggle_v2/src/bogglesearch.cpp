// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <string>
#include <utility>
#include <unistd.h>
#include "bogglegui.h"
#include "grid.h"
#include "lexicon.h"
#include "set.h"
using namespace std;
#define PREVIOUS -1
#define NEXT 1
#define CURRENT 0

bool humanWordSearchHelper(const Grid<char> &board, const string& find, string cur,
                           int cRow, int cCol, Set<pair<int, int>> &history) {
    BoggleGUI::setHighlighted(cRow, cCol, true);
    usleep(100);
    if (board[cRow][cCol] != find[cur.length()]) {
        BoggleGUI::setHighlighted(cRow, cCol, false);
        return false;
    } else {
        cur += board[cRow][cCol];
        pair<int, int> current(cRow, cCol);
        history.add(current);
        if (find == cur) return true;
        for (int i = PREVIOUS; i <= NEXT; i++) {
            for (int j = PREVIOUS; j <= NEXT; j++) {
                pair<int, int> newPt(cRow + i, cCol + j);
                if ((i == CURRENT && j == CURRENT) || history.contains(newPt) ||
                        !board.inBounds(newPt.first, newPt.second)) continue;
                if (humanWordSearchHelper(board, find, cur, newPt.first, newPt.second, history)) return true;


            }
        }
        cur = cur.substr(0, cur.length() - 1);
        history.remove(current);
    }

    BoggleGUI::setHighlighted(cRow, cCol, false);
    return false;
}

bool humanWordSearch(const Grid<char>& board, const Lexicon& dictionary, const string& word) {
    string myWord = toUpperCase(word);
    if (!dictionary.contains(myWord) || myWord.length() < BoggleGUI::MIN_WORD_LENGTH) return false;
    //return false if word not found in dictionary or wordLength smaller than required

    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            Set<pair<int, int>> process;
            //BoggleGUI::setHighlighted(i, j, true);
            //usleep(100);
            if (humanWordSearchHelper(board, myWord, "", i, j, process)) {
                cout << "found solution! Adding score..." << endl;
                switch (myWord.length()) {
                case 4:
                    cout << "case 4";
                    BoggleGUI::scorePointsHuman(1);
                    break;
                case 5:
                    cout << "case 5";
                    BoggleGUI::scorePointsHuman(2);
                    break;
                case 6:
                    cout << "case 6";
                    BoggleGUI::scorePointsHuman(3);
                    break;
                case 7:
                    cout << "case 7";
                    BoggleGUI::scorePointsHuman(5);
                    break;
                default:
                    cout << "case else";
                    BoggleGUI::scorePointsHuman(11);
                }
                BoggleGUI::clearHighlighting();
                return true;
            }

            //BoggleGUI::clearHighlighting();
        }
    }

    return false; //Return false if no path found
}

void computerWordSearchHelper(const Grid<char> &board, const Lexicon &dict, const Set<string> &human, string cur,
                                     int cRow, int cCol, Set<pair<int, int>> &history, Set<string> &result) {
    Set<string> res;
    if (cur.length() >= 4 && !result.contains(cur) && dict.contains(cur) && !human.contains(cur)) {
        switch (cur.length()) {
        case 4:
            cout << "case 4";
            BoggleGUI::scorePointsComputer(1);
            break;
        case 5:
            cout << "case 5";
            BoggleGUI::scorePointsComputer(2);
            break;
        case 6:
            cout << "case 6";
            BoggleGUI::scorePointsComputer(3);
            break;
        case 7:
            cout << "case 7";
            BoggleGUI::scorePointsComputer(5);
            break;
        default:
            cout << "case else";
            BoggleGUI::scorePointsComputer(11);
        }
        result.add(cur);
    }
    cout << "Currently at row: " << cRow << ", " << cCol << " ...";
    for (int i = PREVIOUS; i <= NEXT; i++) {
        for (int j = PREVIOUS; j <= NEXT; j++) {
            pair<int, int> newPt(cRow + i, cCol + j);
            if ((i == CURRENT && j == CURRENT) || history.contains(newPt) ||
                    !board.inBounds(newPt.first, newPt.second)) continue;
            cur += board[newPt.first][newPt.second];
            history.add(newPt);
            if (dict.containsPrefix(cur)) {
                computerWordSearchHelper(board, dict, human, cur, newPt.first, newPt.second, history, result);
            }
            cur = cur.substr(0, cur.length() - 1);
            history.remove(newPt);
        }
    }
}

Set<string> computerWordSearch(const Grid<char>& board, const Lexicon& dictionary, const Set<string>& humanWords) {
    Set<string> res; //Result set to return
    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            Set<pair<int, int>> process;
            computerWordSearchHelper(board, dictionary, humanWords, "", i, j, process, res);
        }
    }
    return res;
}
