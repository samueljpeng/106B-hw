/*
 * CS106B Assignment 3-A
 * Fractals
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/18/2018
 *
 * Basic Version without Extra Features
 *
 */
#include <cmath>
#include <utility>
#include "error.h"
#include "fractals.h"
using namespace std;

void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if (order < 0 || size < 0 || x < 0 || y < 0) {
        throw "Parameters cannot be negative.";
    } else if (order == 0) {
        return;
    } else if (order == 1) {
        //base case
        pair<int, int> pt1, pt2;
        pt1 = make_pair(x + size, y);
        pt2 = make_pair(x + size / 2, y + sqrt(3) * size / 2);
        window.drawLine(x, y, pt1.first, pt1.second);
        window.drawLine(x, y, pt2.first, pt2.second);
        window.drawLine(pt1.first, pt1.second, pt2.first, pt2.second);
    } else {
        drawSierpinskiTriangle(window, x, y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 2, y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 4, y + sqrt(3) * size / 4, size / 2, order - 1);
    }
}

void treeHelper(GWindow& gw, double x, double y, double size, int order, bool initial, int angle) {
    if (order == 1) {
        gw.setColor("#2E8B57");
        if (initial) {
            gw.drawPolarLine(x, y, size, angle);
        } else {
            for(int i = -45 ; i <= 45 ; i += 15){
                gw.drawPolarLine(x, y, size, i + angle);
            }
        }
    } else {
        if (initial) {
            gw.setColor("#8B7765");
            GPoint pt = gw.drawPolarLine(x, y, size, angle);
            treeHelper(gw, pt.getX(), pt.getY(), size / 2, order - 1, false, angle);
        } else {
            for(int i = -45 ; i <= 45 ; i += 15){
                gw.setColor("#8B7765");
                GPoint pt = gw.drawPolarLine(x, y, size, i + angle);
                treeHelper(gw, pt.getX(), pt.getY(), size / 2, order - 1, false, i + angle);
            }
        }
    }
}

void drawTree(GWindow& gw, double x, double y, double size, int order) {
    if (order < 0 || size < 0 || x < 0 || y < 0) {
        throw "Parameters cannot be negative.";
    } else {
        treeHelper(gw, x + size / 2, y + size, size / 2, order, true, 90);
    }
}
