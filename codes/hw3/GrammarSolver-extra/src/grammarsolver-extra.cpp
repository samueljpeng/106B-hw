/*
 * CS106B Assignment 2-B
 * GrammarSolver
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/18/2018
 *
 * Advanced version with extra features including:
 *     Robust grammar solver: able to handle excessive white space in file
 */
#include "grammarsolver.h"
#include "map.h"
#include "queue.h"
#include <cstdlib>
#include <random>
using namespace std;

Map<string, Vector<Queue<string>>> grammar;
Stack<string> keys;

string generate(string kind) {
    Vector<Queue<string>> gen = grammar[kind];
    string result = "";
    Queue<string> current = gen[rand() % gen.size()];
    while(!current.isEmpty()) {
        string item = current.dequeue();
        if(grammar[item].isEmpty()) {
            result += item;
        } else {
            result += generate(item);
            continue;
        }
        result+=" ";
    }
    return result;
}

void analyzeLine(string line) {
    string section = "", name = "";
    Queue<string> current = {};
    for(int i = 0; i < line.length(); i++) {
        if(line[i] == ':') {
            //End of class tag
            if (section != "") {
                name = section;
                keys.push(name);
                section = "";
            }
        } else if (line[i] == ' ') {
            if(name == "" || section == "") {
                continue;
            } else {
                current.enqueue(section);
                section = "";
            }
        } else if (line[i] == '|') {
            current.enqueue(section);
            section = "";
            grammar[name].add(current);
            current.clear();
        } else if (i == line.length() - 1) {
            section += line[i];
            current.enqueue(section);
            section="";
            grammar[name].add(current);
            current.clear();
        } else if (line[i] == '=') {
            continue;
        } else {
            section += line[i];
        }
    }
}

bool bc(Vector<Queue<string>> &process, string &to) {
    if(to == "" && !process.isEmpty()) return false;
    sort(process.begin(), process.end(), [](Queue<string> &q1, Queue<string> &q2) { return q1.size() > q2.size(); });
    for(int i = 0; i < process.size(); i++) {
        cout << "Current process is: " << process << " ..." << endl;
        cout << "Current \"to\" is: " << to << " ..." << endl;
        Queue<string> current = process[i];
        cout << "    Processing queue = " << current << " ..." << endl;
        cout << "    i is currently " << i << ", and i is < " << process.size() << " ..." << endl;
        while(!current.isEmpty()) {
            string item = current.dequeue();
            cout << "        Processing item = " << item << " ..." << endl;
            if(grammar[item].isEmpty()) {
                cout << "            grammar[item] is Empty" << endl;
                cout << "            to.substr(0, item.length()) is " << to.substr(0, item.length()) << " ..." << endl;
                cout << "            item is "<< item << " ..." << endl;
                if (to.substr(0, item.length()) == item) {
                    cout << "            match ..." << endl;
                    if (to.length() > item.length()) to = to.substr(item.length() + 1, to.length() - item.length());
                    else to = "";
                    cout << "            to updated to " << to << " ..." << endl;
                    return true;
                } else {
                    cout << "            not match ..." << endl;
                    continue;
                }
            } else {
                cout << "            grammar[item] is not Empty" << endl;
                if(bc(grammar[item], to)) {
                    cout << "            this return is for ..." << endl;
                    cout << "                current: " << current << endl;
                    cout << "                Vector: " << grammar[item] << endl;
                    cout << "                to: " << to << endl;
                    if (current.isEmpty()) {
                        cout << "            previous step returned true. Returning true ..." << endl;
                        return true;
                    } else {
                        cout << "            previous step returned true. Continue process ..." << endl;
                        continue;
                    }
                } else {
                    cout << "            this return is for ..." << endl;
                    cout << "                Vector: " << grammar[item] << endl;
                    cout << "                to: " << to << endl;
                    cout << "            previous step returned false. Break loop ..." << endl;
                    break;
                }
            }
        }
    }
    return false;
}

bool possible(string sentence, string &id) {

    for(string key : keys) {

        cout<<"Processing key = " << key << " ..." << endl;

        string cSentence = sentence;
        Vector<Queue<string>> gen = grammar[key];
        cout << "The vector for this key is " << gen << " ..." << endl;
        cout << "The size of gen is " << gen.size() << " ..." << endl;

        if (bc(gen, cSentence) && cSentence == "") {
            id = key;
            cout << "FIRST LEVEL FUNCTION RETURNING TRUE ..." << endl;
            return true;
        }
    }

    cout << "FIRST LEVEL FUNCTION RETURNING FALSE ..." << endl << endl;
    return false;
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    Vector<string> result;

    grammar.clear();

    string readLine;
    while(getline(input, readLine)) {
        analyzeLine(readLine);
    }

    if (times == -1) {
        string wordType;
        if (possible(symbol, wordType)) result.add("This is grammatically correct. It is a " + wordType);
        else result.add("This is not grammatically correct.");
        return result;
    }

    if (grammar[symbol].isEmpty()) {
        for(int i = 0 ; i < times ; i++) {
            result.add(symbol);
        }
    } else {
        for(int i = 0 ; i < times ; i++) {
            result.add(generate(symbol));
        }
    }

    return result;
}
