/*
 * CS106B Assignment 2-A
 * Word Ladder
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/11/2018
 *
 * Advanced version with extra features including:
 *      allows adding or removing a letter as a step (Linve 74-90, Line 194-212)
 *      allows end-points to be one step away from dictionary words (Line 96 -> function oneStep (Line 62-92), line 268)
 *
 * This program takes a slightly different approach comparing to the given pseudocode as an optimization.
 * It uses bidirectional BFS to find the path (Line 226-234 -> function BFS (Line 176 - 213) & function BFS_helper (Line 159-174))
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include "filelib.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "simpio.h"
#include "console.h"
using namespace std;

Set<string> dict; //Saving all words from dictionary

void welcomePrint();

bool oneStep(const Set<string> &dic, const string &str);

bool dictCheck(const Set<string> &dict, const string &from, const string &to);

bool wordCheck(const string &from, const string &to);

void generateLadder(Queue<Stack<string>> &revSolve, Stack<string> &cur, const string &midWord, string &ladder, bool side);

void BFS(const Set<string> &dictionary, Queue<Stack<string>> &solve, Queue<Stack<string>> &rSolve, Set<string> &level, string &ladder, bool side);

string game(const Set<string> &dic, const string &from, const string &to);

void file_read();

void interact();

int main() {

    welcomePrint();

    file_read();

    interact();

    cout << "Have a nice day." << endl;
    return 0;
}

void welcomePrint() {
    //Printing the welcome information
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl << endl;
}

bool oneStep(const string &str) {
    //Listing all possible 'nextWords'

    for(int c = 0 ; c < str.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = str;
            newWord.replace(c, 1, 1, char(a));
            //replacing the c-th character of the word to char(a) (by ascii)
            if(dict.contains(newWord)) return true;
        }
    }

    //Consider inserting a letter legal.
    for(int c = 0 ; c <= str.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = str;
            newWord.insert(c, 1, char(a));
            //inserting at the c-th character of the word, char(a) (by ascii)
            if(dict.contains(newWord)) return true;
        }
    }

    //Consider deleting a letter legal.
    for(int c = 0 ; c < str.length() ; c++) {
        string newWord = str;
        newWord.erase(c, 1);
        //deleting the c-th character of the word
        if(dict.contains(newWord)) return true;
    }
    return false;
}

bool dictCheck(const string &from, const string &to) {
    //return true if either or both words is more than one step from dictionary word
    return !((oneStep(from) || dict.contains(from)) && (oneStep(to) || dict.contains(to)));
}

bool wordCheck(const string &from, const string &to) {
    //return true if both words are the same
    return from == to;
}

void generateLadder(Queue<Stack<string>> &revSolve, Stack<string> &cur, const string &midWord, string &ladder, bool side) {
    //Generate the string 'ladder' to print out

    if (side) {
        //Ladder found with solveFront & toLevel

        //Add the front part (word 1 side) to 'ladder'
        for(string path : cur) {
            ladder += path;
            ladder += " ";
        }

        //Add the second part (word 2 side) to 'ladder'
        for(Stack<string> item : revSolve) {
            if(item.peek() == midWord) {
                Stack<string> reverse;
                int stackSize = item.size();
                for (int loc = 0 ; loc < stackSize ; loc++) {
                    reverse.push(item.pop());
                }
                for (string str : reverse) {
                    ladder += str;
                    ladder += " ";
                }
                break;
            }
        }
    } else {
        //Ladder found with solveTo & fromLevel

        //Add the front part (word 1 side) to 'ladder'
        for(Stack<string> item : revSolve) {
            if(item.peek() == midWord) {
                for (string str : item) {
                    ladder += str;
                    ladder += " ";
                }
                break;
            }
        }

        //Generate the string for the second part (word 2 side)
        string halfResult;
        for(string path : cur) {
            halfResult = path + " " + halfResult;
        }

        ladder += halfResult;//Combine two parts to 'ladder'
    }
}

void BFS_helper(Queue<Stack<string>> &solve, Queue<Stack<string>> &rSolve, Set<string> &myLevel, Set<string> &level,
                Stack<string> &current, string &ladder, string newWord, bool side) {
    //Handling a new word
    if(dict.contains(newWord) && !myLevel.contains(newWord)) {
        //new word is a valid word
        if (level.contains(newWord)) {
            //shortest ladder found
            generateLadder(rSolve, current, newWord, ladder, side);
            return;
        }
        current.push(newWord); //Add the valid word to the current stack
        myLevel.add(newWord); //Add the valid word to 'visited' set
        solve.enqueue(current); //Add the stack back into the back of the queue
        current.pop(); //Remove the word from the stack
    }
}

void BFS(Queue<Stack<string>> &solve, Queue<Stack<string>> &rSolve, Set<string> &myLevel,
         Set<string> &level, string &ladder, bool side) {
    //Breadth-First Search function of the given side

    Stack<string> current = solve.dequeue(); //Get item from Queue
    string lastWord = current.peek(); //Peek the top word of the current Stack

    //Listing all possible 'nextWords'
    for(int c = 0 ; c < lastWord.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = lastWord;
            newWord.replace(c, 1, 1, char(a));
            //replacing the c-th character of the word to char(a) (by ascii)

            BFS_helper(solve, rSolve, myLevel, level, current, ladder, newWord, side);
        }
    }

    //Consider inserting a letter legal.
    for(int c = 0 ; c <= lastWord.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = lastWord;
            newWord.insert(c, 1, char(a));
            //inserting at the c-th character of the word, char(a) (by ascii)

            BFS_helper(solve, rSolve, myLevel, level, current, ladder, newWord, side);
        }
    }

    //Consider deleting a letter legal.
    for(int c = 0 ; c < lastWord.length() ; c++) {
        string newWord = lastWord;
        newWord.erase(c, 1);
        //deleting the c-th character of the word

        BFS_helper(solve, rSolve, myLevel, level, current, ladder, newWord, side);
    }
}

string game(const string &from, const string &to) {
    //The function that does the main calculations

    Queue<Stack<string>> solveFrom; //Stores the information for the word 1 side
    Queue<Stack<string>> solveTo; //Stores the information for the word 2 side
    Set<string> fromLevel = {to}, toLevel = {from}; //Stores the words visited by each side
    string ladder = ""; //The result string to return

    solveFrom.add({to}); //Initialize
    solveTo.add({from}); //Initialize

    while(!solveFrom.isEmpty() && !solveTo.isEmpty()){
        //Search from word 1 side
        BFS(solveFrom, solveTo, fromLevel, toLevel, ladder, true);
        if(ladder != "") return ladder; //Ladder found

        //Search from word 2 side
        BFS(solveTo, solveFrom, toLevel, fromLevel, ladder, false);
        if(ladder != "") return ladder; //Ladder found
    }
    return "NA";
}

void file_read() {
    //Read dictionary file

    ifstream infile;
    promptUserForFile(infile, "Dictionary file name: ");

    string dictWord;
    while(getline(infile, dictWord)) {
        dict.add(dictWord);
    }

    infile.close();
}

void interact() {
    //For interaction. Calls the calculation function.

    while (1) {
        string fromWord, toWord;
        cout << endl;

        fromWord = getLine("Word 1 (or Enter to quit): ");
        if (fromWord == "") break;
        transform(fromWord.begin(), fromWord.end(), fromWord.begin(), ::tolower);

        toWord = getLine("Word 2 (or Enter to quit): ");
        if (toWord == "") break;
        transform(toWord.begin(), toWord.end(), toWord.begin(), ::tolower);

        if (dictCheck(fromWord, toWord)) {
            cout << "The two words must be found in the dictionary, or one step from a dictionary word." << endl;
            continue;
        } else if (wordCheck(fromWord, toWord)) {
            cout << "The two words must be different." << endl;
            continue;
        } else {
            string gameResult = game(fromWord, toWord);
            if (gameResult == "NA") {
                cout << "No word ladder found from " << toWord << " back to " << fromWord << "." << endl;
            } else {
                //cout << "Step 1 Done" << endl;
                cout << "A ladder from " << toWord << " back to " << fromWord << ":" << endl;
                cout << gameResult << endl;
            }
        }
    }
}
