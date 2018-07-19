/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file declares utility functions that will help you
 * implement your recursive solvePuzzle function.
 * See marbleutil.cpp for definitions of each function.
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#ifndef _marbleutil_h
#define _marbleutil_h

#include "grid.h"
#include "vector.h"
#include "marbletypes.h"

// Provided helper functions (do not edit)

/*
 * This examines the provided board and returns a Vector of
 * next moves that are possible for this board.
 */
Vector<Move> findPossibleMoves(const Grid<Marble>& board);

/*
 * Performs the specified move on the board.
 * Precondition: this move must be valid.
 */
void makeMove(const Move& move, Grid<Marble>& board);

/*
 * This is the function you need to write.
 */
bool solvePuzzle(Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory);

/*
 * Undoes the specified move on the board.
 * Note: it's important to realize that this method is
 * necessary because you have to replace the jumped marble
 * so you can't just call makeMove() with the start and end
 * locations swapped.
 */
void undoMove(const Move& move, Grid<Marble>& board);

#endif // _marbleutil_h
