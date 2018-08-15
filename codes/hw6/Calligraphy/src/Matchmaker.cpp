/*
 * CS106B Assignment 6-C
 * Matchmaker.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/15/2018
 */
#include "Matchmaker.h"
#include "hashmap.h"
using namespace std;

/**
 * Given a bipartite graph, returns a maximum matching in that graph. If there are multiple
 * equally large maximum matchings, you can return any one of them.
 *
 * @param graph The graph in question.
 * @return One of its maximum matchings.
 */


Matching matchmake(const BipartiteGraph& graph) {
    HashMap<string, HashSet<string>> theGraph;
    //Storing the graph
    HashMap<string, string> right;
    //Correspondence between the right node, and the left it's matching

    Matching result;

    //Putting the given information into our structures
    for (UnorderedPair p : graph.edges) {
        theGraph[p.first].add(p.second);
        theGraph[p.second].add(p.first);
    }
    for (string r : graph.rhs) {
        right.add(r, "");
    }

    int count = 0;
    bool connected;
    //Traverse through all left hand side nodes
    for(string left : graph.lhs) {
        connected = false;
        //Looping though all its possible connections.
        for (string posRight : theGraph[left]) {
            if (right[posRight] == "") {
                right[posRight] = left;
                count ++;
                connected = true;
                break;
            }
        }
        if (connected) continue;
        //Looping thorugh all connections again for breaking
        for (string posRight : theGraph[left]) {
            string tempLeft = right[posRight];
            bool switched = false;
            for (string newPosRight : theGraph[tempLeft]) {
                if (right[newPosRight] == "") {
                    right[newPosRight] = tempLeft;
                    switched = true;
                    break;
                }
            }
            if(switched) right[posRight] = left;
        }
    }

    //Adding result into "result"
    for(string str : right) {
        if(right[str] != "") {
            result.add({str, right[str]});
        }
    }

    return result;
}
