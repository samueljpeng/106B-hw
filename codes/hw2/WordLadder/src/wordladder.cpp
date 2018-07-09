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
#include "filelib.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "simpio.h"
#include "console.h"
using namespace std;

void welcomePrint();
//Printing the welcome information

bool lengthCheck(string from, string to);
//return true if the two given words are not of the same length

bool dictCheck(const Set<string> &dict, string from, string to);
//return true if either or both words don't exist in the dictionary

bool wordCheck(string from, string to);
//return true if both words are the same

string game(const Set<string> &dic, string from, string to);
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

bool lengthCheck(string from, string to) {
    return from.length() != to.length();
}

bool dictCheck(const Set<string> &dict, string from, string to) {
    return !(dict.contains(from) && dict.contains(to));
}

bool wordCheck(string from, string to) {
    return from == to;
}

string game(const Set<string> &dic, string from, string to) {
    Queue<Stack<string>> solve;
    //Main Queue saving the process of BFS

    string ladder = ""; //Result string to return
    Set<string> cont = {to}; //To store words already contained in the Stack

    solve.add({to}); //Initialize solve Queue

    while(!solve.isEmpty()){
        //Loop through the Queue
        Stack<string> current = solve.dequeue(); //Get item from Queue
        string lastWord = current.peek();//Peek the top word of the current Stack

        //Listing all possible 'nextWords'
        for(int c = 0 ; c < lastWord.length() ; c++) {
            for(int a = 97 ; a < 123 ; a++) {
                string newWord = lastWord;
                newWord.replace(c, 1, 1, char(a));
                //replacing the c-th character of the word to char(a) (by ascii)

                if(dic.contains(newWord) && !cont.contains(newWord)) {
                    //new word is a valid word
                    if (newWord == from) {
                        //Found the shortest ladder
                        current.push(newWord);
                        for(string path : current) {
                            ladder += path;
                            ladder += " ";
                        }
                        return ladder;
                    }
                    current.push(newWord); //Add the valid word to the current stack
                    cont.add(newWord); //Add the valid word to 'visited' set
                    solve.enqueue(current); //Add the stack back into the back of the queue
                    current.pop(); //Remove the word from the stack
                }
            }
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
            string gameResult = game(dict, fromWord, toWord);
            if (gameResult == "NA") {
                cout << "No word ladder found from " << toWord << " back to " << fromWord << "." << endl;
            } else {
                cout << "A ladder from " << toWord << " back to " << fromWord << ":" << endl;
                cout << gameResult << endl;
            }
        }
    }
}
