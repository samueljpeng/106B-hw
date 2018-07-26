/*
 * CS106B Assignment 4-A
 * Marbles
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 07/26/2018
 *
 * Basic feature:
 * Find a solution to the given board of marble solitaire game
 */

#include "grid.h"
#include "vector.h"
#include "marbletypes.h"
#include "compression.h"
#include "marbleutil.h"
using namespace std;

bool solvePuzzle (Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory);

//Finding a solution to the given board of marble solitaire game.
//Takes a Grid of Marble, which stores the status of the board, an int, storing the
//    number of marbles on the board, and a Vector of Move, storing the history
//    of the moves to solve the game.
//Returns true when the solution exists, and false when the given board can't be solved.
bool solvePuzzle (Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory) {
    if(marbleCount == 1) return true;
    for(int i = 0; i < board.numRows(); i++) {
        for(int j = 0; j < board.numCols(); j++) {
            for(Move m : findPossibleMoves(board)) {
                makeMove(m, board);
                moveHistory.add(m);
                if (solvePuzzle(board, marbleCount - 1, moveHistory)) return true;
                moveHistory.pop_back();
                undoMove(m, board);
            }
        }
    }
    return false;
}
