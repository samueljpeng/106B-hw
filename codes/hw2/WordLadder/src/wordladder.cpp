/*
 * CS106B Assignment 2-A
 * Word Ladder
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/11/2018
 *
 * Basic version without extra features
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include "filelib.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "simpio.h"
#include "console.h"
using namespace std;

void welcomePrint() {
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl << endl;
}

bool lengthCheck(string from, string to) {
    return from.length() != to.length();
}

bool dictCheck(const Set<string> &dict, string from, string to) {
    return !(dict.contains(from) && dict.contains(to));
}

bool wordCheck(string from, string to) {
    return from == to;
}

bool nextStep(string from, string to) {
    if (lengthCheck(from, to)) return false;
    bool diff = false;
    for(int i = 0 ; i < from.length() ; i++) {
        if (from[i] != to[i] && diff == false) {
            diff = true;
        } else if (from[i] != to[i] && diff == true) {
            return false;
        }
    }
    return diff;
}

string game(const Set<string> &dic, string from, string to) {
    Queue<Stack<string>> solve;

    string ladder = "";

    solve.add({to});

    while(!solve.isEmpty()){
        int qSize = solve.size();
        for(int i = 0 ; i < qSize ; i++) {
            Stack<string> current = solve.dequeue();
            string lastWord = current.peek();
            Set<string> cont = {};
            int cSize = current.size();
            for(int t = 0 ; t < cSize ; t++) {
                string temp = current.pop();
                cont.add(temp);
                current.push(temp);
            }
            for(int c = 0 ; c < lastWord.length() ; c++) {
                for(int a = 97 ; a < 123 ; a++) {
                    string newWord = lastWord;
                    newWord.replace(c, 1, 1, char(a));
                    if(dic.contains(newWord) && !cont.contains(newWord)) {
                        if (newWord == from) {
                            //Found the path
                            current.push(newWord);
                            for(string path : current) {
                                ladder += path;
                                ladder += " ";
                            }
                            return ladder;
                        }
                        current.push(newWord);
                        solve.enqueue(current);
                        current.pop();
                    }
                }
            }
        }
        //cout << solve << endl << endl;
    }
    return "NA";
}

void interact() {
    ifstream infile;
    promptUserForFile(infile, "Dictionary file name: ");

    Set<string> dict;

    string dictWord;
    while(getline(infile, dictWord)) {
        dict.add(dictWord);
    }

    infile.close();

    while (1) {
        string fromWord, toWord;
        cout << endl;

        fromWord = getLine("Word 1 (or Enter to quit): ");
        if (fromWord == "") break;
        transform(fromWord.begin(), fromWord.end(), fromWord.begin(), ::tolower);

        toWord = getLine("Word 2 (or Enter to quit): ");
        if (toWord == "") break;
        transform(toWord.begin(), toWord.end(), toWord.begin(), ::tolower);

        if(lengthCheck(fromWord, toWord)) {
            cout << "The two words must be the same length." << endl;
            continue;
        } else if (dictCheck(dict, fromWord, toWord)) {
            cout << "The two words must be found in the dictionary." << endl;
            continue;
        } else if (wordCheck(fromWord, toWord)) {
            cout << "The two words must be different." << endl;
            continue;
        } else {
            time_t time0;
            time(&time0);
            string gameResult = game(dict, fromWord, toWord);
            if (gameResult == "NA") {
                cout << "No word ladder found from " << toWord << " back to " << fromWord << "." << endl;
            } else {
                cout << "A ladder from " << toWord << " back to " << fromWord << ":" << endl;
                cout << gameResult << endl;
            }
            time_t time1;
            time(&time1);
            cout<<"This process used " << difftime(time1, time0) << " seconds." << endl;
        }
    }
}

int main() {

    welcomePrint();

    interact();

    cout << "Have a nice day." << endl;
    return 0;
}
