/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file defines a function for "compressing" a marble
 * board into a small value that can be stored in a set or other collection.
 * This can help you more efficiently prune the decision tree in your algorithm.
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#include "compression.h"

static Observable<int> compressEvent;
static int compressCount = 0;

BoardEncoding compressBoard(const Grid<Marble>& board) {
    static const int COMPRESS_EVENT_COUNT = 10000;

    // set all bits in encoding to 0
    BoardEncoding encoding = 0;
    int place = 0;
    for (int r = 0, nr = board.numRows(), nc = board.numCols(); r < nr; r++) {
        for (int c = 0; c < nc; c++) {
            if (board[r][c] == MARBLE_OCCUPIED) {
                // turns on the ith bit where i == place, so each marble
                // (occupied vs. empty) is encoded by 1 bit
                encoding |= (1 << place);
                place++;
            } else if (board[r][c] == MARBLE_EMPTY) {
                place++;
            }
        }
    }

    compressCount++;
    if (compressCount % COMPRESS_EVENT_COUNT == 0) {
        compressEvent.notifyObservers(compressCount);
    }

    return encoding;
}

void listenToCompressEvents(Observer<int>* obs) {
    compressEvent.addObserver(obs);
}

void resetCompressCount() {
    compressCount = 0;
}
