#include "LineManager.h"
#define LEFT false
#define RIGHT true
using namespace std;

void LineManager::buildTree(LineManager::Node *&node, const Vector<Line *>& lines) {
    if (lines.size() == 0) return;
    else if (lines.size() == 1) {
        node = new LineManager::Node;
        node->line = lines[0];
        node->left = nullptr;
        node->right = nullptr;
    } else {
        node = new LineManager::Node;
        node->line = lines[lines.size() / 2];
        node->left = node->right = nullptr;
        LineManager::buildTree(node->left, lines.subList(0, lines.size() / 2));
        if (lines.size() > lines.size() / 2 + 1) {
            if (lines.size() % 2) LineManager::buildTree(node->right, lines.subList(lines.size() / 2 + 1, lines.size() / 2));
            else  LineManager::buildTree(node->right, lines.subList(lines.size() / 2 + 1, lines.size() / 2 - 1));
        }
    }
}

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

void LineManager::inRangeHelper(double lowY, double highY, LineManager::Node *node, Vector<Line *> &result) const {
    if (node->line->highY() < lowY && node->line->lowY() > highY) return;
    if (node->line->highY() < lowY) {
        if (node->right != nullptr) {
            LineManager::inRangeHelper(lowY, highY, node->right, result);
        }
    } else if (node->line->lowY() > highY) {
        if (node->left != nullptr) {
            LineManager::inRangeHelper(lowY, highY, node->left, result);
        }
    } else {
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

Line* LineManager::lineAtHelper(double y, LineManager::Node *node) const {
    if (node->line->highY() > y) {
        if (node->line->lowY() < y) return node->line;
        else {
            if (node->left != nullptr) return lineAtHelper(y, node->left);
            else return nullptr;
        }
    } else {
        if(node->right != nullptr) return lineAtHelper(y, node->right);
        else return nullptr;
    }
    return nullptr;
}

Line* LineManager::lineAt(double y) const {
    return lineAtHelper(y, LineManager::root);
}

