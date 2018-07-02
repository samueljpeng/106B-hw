/* 
 * CS106B Assignment 1
 * Game of Life 
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/05/2018
 *
 * Advanced Version with Extra Features
 * including: Random generate first generation
 *
 */

#include <iostream>
#include <fstream>
#include "filelib.h"
#include "console.h"
#include "grid.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void welcomePrint();
//Printing initial lines of information

Grid<bool> calNextGen(const Grid<bool> &currentGen);
//Calculate the next generation of the given gen.

int surCell(const Grid<bool> &cells, int row, int col);
//Calculates number of surrounding living cells

void printGen(const Grid<bool> &grid);
//Print the given generation

void printGen(const Grid<bool> &grid);
//Paint the given generation to GUI

Grid<bool> fileRead();
//Read initial information from file

Grid<bool> randGen();
//Generate random initial generation

void game(bool random);
//Main funtion to play the game

int main() {

    welcomePrint();

    LifeGUI::initialize();

    do {
        game(getYesOrNo("Randomly generate initial generation? (y/n)"));
    } while (getYesOrNo("Load another file? (y/n)"));

    LifeGUI::shutdown();

    cout << "Have a nice Life!" << endl;
    return 0;
}

void welcomePrint(){
    cout << "Welcome to the CS 106B/X Game of Life!" << endl;
    cout << "This program simulates the lifecycle of a bacterial colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "* A cell with 1 or fewer neighbors dies." << endl;
    cout << "* Locations with 2 neighbors remain stable." << endl;
    cout << "* Locations with 3 neighbors will create life." << endl;
    cout << "* A cell with 4 or more neighbors dies." << endl;
    cout << endl;
}

Grid<bool> calNextGen(const Grid<bool> &currentGen) {
    Grid<bool> nextGen(currentGen.numRows(), currentGen.numCols());
    //Defining empty grid to store info of next gen.

    for(int r = 0 ; r < currentGen.numRows() ; r++) {
        for(int c = 0 ; c < currentGen.numCols() ; c++) {
            if(surCell(currentGen, r, c) == 2) {
                nextGen[r][c] = currentGen[r][c];
            } else if (surCell(currentGen, r, c) == 3) {
                nextGen[r][c] = true;
            } else {
                nextGen[r][c] = false;
                //includes cases when surCell = 0, 1, or 4+
            }
        }
    }

    return nextGen;
}

int surCell(const Grid<bool> &cells, int row, int col) {
    int count = 0;
    //Count for living cells

    if (!cells.inBounds(row, col)) {
        return -1; //Error: row & col given not in bounds.
    }

    int cRow, cCol; //Use for location for calculation, added for edge scenarios
    for (int r = row - 1 ; r <= row + 1 ; r++ ) {
        for (int c = col - 1 ; c <= col + 1 ; c++) {
            if (r == -1) {
                cRow = cells.numRows() - 1;
            } else if (r == cells.numRows()) {
                cRow = 0;
            } else {
                cRow = r;
            }

            if (c == -1) {
                cCol = cells.numCols() - 1;
            } else if (c == cells.numCols()) {
                cCol = 0;
            } else {
                cCol = c;
            }

            count += cells[cRow][cCol];
        }
    }

    count -= cells[row][col]; //Deducting the value of the center cell
    return count;
}

void printGen(const Grid<bool> &grid) {
    for(int r = 0 ; r < grid.numRows() ; r++) {
        for(int c = 0 ; c < grid.numCols() ; c++) {
            if (grid[r][c]) {
                cout << "X";
            } else {
                cout << "-";
            }
        }
        cout<<endl;
    }
}

void paintGen(const Grid<bool> &grid) {
    LifeGUI::clear();
    for(int r = 0 ; r < grid.numRows() ; r++) {
        for(int c = 0 ; c < grid.numCols() ; c++) {
            if (grid[r][c]) {
                LifeGUI::fillCell(r, c);
            }
        }
    }
    LifeGUI::repaint();
}

Grid<bool> fileRead() {
    int numRow, numCol; //Number of rows & columns in the grid.

    ifstream infile;
    promptUserForFile(infile, "Grid input file name?"); //Opening input file
    infile >> numRow >> numCol; //Read row & column number from file

    LifeGUI::resize(numRow, numCol);
    LifeGUI::clear();
    LifeGUI::repaint();

    Grid<bool> life(numRow, numCol); //Declaring main grid
    char tempInput; //char for inputing from file

    infile.get(tempInput); //Input the linebreak

    for(int r = 0 ; r < numRow ; r++) {
        for(int c = 0 ; c < numCol ; c++) {
            infile.get(tempInput);
            life[r][c] = (tempInput == 'X'); //true = live cell, false = dead cell
        }
        infile.get(tempInput);//input the linebreak
    }

    infile.close(); //Closing input file

    return life;
}

Grid<bool> randGen() {
    int numRow, numCol;
    //Number of rows & columns in the grid.

    do {
        numRow = rand() % 50;
        numCol = rand() % 50;
    } while(numRow == 0 || numCol == 0);
    //Make sure the row & col number is not 0

    LifeGUI::resize(numRow, numCol);
    LifeGUI::clear();
    LifeGUI::repaint();

    Grid<bool> life(numRow, numCol);

    for(int r = 0 ; r < numRow ; r++) {
        for(int c = 0 ; c < numCol ; c++) {
            life[r][c] = rand() % 2;
        }
    }

    return life;
}

void game(bool random) {
    Grid<bool> life;

    if (random) {
        life = randGen();
    } else {
        life = fileRead();
    }
    printGen(life);
    paintGen(life);

    string interact = getLine("a)nimate, t)ick, q)uit?");

    while(interact != "q" && interact != "Q") {
        if (interact == "t" || interact == "T") {
            life = calNextGen(life);
            printGen(life);
            paintGen(life);
        } else if (interact == "a" || interact == "A") {
            int rounds = getInteger("How many frames?");
            for(int i = 0 ; i < rounds ; i++) {
                pause(100);
                clearConsole();
                life = calNextGen(life);
                printGen(life);
                paintGen(life);
            }
        } else {
            cout << "Invalid choice; please try again." << endl;
        }
        interact = getLine("a)nimate, t)ick, q)uit?");
    }
}
