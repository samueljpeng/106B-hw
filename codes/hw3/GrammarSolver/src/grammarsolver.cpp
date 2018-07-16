// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "grammarsolver.h"
#include "map.h"
#include "queue.h"
#include <cstdlib>
#include <random>
using namespace std;

bool read = false;
Map<string, Vector<Queue<string>>> grammar;
Map<string, bool> bracketed;

string generate(string kind) {
    Vector<Queue<string>> gen = grammar[kind];
    string result = "";
    Queue<string> current = gen[rand() % gen.size()];
    while(!current.isEmpty()) {
        string item = current.dequeue();
        if(bracketed[kind]) {
            result += generate(item);
            continue;
        } else {
            result += item;
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
    bool bracket;
    for(int i = 0; i < line.length(); i++) {
        if(line[i] == ':') {
            //End of class tag
            if (section != "") {
                name = section.substr(1, section.length() - 2);
                section = "";
            }
        } else if (line[i] == ' ') {
            current.enqueue(deBracket(section));
            section = "";
        } else if (line[i] == '|') {
            current.enqueue(deBracket(section));
            section = "";
            grammar[name].add(current);
            current.clear();
        } else if (i == line.length() - 1) {
            section += line[i];
            current.enqueue(deBracket(section));
            section="";
            grammar[name].add(current);
            current.clear();
        } else if (line[i] == '=') {
            if (line[i+1]=='<') bracket = true;
            else bracket = false;
            bracketed[name] = bracket;
            continue;
        } else {
            section += line[i];
        }
    }
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    Vector<string> result;

    if (!read) {
        string readLine;
        while(getline(input, readLine)) {
            analyzeLine(readLine);
        }
        read = true;
    }

    if (grammar[deBracket(symbol)].isEmpty()) {
        for(int i = 0 ; i < times ; i++) {
            result.add(symbol);
        }
    } else {
        for(int i = 0 ; i < times ; i++) {
            result.add(generate(deBracket(symbol)));
        }
    }

    return result;
}
