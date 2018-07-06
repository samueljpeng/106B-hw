/*
 * CS106B Assignment 2-B
 * Ngrams
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/11/2018
 *
 * Advanced version with extra features:
 * Complete sentences.
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "filelib.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "map.h"
#include "simpio.h"
#include "console.h"
using namespace std;

Map<Queue<string>, Vector<string>> doc;
int n;

void welcomePrint();

string nextWord(const Vector<string> &lib);

bool ended(string word);

void getinfo();

void game();

int main() {

    welcomePrint();

    getInfo();

    interact();

    cout << "Exiting." << endl;
    return 0;
}

void welcomePrint() {
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}

string nextWord(const Vector<string> &lib) {
    return lib[rand() % lib.size()];
}

bool ended(string word) {
    return word.substr(word.length() - 1, 1) == "." ||
            word.substr(word.length() - 1, 1) == "!" ||
            word.substr(word.length() - 1, 1) == "?";
}

void getInfo() {
    ifstream infile;
    promptUserForFile(infile, "Input file name?");

    n = getInteger("Value of N?");

    string word;
    string savedWords[n - 1];
    int index = 0;
    Queue<string> preWords;
    while(infile >> word) {
        if (index < n - 1) {
            savedWords[index] = word;
            index++;
        }
        if (preWords.size() == n - 1) {
            if (doc.containsKey(preWords)) {
                doc[preWords].add(word);
                preWords.dequeue();
                preWords.enqueue(word);
            } else {
                doc[preWords] = {word};
                preWords.dequeue();
                preWords.enqueue(word);
            }
        } else {
            preWords.enqueue(word);
        }
    }

    //wrap the file
    for(int i = 0 ; i < n - 1 ; i++) {
        word = savedWords[i];
        if (doc.containsKey(preWords)) {
            doc[preWords].add(word);
            preWords.dequeue();
            preWords.enqueue(word);
        } else {
            doc[preWords] = {word};
            preWords.dequeue();
            preWords.enqueue(word);
        }
    }

    cout << endl;
}

void interact() {
    int gen = getInteger("# of random words to generate (0 to quit):");
    while(gen) {
        if (!(gen > 0)) {
            cout << "Invalid input." << endl << endl;
        } else if (gen - n + 1 < 0) {
            cout << "Length too short!" << endl << endl;
        } else {
            Queue<string> startKey;
            do {
                startKey = doc.keys().get(rand() % doc.size());
            } while (toUpperCase(startKey.peek().substr(0, 1)) != startKey.peek().substr(0, 1));
            cout << startKey.peek() << " ";
            startKey.enqueue(startKey.dequeue());
            for(int i = 0 ; i < n - 2 ; i++) {
                cout << startKey.peek() << " ";
                startKey.enqueue(startKey.dequeue());
            }
            string nextWd;
            for(int i = 0 ; i < gen - n + 1 ; i++) {
                nextWd = nextWord(doc[startKey]);
                cout << nextWd << " " ;
                startKey.enqueue(nextWd);
                startKey.dequeue();
            }
            if(!ended(nextWd)) {
                do {
                    nextWd = nextWord(doc[startKey]);
                    cout << nextWd << " " ;
                    startKey.enqueue(nextWd);
                    startKey.dequeue();
                } while (!ended(nextWd));
            }
            cout << endl << endl;
        }

        gen = getInteger("# of random words to generate (0 to quit):");
    }
}
