/*
 * CS106B Assignment 6-A
 * WaterLevel.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/15/2018
 */
#include "WaterLevel.h"
using namespace std;

/**
 * Given a floodplain and an altitude, returns a Grid<bool> indicating whether each cell
 * is under water. Cells under water are marked true; cells above water are marked false.
 *
 * Remember that water can only flow in cardinal directions (up, down, left, and right),
 * not diagonally.
 *
 * @param floodplain The plain to flood.
 * @param height The water height, in meters.
 * @return A Grid indicating which cells are flooded.
 */

//Can be considered a weighted graph
//Use DFS to solve the problem

//Helper function for simulateFlood, using DFS to find flooded areas.
//Takes a const Floodplain, a double storing the sealevel height,
//    a GridLocation loc storing current location, and a Grid of
//    bool by reference, storing the result.
void floodHelper(const Floodplain& flood, double height, GridLocation loc, Grid<bool> &result) {
    if (flood.heights[loc.row][loc.col] <= height) {
        if (result[loc.row][loc.col] == true) return; //Already visited

        result[loc.row][loc.col] = true; //Marking current loc
        GridLocation nextLoc;
        if (loc.row > 0) {
            nextLoc.row = loc.row - 1;
            nextLoc.col = loc.col;
            floodHelper(flood, height, nextLoc, result);
        }
        if (loc.col > 0) {
            nextLoc.row = loc.row;
            nextLoc.col = loc.col - 1;
            floodHelper(flood, height, nextLoc, result);
        }
        if (loc.row < flood.heights.numRows() - 1) {
            nextLoc.row = loc.row + 1;
            nextLoc.col = loc.col;
            floodHelper(flood, height, nextLoc, result);
        }
        if (loc.col < flood.heights.numCols() - 1) {
            nextLoc.row = loc.row;
            nextLoc.col = loc.col + 1;
            floodHelper(flood, height, nextLoc, result);
        }
    }
}

Grid<bool> simulateFlood(const Floodplain& floodplain, double height) {
    Grid<bool> result;
    result.resize(floodplain.heights.numRows(), floodplain.heights.numCols(), false);
    for (GridLocation location : floodplain.waterSources) {
        //Traverse through all waterSouces
        floodHelper(floodplain, height, location, result);
    }
    return result;
}
