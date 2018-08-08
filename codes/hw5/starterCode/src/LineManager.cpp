/*
 * CS106B Assignment 5-C
 * LineManager.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/08/2018
 *
 * Basic feature:
 * A type that stores all the liens that comprise a webpage.
 */

#include "LineManager.h"
#define LEFT false
#define RIGHT true
using namespace std;

//Helper function for the constructor.
//Build binary search tree form the given lines.
//Takes a node, and a vector of pointers of lines, and build the
//    binary search tree of the lines using recursion.
void LineManager::buildTree(LineManager::Node *&node, const Vector<Line *>& lines) {
    if (lines.size() == 0) return; //The list is empty. No operations needed.
    else if (lines.size() == 1) {
        //Leaf node.
        node = new LineManager::Node;
        node->line = lines[0];
        node->left = nullptr;
        node->right = nullptr;
    } else {
        //Non-leaf node.
        node = new LineManager::Node;
        node->line = lines[lines.size() / 2]; //The middle line at the current node.
        node->left = node->right = nullptr;
        //Divide the vector into two parts and build tree.
        LineManager::buildTree(node->left, lines.subList(0, lines.size() / 2));
        if (lines.size() > lines.size() / 2 + 1) {
            if (lines.size() % 2) LineManager::buildTree(node->right, lines.subList(lines.size() / 2 + 1, lines.size() / 2));
            else  LineManager::buildTree(node->right, lines.subList(lines.size() / 2 + 1, lines.size() / 2 - 1));
        }
    }
}

//Helper function for the destructor.
//Delete the binary search tree & ensures there's no
//    memory leak.
void LineManager::destroyTree(LineManager::Node *&node) {
    if(node == nullptr) return;
    if(node->left != nullptr) LineManager::destroyTree(node->left);
    if(node->right != nullptr) LineManager::destroyTree(node->right);
    delete node;
}

LineManager::LineManager(const Vector<Line *>& lines) {
    root = nullptr;
    buildTree(root, lines);
}

LineManager::~LineManager() {
    destroyTree(root);
}

//Helper function for contentHeight.
//Find the highest or lowest y coordinate for the
//    current tree;
//Takes a node and the side being looked for, and returns
//    a double of the coordinate.
double LineManager::findEstY(LineManager::Node *node, bool side) const {
    if (side == LEFT) {
        if (node->left == nullptr) return node->line->lowY();
        else return LineManager::findEstY(node->left, LEFT);
    } else {
        if (node->right == nullptr) return node->line->highY();
        else return LineManager::findEstY(node->right, RIGHT);
    }
    return 0.0;
}

double LineManager::contentHeight() const {
    return LineManager::findEstY(LineManager::root, RIGHT)
            - LineManager::findEstY(LineManager::root, LEFT);
}

//Helper function for linesInRange.
//Takes two doubles, indicating the range, a node, and a vector of pointers of lines
//    passed by reference to store the results to be found.
void LineManager::inRangeHelper(double lowY, double highY, LineManager::Node *node, Vector<Line *> &result) const {
    if (node->line->highY() < lowY && node->line->lowY() > highY) return; //Line not in range.
    if (node->line->highY() < lowY) {
        //Left side out of range.
        if (node->right != nullptr) {
            LineManager::inRangeHelper(lowY, highY, node->right, result);
        }
    } else if (node->line->lowY() > highY) {
        //Right side out of range.
        if (node->left != nullptr) {
            LineManager::inRangeHelper(lowY, highY, node->left, result);
        }
    } else {
        //Both sides in range.
        if (node->left != nullptr) LineManager::inRangeHelper(lowY, highY, node->left, result);
        result.add(node->line);
        if (node->right != nullptr) LineManager::inRangeHelper(lowY, highY, node->right, result);
    }
}

Vector<Line *> LineManager::linesInRange(double lowY, double highY) const {
    Vector<Line *> result;
    LineManager::inRangeHelper(lowY, highY, LineManager::root, result);
    return result;
}

//Helper function for lineAt.
//Takes a double, indicating the y coordinate being looked at, and a node.
Line* LineManager::lineAtHelper(double y, LineManager::Node *node) const {
    if (node->line->highY() > y) {
        //Rules out the right sub tree.
        if (node->line->lowY() < y) return node->line; //found
        else {
            if (node->left != nullptr) return lineAtHelper(y, node->left);
            else return nullptr; //Line doesn't exist at the given y.
        }
    } else {
        if(node->right != nullptr) return lineAtHelper(y, node->right);
        else return nullptr; //Line doesn't exist at the given y.
    }
    return nullptr;
}

Line* LineManager::lineAt(double y) const {
    return lineAtHelper(y, LineManager::root);
}

