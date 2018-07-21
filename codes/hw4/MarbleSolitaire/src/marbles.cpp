// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "grid.h"
#include "vector.h"
#include "marbletypes.h"
#include "compression.h"
#include "marbleutil.h"
using namespace std;

bool solvePuzzle(Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory) {
    if(marbleCount == 1 && board[3][3] == MARBLE_OCCUPIED) return true;
    for(int i = 0; i < board.numRows(); i++) {
        for(int j = 1; i < board.numCols(); i++) {
            if(board[i][j] == MARBLE_EMPTY) {

            }
        }
    }
    return false;
}
