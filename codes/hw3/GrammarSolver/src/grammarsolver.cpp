// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "grammarsolver.h"
#include "map.h"
#include "queue.h"
#include <cstdlib>
#include <random>
using namespace std;

Map<string, Vector<Queue<string>>> grammar;

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

string deBracket(string wBracket) {
    if (wBracket[0] == '<') return wBracket.substr(1, wBracket.length() - 2);
    else return wBracket;
}

void analyzeLine(string line) {
    string section = "", name = "";
    Queue<string> current = {};
    for(int i = 0; i < line.length(); i++) {
        if(line[i] == ':') {
            //End of class tag
            if (section != "") {
                name = section;
                section = "";
            }
        } else if (line[i] == ' ') {
            current.enqueue(section);
            section = "";
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

Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    Vector<string> result;

    grammar.clear();

    string readLine;
    while(getline(input, readLine)) {
        analyzeLine(readLine);
    }

    cout << grammar << endl;

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
