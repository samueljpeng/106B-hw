/*
 * CS106B Assignment 3-A
 * Fractals
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/18/2018
 *
 * Basic feature:
 * Draw fractals including Sierpinski Triangle and
 * Tree with given specifications
 * Advanced Version with Extra Features including:
 *     drawing Sierpinski Carpet pattern
 *     only works with the updated factalgui-extra.cpp, factal.h, and factalgui-extra.h
 */
#include <cmath>
#include <utility>
#include "error.h"
#include "fractals.h"
using namespace std;

void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order);
void treeHelper(GWindow& gw, double x, double y, double size, int order, bool initial, int angle);
void drawTree(GWindow& gw, double x, double y, double size, int order);
void drawCarpet(GWindow& window, double x, double y, double size, int order);

//Takes the windows to draw in, the coordinates for the left-top point of the triangle,
//    the size of the triangle, and the current order of the recursion. Draws a Sierpinski
//    Triangle.
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if (order < 0 || size < 0 || x < 0 || y < 0) {
        throw "Parameters cannot be negative.";
    } else if (order == 0) {
        return; //Should be empty picture
    } else if (order == 1) {
        //base case
        pair<int, int> pt1, pt2; //to store the other two points of the triangle
        //calculate the coordinates of the other two points of the triangle
        pt1 = make_pair(x + size, y);
        pt2 = make_pair(x + size / 2, y + sqrt(3) * size / 2);
        //draw the three lines of the triangle
        window.drawLine(x, y, pt1.first, pt1.second);
        window.drawLine(x, y, pt2.first, pt2.second);
        window.drawLine(pt1.first, pt1.second, pt2.first, pt2.second);
    } else {
        //Recursive case
        //Draw smaller triangles by dividing the given triangle into three parts
        drawSierpinskiTriangle(window, x, y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 2, y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 4, y + sqrt(3) * size / 4, size / 2, order - 1);
    }
}

//Helper function to draw tree
//Takes the window to draw in, the coordinates of the starding point, the size
//    of the drawing, the current order, a bool indicating whether it's the initial case,
//    and the base angle of the current case.
//Draws the tree.
void treeHelper(GWindow& gw, double x, double y, double size, int order, bool initial, int angle) {
    if (order == 1) {
        //Base case
        gw.setColor("#2E8B57"); //Draw with the green color
        if (initial) {
            //if it's the first call, draw one vertical line.
            gw.drawPolarLine(x, y, size, angle);
        } else {
            //if not first call, loop & draw the 7 lines.
            for(int i = -45 ; i <= 45 ; i += 15){
                gw.drawPolarLine(x, y, size, i + angle);
            }
        }
    } else {
        //Recursive case
        if (initial) {
            gw.setColor("#8B7765"); //Draw with the Brown color
            //draw the first line, and save endpoint to pt
            GPoint pt = gw.drawPolarLine(x, y, size, angle);
            //Call recursion to draw the next layer of tree
            treeHelper(gw, pt.getX(), pt.getY(), size / 2, order - 1, false, angle);
        } else {
            for(int i = -45 ; i <= 45 ; i += 15){ //Loop through the 7 branches
                gw.setColor("#8B7765"); //Draw with the Brown color
                //Draw the branch, and save endpoint for recursion call
                GPoint pt = gw.drawPolarLine(x, y, size, i + angle);
                //Call recursion to draw the next layer of tree
                treeHelper(gw, pt.getX(), pt.getY(), size / 2, order - 1, false, i + angle);
            }
        }
    }
}

//Wrapper function for drawing the tree
//Takes the window to draw in, coordinates of the starding point,
//    the size of the tree, and the order of the tree to be drawn.
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    if (order < 0 || size < 0 || x < 0 || y < 0) {
        throw "Parameters cannot be negative.";
    } else if (order == 0) {
        return; //Should be empty picture
    } else {
        //Call helper function to draw the tree
        treeHelper(gw, x + size / 2, y + size, size / 2, order, true, 90);
    }
}

//Helper function to draw Sierpinski Carpet pattern
//Takes the window to draw in, the coordinates of the upper left point, the size
//    of the drawing, and the current order.
//Draws the Sierpinski Carpet pattern.
void carpetHelper(GWindow& gw, double x, double y, double size, int order) {
    int add1 = size / 9;
    int add2 = size / 9 + size / 3;
    int add3 = size / 9 + 2 * size / 3;
    //Draw the rectangles for this call
    gw.drawRect(x + add1, y + add1, size / 9, size / 9);
    gw.drawRect(x + add1, y + add2, size / 9, size / 9);
    gw.drawRect(x + add1, y + add3, size / 9, size / 9);
    gw.drawRect(x + add2, y + add1, size / 9, size / 9);
    gw.drawRect(x + add2, y + add3, size / 9, size / 9);
    gw.drawRect(x + add3, y + add1, size / 9, size / 9);
    gw.drawRect(x + add3, y + add2, size / 9, size / 9);
    gw  .drawRect(x + add3, y + add3, size / 9, size / 9);
    if (order > 1) {
        //call recursive functions
        carpetHelper(gw, x, y, size / 3, order - 1);
        carpetHelper(gw, x + size / 3, y, size / 3, order - 1);
        carpetHelper(gw, x + 2 * size / 3, y, size / 3, order - 1);
        carpetHelper(gw, x, y + size / 3, size / 3, order - 1);
        carpetHelper(gw, x + 2 * size / 3, y + size / 3, size / 3, order - 1);
        carpetHelper(gw, x, y + 2 * size / 3, size / 3, order - 1);
        carpetHelper(gw, x + size / 3, y + 2 * size / 3, size / 3, order - 1);
        carpetHelper(gw, x + 2 * size / 3, y + 2 * size / 3, size / 3, order - 1);
    }
}

//Wrapper function for drawing the Sierpinski Carpet
//Takes the window to draw in, coordinates of the upper left point,
//    the size of the pattern, and the order of the pattern to be drawn.
void drawCarpet(GWindow& window, double x, double y, double size, int order) {
    if (order < 0 || size < 0 || x < 0 || y < 0) throw "Parameters cannot be negative.";
    if (order == 0) return;
    //draw the big Rectangle
    window.setColor("#000000");
    window.drawRect(x, y, size, size);
    //draw the middle Rectangle
    window.drawRect(x + size / 3, y + size / 3, size / 3, size / 3);
    //Call recursion
    carpetHelper(window, x, y, size, order - 1);
}
