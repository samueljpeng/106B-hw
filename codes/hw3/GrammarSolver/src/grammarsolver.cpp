/*
 * CS106B Assignment 2-B
 * GrammarSolver
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/18/2018
 *
 * Basic feature:
 * Given a inpur stream for grammar file, a string of kind of element, and a integer
 *     of times to generate. Generate randomly, the given kind of element for
 *     the given amount of times.
 *
 * Basic version without extra features.
 */
#include "grammarsolver.h"
#include "map.h"
#include "queue.h"
#include "error.h"
#include <cstdlib>
#include <random>
using namespace std;

string generate(string kind, const Map<string, Vector<Queue<string>>> &grammar);
void analyzeLine(string line, Map<string, Vector<Queue<string>>> &grammar); 
Vector<string> grammarGenerate(istream& input, string symbol, int times); 

//Takes a string, indicating which kind of element it's generating
//    and a Map of vector of queue of strings, with string keys,
//    which stores the grammar rules, and randomly generate a
//    element of the given kind.
string generate(string kind, const Map<string, Vector<Queue<string>>> &grammar) {
    Vector<Queue<string>> gen = grammar[kind];
    //Getting the vector of the given kind of element

    string result = ""; //result to return

    Queue<string> current = gen[rand() % gen.size()];
    //randomly select a possible queue

    while(!current.isEmpty()) { //Traverse through the elements of the queue
        string item = current.dequeue(); //Get the first element in the queue
        if(grammar[item].isEmpty()) { //The element is a word
            result += item; //Append the word to the result string
        } else { //The element is an element kind
            result += generate(item, grammar); //Recursion to generate this kind of element
            continue;
        }
        result+=" "; //Append space between words only
    }

    return result;//Return the result string
}

//Takes a string, which is the line to be analyzed, and a Map that stores the
//    grammar rules by reference. Analyse and add the information of the given line
//    into the Map of grammar rules.
void analyzeLine(string line, Map<string, Vector<Queue<string>>> &grammar) {
    string section = "", name = "";
    Queue<string> current = {};
    for(int i = 0; i < line.length(); i++) {
        if(line[i] == ':') {
            //End of element name tag
            if (section != "") {
                name = section;
                section = "";
            }
        } else if (line[i] == ' ') {
            //Another element in the current possibility (Queue)
            current.enqueue(section);
            section = "";
        } else if (line[i] == '|') {
            //Spacer for possibilities in this kind of element
            current.enqueue(section);
            section = "";
            grammar[name].add(current);
            current.clear();
        } else if (i == line.length() - 1) {
            //At the end of the line
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

//Main function to be called. Calls functions base on the information given.
//Takes a input stream for grammar rules, a string stores either the kind of element
//    to be generated or the sentence to be checked, and an integer of times
//    times to generate the given kind of element (times > 0).
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> result; //Result to be returned
    Map<string, Vector<Queue<string>>> grammar; //Store grammar rules

    //Read grammar file and store into variable
    string readLine;
    while(getline(input, readLine)) {
        //read each line & analyze the content
        analyzeLine(readLine, grammar);
    }

    if(times > 0) { //Generate elements
        if (grammar[symbol].isEmpty()) {
            //the symbol given is not in the dictionary.
            //Output should just be the given symbol
            for(int i = 0 ; i < times ; i++) {
                result.add(symbol);
            }
        } else {
            //the symbol given is in the dictionary
            //generate accordingly
            for(int i = 0 ; i < times ; i++) {
                result.add(generate(symbol, grammar));
            }
        }
    } else {
        throw("Times need to be larger than zero.");
    }

    return result;
}
