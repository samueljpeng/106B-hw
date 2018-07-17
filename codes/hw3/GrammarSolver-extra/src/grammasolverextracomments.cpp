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
/*
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

bool bc(Vector<Queue<string>> &process, string &to, string comment) {
    if(to == "" && !process.isEmpty()) return false;
    for(int i = 0; i < process.size(); i++) {
        cout << comment << "Current process is: " << process << " ..." << endl;
        cout << comment << "Current \"to\" is: " << to << " ..." << endl;
        Queue<string> current = process[i];
        cout << comment << "    Processing queue = " << current << " ..." << endl;
        cout << comment << "    i is currently " << i << ", and i is < " << process.size() << " ..." << endl;
        while(!current.isEmpty()) {
            string item = current.dequeue();
            cout << comment << "        Processing item = " << item << " ..." << endl;
            if(grammar[item].isEmpty()) {
                cout << comment << "            grammar[item] is Empty" << endl;
                cout << comment << "            to.substr(0, item.length()) is " << to.substr(0, item.length()) << " ..." << endl;
                cout << comment << "            item is "<< item << " ..." << endl;
                if (to.substr(0, item.length()) == item) {
                    cout << comment << "            match ..." << endl;
                    //return bc(process.subList(1, process.size() - 1), to.substr(item.length(), to.length() - item.length()), comment + "                ");
                    if (to.length() > item.length()) {
                        to = to.substr(item.length() + 1, to.length() - item.length());
                    } else {
                        to = "";
                    }
                    cout << comment << "            to updated to " << to << " ..." << endl;
                    return true;
                } else {
                    cout << comment << "            not match ..." << endl;
                    continue;
                }
            } else {
                cout << comment << "            grammar[item] is not Empty" << endl;
                if(bc(grammar[item], to, comment + "                ")) {
                    cout << comment << "            this return is for ..." << endl;
                    cout << comment << "                current: " << current << endl;
                    cout << comment << "                Vector: " << grammar[item] << endl;
                    cout << comment << "                to: " << to << endl;
                    if (current.isEmpty()) {
                        cout << comment << "            previous step returned true. Returning true ..." << endl;
                        return true;
                    } else {
                        cout << comment << "            previous step returned true. Continue process ..." << endl;
                        continue;
                    }
                }
                else {
                    cout << comment << "            this return is for ..." << endl;
                    cout << comment << "                Vector: " << grammar[item] << endl;
                    cout << comment << "                to: " << to << endl;
                    cout << comment << "            previous step returned false. Break loop ..." << endl;
                    break;
                }
            }
        }
    }
    return false;
}

bool helper(string key, string &to, string comment) {

    cout<<"Processing key = " << key << " ..." << endl;
    Vector<Queue<string>> gen = grammar[key];
    cout << "The vector for this key is " << gen << " ..." << endl;
    cout << "The size of gen is " << gen.size() << " ..." << endl;

    bool result = bc(gen, to, comment);

    cout << "Back to first level function ..." << endl;
    if(result && to == "") {
        cout << "FIRST LEVEL FUNCTION RETURNING TRUE ..." << endl;
        return true;
    } else {
        cout << "FIRST LEVEL FUNCTION RETURNING FALSE ..." << endl << endl;
        return false;
    }
}

bool possible(string sentence, string &id) {
    cout << "In the zero-level function" << endl;

    for(string key : keys) {
        string cSentence = sentence;
        if (helper(key, cSentence, "") == 1) {
            id = key;
            return true;
        }
    }
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
*/
