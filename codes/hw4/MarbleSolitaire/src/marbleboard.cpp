/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file defines the members of a class representing the
 * data stored in the game board for this problem.
 * See marbleboard.h for class declaration and documentation.
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/10/22
 * - removed cout statements
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#include "marbleboard.h"
#include <fstream>
#include <iostream>
#include "filelib.h"
#include "random.h"

// define constants
const int MarbleBoard::BOARD_DIMENSION = 7;
const int MarbleBoard::N_MARBLES_START = 32;

MarbleBoard::MarbleBoard()
    : _marbleCount(0),
      _lastMove()
{
    // initialize grid
    _grid.resize(BOARD_DIMENSION, BOARD_DIMENSION);
    resetToDefault();
}

void MarbleBoard::clear() {
    _grid.fill(MARBLE_EMPTY);
    _lastMove = Move();
    notifyObservers(BOARD_EVENT_CLEAR);
}

int MarbleBoard::dimension() const {
    return _grid.numRows();
}

Marble MarbleBoard::get(const Coord& coord) const {
    return _grid[coord.row][coord.col];
}

Marble MarbleBoard::get(int row, int col) const {
    return _grid[row][col];
}

const Grid<Marble>& MarbleBoard::grid() const {
    return _grid;
}

bool MarbleBoard::inBounds(const Coord& coord) const {
    return _grid.inBounds(coord.row, coord.col);
}

bool MarbleBoard::inBounds(int row, int col) const {
    return _grid.inBounds(row, col);
}

bool MarbleBoard::isSolved() const {
    return _marbleCount <= 1;
}

/*
 * Checks whether the given move is valid on this board.
 * A move is valid if:
 * 1) A marble occupies the start location.
 * 2) The end location is empty.
 * 3) A marble occupies the space in between the start and end locations.
 * 4) The start and end locations are in an orthogonal direction and
 *    exactly 2 spaces away (i.e. no diagonal moves)
 * If all 3 of these properties are true, then this method will
 * return that the move is valid.
 */
bool MarbleBoard::isValidMove(const Move& move) const {
    // start/end must be on the board, start must be a marble, end must be empty
    if (!inBounds(move.startRow, move.startCol)
            || !inBounds(move.endRow, move.endCol)
            || _grid[move.startRow][move.startCol] != MARBLE_OCCUPIED
            || _grid[move.endRow][move.endCol] != MARBLE_EMPTY) {
        return false;
    }

    // move must be +/- 2 rows xor 2 columns
    int dr = abs(move.endRow - move.startRow);
    int dc = abs(move.endCol - move.startCol);
    if (!((dr == 2 && dc == 0) || (dr == 0 && dc == 2))) {
        return false;
    }

    // square between start and end must contain a marble (that is being jumped)
    int jumpedRow = move.startRow + (move.endRow - move.startRow) / 2;
    int jumpedCol = move.startCol + (move.endCol - move.startCol) / 2;
    return (_grid[jumpedRow][jumpedCol] == MARBLE_OCCUPIED);
}

const Move& MarbleBoard::lastMove() const {
    return _lastMove;
}

bool MarbleBoard::makeMove(const Move& move) {
    if (!isValidMove(move)) {
        return false;
    }

    // figure out which marble is being jumped over; remove it
    int jumpedRow = move.startRow + (move.endRow - move.startRow) / 2;
    int jumpedCol = move.startCol + (move.endCol - move.startCol) / 2;

    Marble temp = _grid[move.startRow][move.startCol];
    _grid[move.startRow][move.startCol] = _grid[move.endRow][move.endCol];
    _grid[move.endRow][move.endCol] = temp;
    _grid[jumpedRow][jumpedCol] = MARBLE_EMPTY;

    _marbleCount--;
    _lastMove = move;
    notifyObservers(BOARD_EVENT_MOVE_MADE);
    return true;
}

int MarbleBoard::marbleCount() const {
    return _marbleCount;
}

/*
 * Reads in the board from the file and populates the board along with
 * the set of valid end points.
 * Precondition: filename refers to a correctly formatted file.
 * Returns true if successful or false if not.
 */
bool MarbleBoard::readBoardFromFile(const std::string& filename) {
    std::ifstream file;
    file.open(filename);
    if (file.fail()) {
        return false;
    }

    // skip comments at top of file
    std::string line = "";
    do {
        getline(file, line);
    }
    while (!file.fail() && !line.empty() && line[0] == '#');

    // read dimensions
    int row = stringToInteger(line);
    getline(file, line);
    int col = stringToInteger(line);

    // read board squares
    _marbleCount = 0;
    for (int r = 0; r < row; r++) {
        getline(file, line);
        for (int c = 0; c < col; c++) {
            if(line[c] == 'X') {
                _grid[r][c] = MARBLE_INVALID;
            } else if(line[c] == '-') {
                _grid[r][c] = MARBLE_EMPTY;
            } else if(line[c] == 'O') {
                _marbleCount++;
                _grid[r][c] = MARBLE_OCCUPIED;
            }
        }
    }

    notifyObservers(BOARD_EVENT_LOAD_FROM_FILE);
    return true;
}

/*
 * Populates the given board with the default board configuration
 * and only has the position (3,3) as a valid end point for the game.
 */
void MarbleBoard::resetToDefault() {
    for (int row = 0; row < _grid.numRows(); row++) {
        for (int col = 0; col < _grid.numCols(); col++) {
            if ((row < 2 && col < 2) || (row < 2 && col > 4)
                    || (row > 4 && col < 2) || (row > 4 && col > 4)) {
                _grid[row][col] = MARBLE_INVALID;
            } else {
                _grid[row][col] = MARBLE_OCCUPIED;
            }
        }
    }

    // remove the very center marble
    _grid[3][3] = MARBLE_EMPTY;
    _marbleCount = MarbleBoard::N_MARBLES_START;

    notifyObservers(BOARD_EVENT_LOAD_DEFAULT);
}
