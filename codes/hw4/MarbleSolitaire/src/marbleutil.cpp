/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file defines utility functions that will help you
 * implement your recursive solvePuzzle function.
 * See marbleutil.h for declarations and documentation of each function.
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#include "marbleutil.h"
#include <iostream>
#include "random.h"
#include "shuffle.h"

// set to true to shuffle up the moves returned by findPossibleMoves
static const bool RANDOMIZED_POSSIBLE_MOVES = true;

void makeMove(const Move& move, Grid<Marble>& board) {
    int jumpedRow = move.startRow + (move.endRow - move.startRow) / 2;
    int jumpedCol = move.startCol + (move.endCol - move.startCol) / 2;
    board[move.startRow][move.startCol] = MARBLE_EMPTY;
    board[move.endRow][move.endCol] = MARBLE_OCCUPIED;
    board[jumpedRow][jumpedCol] = MARBLE_EMPTY;
}

void undoMove(const Move& move, Grid<Marble>& board) {
    int jumpedRow = move.startRow + (move.endRow - move.startRow) / 2;
    int jumpedCol = move.startCol + (move.endCol - move.startCol) / 2;
    board[move.startRow][move.startCol] = MARBLE_OCCUPIED;
    board[move.endRow][move.endCol] = MARBLE_EMPTY;
    board[jumpedRow][jumpedCol] = MARBLE_OCCUPIED;
}

Vector<Move> findPossibleMoves(const Grid<Marble>& board) {
    Vector<Move> moves;
    for (int row = 0, nr = board.numRows(), nc = board.numCols(); row < nr; row++) {
        for (int col = 0; col < nc; col++) {
            if (board[row][col] == MARBLE_EMPTY) {
                if (row - 2 >= 0 && board[row - 2][col] == MARBLE_OCCUPIED
                        && board[row - 1][col] == MARBLE_OCCUPIED) {
                    moves.add(Move(row - 2, col, row, col));
                }
                if (row + 2 < nr && board[row + 2][col] == MARBLE_OCCUPIED
                        && board[row + 1][col] == MARBLE_OCCUPIED) {
                    moves.add(Move(row + 2, col, row, col));
                }
                if (col - 2 >= 0 && board[row][col - 2] == MARBLE_OCCUPIED
                        && board[row][col - 1] == MARBLE_OCCUPIED) {
                    moves.add(Move(row, col - 2, row, col));
                }
                if (col + 2 < nc && board[row][col + 2] == MARBLE_OCCUPIED
                        && board[row][col + 1] == MARBLE_OCCUPIED) {
                    moves.add(Move(row, col + 2, row, col));
                }
            }
        }
    }
    if (RANDOMIZED_POSSIBLE_MOVES) {
        shuffle(moves);
    }
    return moves;
}
