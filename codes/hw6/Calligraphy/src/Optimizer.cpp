/*
 * CS106B Assignment 6-B
 * Optimizer.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/15/2018
 *
 * Basic Feature:
 * Finding the tasks that can decresase the total time
 * if its time is decreased.
 */
#include "TaskGroup.h"
#include "hashset.h"
#include "queue.h"
using namespace std;

struct Node{
    int maxPreReq; //Maximum required time, including the current node
    HashSet<Node *> preReq; //The pre-required nodes that has the maximum required time
    Task *task; //Point to the corresponding task
};
HashMap<Task *, Node *> theMap; //Map that finds the Node * given the Task *

//Helper function for optimizationCandidatesFor
void optHelper(const TaskGroup& process, Task* t, Node* &Max, bool &single) {
    if (theMap[t]->maxPreReq == -1) {
        //Haven't traversed yet
        if (t->dependsOn.isEmpty()) {
            //First node in the chain
            theMap[t]->maxPreReq = t->duration.length();
        } else {
            //Does have tasks it depends on
            for (Task *newT : t->dependsOn) {
                optHelper(process, newT, Max, single);
                if (theMap[t]->maxPreReq < theMap[newT]->maxPreReq + t->duration.length()) {
                    //Time required longer than current max time
                    theMap[t]->maxPreReq = theMap[newT]->maxPreReq + t->duration.length();
                    theMap[t]->preReq.clear();
                    theMap[t]->preReq.add(theMap[newT]);
                } else if (theMap[t]->maxPreReq == theMap[newT]->maxPreReq + t->duration.length()) {
                    //Time required equals current max time
                    theMap[t]->preReq.add(theMap[newT]);
                }
            }
        }
        if (Max == nullptr || Max->maxPreReq < theMap[t]->maxPreReq) {
            single = true;
            Max = theMap[t];
        } else if (Max->maxPreReq == theMap[t]->maxPreReq) single = false;
    }
}

//Helper function for optimizationCandidatesFor
//Adding the given information into the HashSet result
//    passed in by reference
void addToRes(Node* node, HashSet<Task *> &result) {
    result.add(node->task);
    for (Node *n : node->preReq) {
        addToRes(n, result);
    }
}

HashSet<Task *> optimizationCandidatesFor(const TaskGroup& process) {

    HashSet<Task *> result;

    theMap.clear();

    //Create Nodes based on the Tasks given
    for (Task *t : process) {
        Node *node = new Node;
        node->task = t;
        node->preReq.clear();
        node->maxPreReq = -1;
        theMap[t] = node;
    }

    Node* maxNode = nullptr;
    bool singleMax = false;

    //Call helper function for all tasks
    for (Task *t : process) {
        if (theMap[t]->maxPreReq == -1) {
            optHelper(process, t, maxNode, singleMax);
        }
    }

    //More than one longest tracks exist
    if(!singleMax) return result;

    //Adding information to result
    addToRes(maxNode, result);

    //Clearing memory
    for (Task *t : theMap) {
        delete theMap[t];
    }

    return result;
}
