/*
 * CS106B Assignment 2-A
 * Word Ladder
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/11/2018
 *
 * Advanced version with extra features including:
 *      allows adding or removing a letter as a step (Line 87-103, Line 142-171, Line 169-308)
 *      allows end-points to be one step away from dictionary words (Line 108 -> function oneStep (Line 76-105), line 369)
 *
 * This program takes a slightly different approach comparing to the given pseudocode.
 * It first uses a DFS to confirm the existence of a path between the given words
 * then uses a bidirectional BFS to find the path
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

void welcomePrint();
//Printing the welcome information

bool lengthCheck(const string &from, const string &to);
//return true if the two given words are not of the same length

bool dictCheck(const Set<string> &dict, const string &from, const string &to);
//return true if either or both words don't exist in the dictionary

bool wordCheck(const string &from, const string &to);
//return true if both words are the same

bool exist(const Set<string> &dic, const string &from, const string &to);
//Uses DFS to determine if a valid ladder exists
//return true if a ladder exist from word 1 to word 2

void generateLadder(Queue<Stack<string>> &revSolve, Stack<string> &cur, const string &midWord, string &ladder, bool side);
//Generate the string 'ladder' to print out

void BFS(const Set<string> &dictionary, Queue<Stack<string>> &solve, Queue<Stack<string>> &rSolve, Set<string> &level, string &ladder, bool side);
//Breadth-First Search function of the given side

string game(const Set<string> &dic, const string &from, const string &to);
//The function that does the main calculations

void interact();
//for interaction. Takes in inputs, calls the calculation function.

int main() {

    welcomePrint();

    interact();

    cout << "Have a nice day." << endl;
    return 0;
}

void welcomePrint() {
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl << endl;
}

bool lengthCheck(const string &from, const string &to) {
    return from.length() != to.length();
}

bool oneStep(const Set<string> &dic, const string &str) {
    //Listing all possible 'nextWords'
    for(int c = 0 ; c < str.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = str;
            newWord.replace(c, 1, 1, char(a));
            //replacing the c-th character of the word to char(a) (by ascii)
            if(dic.contains(newWord)) return true;
        }
    }

    //Consider inserting a letter legal.
    for(int c = 0 ; c <= str.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = str;
            newWord.insert(c, 1, char(a));
            //inserting at the c-th character of the word, char(a) (by ascii)
            if(dic.contains(newWord)) return true;
        }
    }

    //Consider deleting a letter legal.
    for(int c = 0 ; c < str.length() ; c++) {
        string newWord = str;
        newWord.erase(c, 1);
        //deleting the c-th character of the word
        if(dic.contains(newWord)) return true;
    }
    return false;
}

bool dictCheck(const Set<string> &dict, const string &from, const string &to) {
    return !((oneStep(dict, from) || dict.contains(from)) && (oneStep(dict, to) || dict.contains(to)));
}

bool wordCheck(const string &from, const string &to) {
    return from == to;
}

bool exist(const Set<string> &dic, const string &from, const string &to) {
    //Use DFS to search if path exists

    Stack<string> tracking = {from}; //Stack of DFS process
    Set<string> trackMember = {from}; //All visited members

    Stack<int> index = {0};
    //indicating which letter of the word to change & to what letter
    //index / 26 = loc @ word; index % 26 = letter in alphabet;

    while(!tracking.isEmpty()){
        string lastWord = tracking.peek();
        if(index.peek() < lastWord.length() * 26) {
            string newWord = lastWord;
            newWord.replace(index.peek() / 26, 1, 1, char(index.peek() % 26 + 97));
            if(newWord == to) return true; //found target word. ladder exists.
            if(dic.contains(newWord) && !trackMember.contains(newWord)){
                //valid nextWord. Push into stack
                //move one level deeper.
                tracking.push(newWord);
                trackMember.add(newWord);
                index.push(0);
            } else {
                //invalid nextWord
                //move on to the next possible nextWord
                index.push(index.pop() + 1);
            }
        } else if (index.peek() < lastWord.length() * 26 + (lastWord.length() + 1) * 26) {
            string newWord = lastWord;
            newWord.insert(index.peek() / 26 - lastWord.length(), 1, char(index.peek() % 26 + 97));
            if(newWord == to) return true; //found target word. ladder exists.
            if(dic.contains(newWord) && !trackMember.contains(newWord)){
                //valid nextWord. Push into stack
                //move one level deeper.
                tracking.push(newWord);
                trackMember.add(newWord);
                index.push(0);
            } else {
                //invalid nextWord
                //move on to the next possible nextWord
                index.push(index.pop() + 1);
            }
        } else if (index.peek() < lastWord.length() * 26 + (lastWord.length() + 1) * 26 + lastWord.length()) {
            string newWord = lastWord;
            newWord.erase(index.peek() - (lastWord.length() * 26 + (lastWord.length() + 1) * 26), 1);
            if(newWord == to) return true; //found target word. ladder exists.
            if(dic.contains(newWord) && !trackMember.contains(newWord)){
                //valid nextWord. Push into stack
                //move one level deeper.
                tracking.push(newWord);
                trackMember.add(newWord);
                index.push(0);
            } else {
                //invalid nextWord
                //move on to the next possible nextWord
                index.push(index.pop() + 1);
            }
        } else {
            //all possible nextwords is visited.
            //move back to the previous word in stack.
            index.pop();
            tracking.pop();
        }
    }

    return false;
}

void generateLadder(Queue<Stack<string>> &revSolve, Stack<string> &cur, const string &midWord, string &ladder, bool side) {
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

void BFS(const Set<string> &dictionary, Queue<Stack<string>> &solve, Queue<Stack<string>> &rSolve, Set<string> &level, string &ladder, bool side) {
    Stack<string> current = solve.dequeue(); //Get item from Queue
    string lastWord = current.peek(); //Peek the top word of the current Stack
    Set<string> cont = {}; //To store words already contained in the Stack
    int cSize = current.size(); //size of the current Stack

    //Add all the items of the Stack to the Set 'cont'
    for(int t = 0 ; t < cSize ; t++) {
        string temp = current.pop();
        cont.add(temp);
        current.push(temp);
    }

    //Listing all possible 'nextWords'
    for(int c = 0 ; c < lastWord.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = lastWord;
            newWord.replace(c, 1, 1, char(a));
            //replacing the c-th character of the word to char(a) (by ascii)

            if(dictionary.contains(newWord) && !cont.contains(newWord)) {
                //new word is a valid word
                if (level.contains(newWord)) {
                    //shortest ladder found
                    generateLadder(rSolve, current, newWord, ladder, side);
                    return;
                }
                current.push(newWord); //Add the valid word to the current stack
                solve.enqueue(current); //Add the stack back into the back of the queue
                current.pop(); //Remove the word from the stack
            }
        }
    }

    //Consider inserting a letter legal.
    for(int c = 0 ; c <= lastWord.length() ; c++) {
        for(int a = 97 ; a < 123 ; a++) {
            string newWord = lastWord;
            newWord.insert(c, 1, char(a));
            //inserting at the c-th character of the word, char(a) (by ascii)

            if(dictionary.contains(newWord) && !cont.contains(newWord)) {
                //new word is a valid word
                if (level.contains(newWord)) {
                    //shortest ladder found
                    generateLadder(rSolve, current, newWord, ladder, side);
                    return;
                }
                current.push(newWord); //Add the valid word to the current stack
                solve.enqueue(current); //Add the stack back into the back of the queue
                current.pop(); //Remove the word from the stack
            }
        }
    }

    //Consider deleting a letter legal.
    for(int c = 0 ; c < lastWord.length() ; c++) {
        string newWord = lastWord;
        newWord.erase(c, 1);
        //deleting the c-th character of the word

        if(dictionary.contains(newWord) && !cont.contains(newWord)) {
            //new word is a valid word
            if (level.contains(newWord)) {
                //shortest ladder found
                generateLadder(rSolve, current, newWord, ladder, side);
                return;
            }
            current.push(newWord); //Add the valid word to the current stack
            solve.enqueue(current); //Add the stack back into the back of the queue
            current.pop(); //Remove the word from the stack
        }
    }
}

string game(const Set<string> &dic, const string &from, const string &to) {

    Queue<Stack<string>> solveFrom; //Stores the information for the word 1 side
    Queue<Stack<string>> solveTo; //Stores the information for the word 2 side
    Set<string> fromLevel = {to}, toLevel = {from}; //Stores the words visited by each side
    string ladder = ""; //The result string to return

    solveFrom.add({to}); //Initialize
    solveTo.add({from}); //Initialize

    while(!solveFrom.isEmpty() && !solveTo.isEmpty()){

        //Search from word 1 side
        BFS(dic, solveFrom, solveTo, toLevel, ladder, true);
        if(ladder != "") return ladder; //Ladder found

        //Update word 1 side visited-word pool
        for(Stack<string> item : solveFrom) {
            fromLevel.add(item.peek());
        }

        //Search from word 2 side
        BFS(dic, solveTo, solveFrom, fromLevel, ladder, false);
        if(ladder != "") return ladder; //Ladder found

        //Update word 2 side visited-word pool
        for(Stack<string> item : solveTo) {
            toLevel.add(item.peek());
        }
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

        if (dictCheck(dict, fromWord, toWord)) {
            cout << "The two words must be found in the dictionary, or one step from a dictionary word." << endl;
            continue;
        } else if (wordCheck(fromWord, toWord)) {
            cout << "The two words must be different." << endl;
            continue;
        } else {
            if (!exist(dict, fromWord, toWord)) {
                cout << "No word ladder found from " << toWord << " back to " << fromWord << "." << endl;
            } else {
                cout << "A ladder from " << toWord << " back to " << fromWord << ":" << endl;
                cout << game(dict, fromWord, toWord) << endl;
            }
        }
    }
}
