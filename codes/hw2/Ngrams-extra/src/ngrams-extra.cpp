/*
 * CS106B Assignment 2-B
 * Ngrams
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/11/2018
 *
 * Advanced version with extra features:
 *      Complete sentences. (Function ended(Line 62-67), Line 156-164)
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

Map<Queue<string>, Vector<string>> doc; //Storing the document
int n;  //Storing the document

void welcomePrint();

string nextWord(const Vector<string> &lib);

bool ended(string word);

void getInfo();

void interact();

int main() {

    welcomePrint();

    getInfo();

    interact();

    cout << "Exiting." << endl;
    return 0;
}

void welcomePrint() {
    //Printing welcome information
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}

string nextWord(const Vector<string> &lib) {
    //Return the next word randomly chosen based on possibility
    return lib[rand() % lib.size()];
}

bool ended(string word) {
    //Detect if the given word is the end of sentence
    return word.substr(word.length() - 1, 1) == "." ||
            word.substr(word.length() - 1, 1) == "!" ||
            word.substr(word.length() - 1, 1) == "?";
}

void getInfo() {
    //Get information from user i.e. Input

    ifstream infile;
    promptUserForFile(infile, "Input file name?");

    n = getInteger("Value of N?");

    string word;
    string savedWords[n - 1];
    int index = 0;
    Queue<string> preWords;

    //Processing document word by word
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
    //Main functionalities

    int gen = getInteger("# of random words to generate (0 to quit):");
    while(gen) {
        if (!(gen > 0)) {
            cout << "Invalid input." << endl << endl;
        } else if (gen - n + 1 < 0) {
            cout << "Must be at least" << n << "words."<< endl << endl;
        } else {
            Queue<string> startKey;

            //Randomly generate startKey
            //Repeat until the begining of the word is Upper Case
            do {
                startKey = doc.keys().get(rand() % doc.size());
            } while (toLowerCase(startKey.peek().substr(0, 1)) == startKey.peek().substr(0, 1));

            cout << startKey.peek() << " ";
            startKey.enqueue(startKey.dequeue());

            //printing start key contents
            for(int i = 0 ; i < n - 2 ; i++) {
                cout << startKey.peek() << " ";
                startKey.enqueue(startKey.dequeue());
            }

            string nextWd;
            //Generating paragraph word by word
            for(int i = 0 ; i < gen - n + 1 ; i++) {
                nextWd = nextWord(doc[startKey]);
                cout << nextWd << " " ;
                startKey.enqueue(nextWd);
                startKey.dequeue();
            }

            //After the given amount of words. If not ended, continue.
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
