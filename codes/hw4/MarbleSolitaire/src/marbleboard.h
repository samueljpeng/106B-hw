/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file declares a class representing the
 * data stored in the game board for this problem.
 * See marbleboard.cpp for the implementations of each member.
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#ifndef _marbleboard_h
#define _marbleboard_h

#include <string>
#include "grid.h"
#include "observable.h"
#include "marbletypes.h"

/*
 * Enumeration of constants representing various kinds of events that can occur.
 * These are sent to the GUI, which responds to each one accordingly.
 */
enum BoardEvent {
    BOARD_EVENT_CLEAR,
    BOARD_EVENT_LOAD_DEFAULT,
    BOARD_EVENT_LOAD_FROM_FILE,
    BOARD_EVENT_MOVE_MADE
};

class MarbleBoard : public Observable<BoardEvent> {
public:
    static const int BOARD_DIMENSION;   // w/h of internal grid
    static const int N_MARBLES_START;   // number of marbles in a full/default board

    /*
     * Constructs a new board of default size.
     */
    MarbleBoard();

    /*
     * Removes all marbles from the board.
     */
    void clear();

    /*
     * Returns the w/h of this board's grid.
     * Equivalent to BOARD_DIMENSION.
     */
    int dimension() const;

    /*
     * Returns the state of the given board square.
     */
    Marble get(const Coord& coord) const;

    /*
     * Returns the state of the given board square.
     */
    Marble get(int row, int col) const;

    /*
     * Returns the entire grid of states used by this board.
     * This is provided as a convenience so the GUI can make a copy of the grid.
     */
    const Grid<Marble>& grid() const;

    /*
     * Returns true if the given square is within the bounds of the board.
     */
    bool inBounds(const Coord& coord) const;

    /*
     * Returns true if the given square is within the bounds of the board.
     */
    bool inBounds(int row, int col) const;

    /*
     * Returns true if the board is already solved, which will be the case
     * if it has 1 marble or fewer remaining.
     */
    bool isSolved() const;

    /*
     * Returns true if the given move is valid to make on this board in its
     * current state.  That will be the case if the move's starting square holds
     * a marble; if the ending square is an empty square, 2 rows xor columns away;
     * and between the two is a square that also holds a marble to be jumped over.
     * Implicit in the contract of a returned value of 'true' from this function
     * is that the board is not yet solved.
     */
    bool isValidMove(const Move& move) const;

    /*
     * Returns the last move that was made on this board.
     * If no moves have been made, returns a fake move from (0,0) to (0,0).
     */
    const Move& lastMove() const;

    /*
     * Performs a move on this board.
     * Returns true if the move was made successfully, which will be the case
     * if the board is not yet solved and the move is valid according to
     * the current board state.
     */
    bool makeMove(const Move& move);

    /*
     * Returns how many marbles are currently present on the board.
     */
    int marbleCount() const;

    /*
     * Reads board state from a text file.
     * Returns true if successful and false if it failed.
     * Precondition: filename refers to a correctly formatted file.
     */
    bool readBoardFromFile(const std::string& filename);

    /*
     * Sets the state of this board to the default full board state,
     * where every square except the middle one is occupied by a marble.
     */
    void resetToDefault();

private:
    Grid<Marble> _grid;   // internal grid of marble state
    int _marbleCount;     // count of marbles present
    Move _lastMove;       // remembers last move
};

#endif // _marbleboard_h
