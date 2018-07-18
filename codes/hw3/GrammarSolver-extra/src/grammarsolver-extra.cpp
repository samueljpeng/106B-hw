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
 * Advanced version with extra features including:
 *     Robust grammar solver: able to handle excessive white space in file (function analyzeLine).
 *     Grammar checker: check if the given sentence is grammatically correct.
 *                      function possible and grammarCheck, and line (204 - 207)
 *                      input times = -1 to use.
 */
#include "grammarsolver.h"
#include "map.h"
#include "queue.h"
#include "error.h"
#include <cstdlib>
#include <random>
using namespace std;

string generate(string kind, const Map<string, Vector<Queue<string>>> &grammar);
void analyzeLine(string line, Map<string, Vector<Queue<string>>> &grammar, Stack<string> &keys);
Vector<string> decomp(string to);
bool grammarCheck(Stack<Queue<string>> history, Queue<string> current, Vector<string> process,
                  const Vector<string> to, const Map<string, Vector<Queue<string>>> &grammar);
bool possible(const Vector<string> &element, string &id, const Map<string, Vector<Queue<string>>> &grammar,
              Stack<string> &keys);
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

//Takes a string, which is the line to be analyzed, a Map that stores the
//    grammar rules by reference, and a Stack which stores all the keys exist
//    in this grammar by reference. Analyse and add the information of the given line
//    into the Map of grammar rules and Stack of element kinds.
void analyzeLine(string line, Map<string, Vector<Queue<string>>> &grammar, Stack<string> &keys) {
    string section = "", name = "";
    Queue<string> current = {};
    for(int i = 0; i < line.length(); i++) {
        if(line[i] == ':') {
            //End of element name tag
            if (section != "") {
                name = section;
                keys.push(name);
                section = "";
            }
        } else if (line[i] == ' ') {
            //Either another element in this possibility
            //    or excessive spacing
            if(name != "" && section != "") {
                current.enqueue(section);
                section = "";
            }
        } else if (line[i] == '|') {
            //Spacer for possibilities in this kind of element
            if(section != "") {
                current.enqueue(section);
                section = "";
            }
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

//Takes in a line of string, which is the sentence to be decomposed.
//    Returns a Vector of strings, with each word of the sentence stored
//    seperately in the Vector.
Vector<string> decomp(string to) {
    Vector<string> result; //The vector to return
    string temp;
    for(int i = 0; i < to.length(); i++) {
        if (to[i] == ' ') { //End of a word
            if(temp!= "") result.add(temp);
            temp = "";
        } else if (i == to.length() - 1) { //End of the sentence
            temp += to[i];
            result.add(temp);
        } else temp += to[i]; //Add character to temp
    }
    return result; //return the vector
}

//Takes a stack of Queues, storing the history of search, a Queue, which is the current queue
//    being processed, a Vector, which stores the process (already found), another const Vector
//    by reference, storing the target to be found, and a Map by reference, storing the grammar rules.
//Returns true if the sentence if grammatically correct under current situation, and false if not.
bool grammarCheck(Stack<Queue<string>> history, Queue<string> current, Vector<string> process,
                  const Vector<string> to, const Map<string, Vector<Queue<string>>> &grammar) {
    if(process == to) { //Process reached target
        if (history.isEmpty() && current.isEmpty()) return true; //Search ended, grammar correct
        else return false; //Search ended before queue ended, not correct
    }
    if (grammar[current.peek()].isEmpty()) {
        //Endpoint case
        Vector<string> procBackup = process;
        for (string elem : current) {
            if (to[process.size()] != elem){
                //did not match, restoring backup and returning false
                process = procBackup;
                return false;
            } else {
                //matched, add element to the process list
                process.add(elem);
            }
        }
        //matched, going through history ...
        while(true) {
            if(history.isEmpty()) {
                if(process == to) return true; // Found destination
                else return false; // Not a match ...
            }
            Queue<string> temp = history.pop();
            temp.dequeue();
            if(!temp.isEmpty()) return grammarCheck(history, temp, process, to, grammar);
        }
    } else {
        //Element Type Case
        for(Queue<string> q : grammar[current.peek()]) {
            Stack<Queue<string>> temp=history;
            temp.push(current);
            //Adding the current queue to history, and call recursive function
            if (grammarCheck(temp, q, process, to, grammar)) return true;
        }
    }

    return false;
}

//Wrapper function for grammarCheck
//Takes a const Vector by reference, storing the given sentence, a string by reference, to pass
//    back the element kind if found, a const Map by reference, which stores all the grammar rules,
//    and a Stack storing all the element kinds of the grammar.
//Returns true if the sentence if grammatically correct, and false if not.
bool possible(const Vector<string> &element, string &id, const Map<string, Vector<Queue<string>>> &grammar,
              Stack<string> &keys) {
    for(string key : keys) { //Looping through all possible kinds of elements
        for(Queue<string> q : grammar[key]) { //Looping through all possible queues under the given kind
            if(grammarCheck({}, q, {}, element, grammar)) { //Grammatically correct
                id = key;
                return true;
            }
        }
    }
    //Exhaust. Not correct.
    return false;
}

//Main function to be called. Calls functions base on the information given.
//Takes a input stream for grammar rules, a string stores either the kind of element
//    to be generated or the sentence to be checked, and an integer of either times
//    times to generate the given kind of element (times > 0) or check grammar of
//    the given sentence (times = -1).
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> result; //Result to be returned

    Map<string, Vector<Queue<string>>> grammar; //Store grammar rules
    Stack<string> keys; //Store all possible kinds of elements

    //Read grammar file and store into variable
    string readLine;
    while(getline(input, readLine)) {
        //read each line & analyze the content
        analyzeLine(readLine, grammar, keys);
    }

    if (times == -1) { //times = -1 to check if the sentence is grammatically correct
        string wordType; //Pass by reference to store the type of element
        if (possible(decomp(symbol), wordType, grammar, keys)) result.add("This is grammatically correct. It is a " + wordType);
        else result.add("This is not grammatically correct.");
    } else if (times > 0) { //Generate elements
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
        throw("Times needs to be larger than zero, or equals to -1 for grammar check.");
    }

    return result;
}
