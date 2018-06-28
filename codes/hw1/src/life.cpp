/* 
 * CS106B Assignment 1
 * Game of Life 
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/05/2018
 */
#include <iostream>
#include <fstream>
#include "filelib.h"
#include "console.h"
#include "grid.h"
#include "simpio.h"
using namespace std;

int surCell(const Grid<bool> &cells, int row, int col) {
    int count = 0; //Count for living cells
    if (!cells.inBounds(row, col)) {
        return -1; //Error: row & col given not in bounds.
    }
    for (int r = row - 1 ; r <= row + 1 ; r++ ) {
        for (int c = col - 1 ; c <= col + 1 ; c++) {
            if (cells.inBounds(r,c)) {
                count += cells[r][c]; //counts +1 if cells[r][c] is true
            } 
        }
    }
    count -= cells[r][c]; //Deducting the value of the center cell
    return count;
}   

Grid<bool> calNextGen(const Grid<bool> &currentGen){
    Grid<bool> nextGen(currentGen.numRows, currentGen.numCols);//Defining empty grid to store info of next gen.
    for(int r = 0 ; r < currentGen.numRows ; r++) {
        for(int c = 0 ; c < currentGen.numCols ; c++) {
            switch(surCell(currentGen, r, c)){
                case 2:
                    nextGen[r][c] = currentGen[r][c];
                case 3:
                    nextGen[r][c] = true;
                default:
                    nextGen[r][c] = false; //includes cases when surCell = 0, 1, or 4+
            }
        }
    }
    return nextGen;
}

void animate(const Grid<bool> &currentGen) {

}

int main() {
    int row, col; //Number of rows & columns in the grid.

    cout << "Welcome to the CS 106B/X Game of Life!" << endl;
    cout << "This program simulates the lifecycle of a bacterial colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "* A cell with 1 or fewer neighbors dies." << endl;
    cout << "* Locations with 2 neighbors remain stable." << endl;
    cout << "* Locations with 3 neighbors will create life." << endl;
    cout << "* A cell with 4 or more neighbors dies." << endl;

    ifstream infile;
    promptUserForFile(infile, "Grid input file name?"); //Opening input file
    infile >> numRow >> numCol; //Read row & column number from file
    
    Grid<bool> life(numRow, numCol); //Declaring main grid
    char tempInput; //char for inputing from file
    
    for(int r = 0 ; r < numRow ; r++) {
        for(int c = 0 ; c < numCol ; c++) {
            infile.get(tempInput);
            cout << tempInput;
            life[r][c] = tempInput == "X"; //true = live cell, false = dead cell               
        }
        cout << endl;
    }

    infile.close; //Closing input file

    string interact = getLine("a)nimate, t)ick, q)uit?");
    while(interact != "q") {
        switch(interact) {
            case "t":
                life = calNextGen(life);
                cout << life;
            case "a":
                int rounds = getInteger("How many frames?");
                for(int i = 0 ; i < rounds ; i++) {
                    animate(life);                    
                }
        }
        interact = getLine("a)nimate, t)ick, q)uit?");
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}
