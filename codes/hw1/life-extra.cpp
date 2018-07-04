/* 
 * CS106B Assignment 1
 * Game of Life 
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/05/2018
 *
 * Advanced Version with Extra Features
 * including: Random generate first generation (line 53 -> line 69 -> line 248-271)
 *            Support more input (line 56 -> line 273-286 -> line 299)
 *            GUI: Color gets darker the more generations a cell lives.
 *                 (line 32&38&41 -> line 93-113 -> line 158-190 -> line 210)
 *                 Changed grid type from bool (to save memory) to int, which saves the age of a cell
 *                 (how many generations it has lived), its positivity represents if it's alive or dead.
 *                 Only works with modified lifegui.cpp & lifegui.h
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "filelib.h"
#include "console.h"
#include "grid.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void welcomePrint();
//Printing initial lines of information

Grid<int> calNextGen(const Grid<int> &currentGen);
//Calculate the next generation of the given gen.

int surCell(const Grid<int> &cells, int row, int col);
//Calculates number of surrounding living cells

string decToHex(int decimal);
//Convert decimal to hexadecimal

string getColor(int age);
//Get the color for the corresponding age

void printGen(const Grid<int> &grid);
//Print the given generation

void paintGen(const Grid<int> &grid);
//Paint the given generation to GUI

Grid<int> fileRead();
//Read initial information from file

Grid<int> randGen();
//Generate random initial generation

char order(string in);
//turns all input into a, t, or q

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

Grid<int> calNextGen(const Grid<int> &currentGen) {
    Grid<int> nextGen(currentGen.numRows(), currentGen.numCols());
    //Defining empty grid to store info of next gen.

    for(int r = 0 ; r < currentGen.numRows() ; r++) {
        for(int c = 0 ; c < currentGen.numCols() ; c++) {
            if(surCell(currentGen, r, c) == 2) {
                //stays the current status
                if (currentGen[r][c] > 0) {
                    nextGen[r][c] = currentGen[r][c] + 1;
                    //if its alive, its age increse by one
                } else {
                    nextGen[r][c] = currentGen[r][c];
                    //if its dead, it stays dead
                }
            } else if (surCell(currentGen, r, c) == 3) {
                nextGen[r][c] = abs(currentGen[r][c]) + 1;
                //it turns alive, and age increase by one
            } else {
                nextGen[r][c] = -(abs(currentGen[r][c]));
                //it becomes dead
                //includes cases when surCell = 0, 1, or 4+
            }
        }
    }

    return nextGen;
}

int surCell(const Grid<int> &cells, int row, int col) {
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

            if(cells[cRow][cCol] > 0) {
                count++;
            }
        }
    }

    if(cells[row][col] > 0) {
        count--;
    }

    return count;
}

string decToHex(int decimal) {
    if (decimal <  10) {
        return "0" + to_string(decimal);
    } else if (decimal < 16) {
        switch (decimal) {
            case 10:
                return "0A";
            case 11:
                return "0B";
            case 12:
                return "0C";
            case 13:
                return "0D";
            case 14:
                return "0E";
             case 15:
                return "0F";
        }
    } else {
        return decToHex(decimal / 16).substr(1,1) + decToHex(decimal % 16).substr(1,1);
    }
    return "00";
}

string getColor(int age) {
    if (age > 100) {
        return "#9A0000";
    } else {
        int r = 255 - age;
        int gb = 202 - 2 * age;
        return "#" + decToHex(r) + decToHex(gb) + decToHex(gb);
    }
}

void printGen(const Grid<int> &grid) {
    for(int r = 0 ; r < grid.numRows() ; r++) {
        for(int c = 0 ; c < grid.numCols() ; c++) {
            if (grid[r][c] > 0) {
                cout << "X";
            } else {
                cout << "-";
            }
        }
        cout<<endl;
    }
}

void paintGen(const Grid<int> &grid) {
    LifeGUI::clear();
    for(int r = 0 ; r < grid.numRows() ; r++) {
        for(int c = 0 ; c < grid.numCols() ; c++) {
            if (grid[r][c] > 0) {
                LifeGUI::fillCell(r, c, getColor(grid[r][c]));
            }
        }
    }
    LifeGUI::repaint();
}

Grid<int> fileRead() {
    int numRow, numCol; //Number of rows & columns in the grid.

    ifstream infile;
    promptUserForFile(infile, "Grid input file name?"); //Opening input file
    infile >> numRow >> numCol; //Read row & column number from file

    LifeGUI::resize(numRow, numCol);
    LifeGUI::clear();
    LifeGUI::repaint();

    Grid<int> life(numRow, numCol); //Declaring main grid
    char tempInput; //char for inputing from file

    infile.get(tempInput); //Input the linebreak

    for(int r = 0 ; r < numRow ; r++) {
        for(int c = 0 ; c < numCol ; c++) {
            infile.get(tempInput);
            life[r][c] = (tempInput == 'X'); //true = live cell, false = dead cell
        }
        infile.get(tempInput);//input the linebreak
    }

    cout<<life;

    infile.close(); //Closing input file

    return life;
}

Grid<int> randGen() {
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

    Grid<int> life(numRow, numCol);

    for(int r = 0 ; r < numRow ; r++) {
        for(int c = 0 ; c < numCol ; c++) {
            life[r][c] = rand() % 2;
        }
    }

    return life;
}

char order(string in){
    if (in == "animate" || in == "Animate" || in == "ANIMATE" || in == "a)nimate"
            || in == "a" || in == "A") {
        return 'a';
    } else if (in == "tick" || in == "Tick" || in == "TICK" || in == "t)ick"
               || in == "t" || in == "T") {
        return 't';
    } else if (in == "quit" || in == "Quit" || in == "QUIT" || in == "q)uit"
               || in == "q" || in == "Q") {
        return 'q';
    } else {
        return 'e';
    }
}

void game(bool random) {
    Grid<int> life;

    if (random) {
        life = randGen();
    } else {
        life = fileRead();
    }
    printGen(life);
    paintGen(life);

    char interact = order(getLine("a)nimate, t)ick, q)uit?"));

    while(interact != 'q') {
        if (interact == 't') {
            life = calNextGen(life);
            printGen(life);
            paintGen(life);
        } else if (interact == 'a') {
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
        interact = order(getLine("a)nimate, t)ick, q)uit?"));
    }
}
