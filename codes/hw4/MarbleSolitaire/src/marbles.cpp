// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "grid.h"
#include "vector.h"
#include "marbletypes.h"
#include "compression.h"
#include "marbleutil.h"
using namespace std;

bool solvePuzzle (Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory) {
    Vector<direction> allDirections = {up, down, lef, rig};
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
