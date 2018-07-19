/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file declares some useful data types that can be
 * used by your code.  Specifically, we provide:
 *
 * - Coord : represents a row/col location on a board
 * - Move  : represents a move from a starting row/col to an ending row/col on a board
 * - Marble: enumeration of states of each board square
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/11/02
 * - added comparison operators for Coord and Move
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#ifndef _marbletypes_h
#define _marbletypes_h

#include <iostream>
#include "hashcode.h"

/*
 * Represents a row/col location on a board.
 */
struct Coord {
    /* Constructs a new coordinate. */
    Coord(int r = 0, int c = 0);

    /* Returns a string representation such as "(3,4)". */
    std::string toString() const;

    int row;
    int col;
};

/*
 * Returns a hash code for a coordinate so that they can be stored in a HashSet/Map.
 */
int hashCode(const Coord& coord);

/*
 * Compares two coordinates. Also allows Coords to be stored in a Set/Map.
 */
bool operator <(const Coord& c1, const Coord& c2);
bool operator <=(const Coord& c1, const Coord& c2);
bool operator >(const Coord& c1, const Coord& c2);
bool operator >=(const Coord& c1, const Coord& c2);
bool operator ==(const Coord& c1, const Coord& c2);
bool operator !=(const Coord& c1, const Coord& c2);

/*
 * Prints a Coord to the given output stream in a format such as "(3,4)".
 */
std::ostream& operator <<(std::ostream& os, const Coord& c);

/*
 * Constants for all states a board square can have.
 */
enum Marble {
    MARBLE_EMPTY,    // an empty space on the board grid
    MARBLE_OCCUPIED, // a space on the board grid which has a marble on it
    MARBLE_INVALID   // a spot in the board grid which is out of bounds
};

/*
 * This move structure is a simple way to represent a move from one location
 * to another location as represented by the start/end row/column values.
 */
struct Move {
    /* Constructs a new move. */
    Move(int sr = 0, int sc = 0, int er = 0, int ec = 0);

    /* Returns a string representation such as "(3,4) to (1,4)". */
    std::string toString() const;

    int startRow;
    int startCol;
    int endRow;
    int endCol;
};

/*
 * Returns a hash code for a move so that they can be stored in a HashSet/Map.
 */
int hashCode(const Move& m);

/*
 * Compares two moves. Also allows Moves to be stored in a Set/Map.
 */
bool operator <(const Move& m1, const Move& m2);
bool operator <=(const Move& m1, const Move& m2);
bool operator >(const Move& m1, const Move& m2);
bool operator >=(const Move& m1, const Move& m2);
bool operator ==(const Move& m1, const Move& m2);
bool operator !=(const Move& m1, const Move& m2);

/*
 * Prints a Move to the given output stream in a format such as "(3,4) to (1,4)".
 */
std::ostream& operator <<(std::ostream& os, const Move& m);

#endif // _marbletypes_h
