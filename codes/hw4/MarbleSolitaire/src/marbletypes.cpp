/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file defines some useful data types that can be
 * used by your code.  See marbletypes.h for declarations and documentation.
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

#include "marbletypes.h"
#include <sstream>
#include "collections.h"

Coord::Coord(int r, int c)
    : row(r),
      col(c)
{
    // empty
}

std::string Coord::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

bool operator <(const Coord& c1, const Coord& c2) {
    return c1.row < c2.row || (c1.row == c2.row && c1.col < c2.col);
}

bool operator <=(const Coord& c1, const Coord& c2) {
    return c1 < c2 || c1 == c2;
}

bool operator >(const Coord& c1, const Coord& c2) {
    return c2 < c1;
}

bool operator >=(const Coord& c1, const Coord& c2) {
    return c2 > c1 || c2 == c1;
}

bool operator ==(const Coord& c1, const Coord& c2) {
    return c1.row == c2.row && c1.col == c2.col;
}

bool operator !=(const Coord& c1, const Coord& c2) {
    return !(c1 == c2);
}


std::ostream& operator <<(std::ostream& os, const Coord& c) {
    return os << "(" << c.row << "," << c.col << ")";
}

int hashCode(const Coord& coord) {
    return hashCode2(coord.row, coord.col);
}

Move::Move(int sr, int sc, int er, int ec)
    : startRow(sr),
      startCol(sc),
      endRow(er),
      endCol(ec)
{
    // empty
}

std::string Move::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const Move& m) {
    return hashCode4(m.startRow, m.startCol, m.endRow, m.endCol);
}

/* Makes it easy to print out Moves in a nice format. */
std::ostream& operator <<(std::ostream& os, const Move& m) {
   os << "(" << m.startRow << "," << m.startCol << ")"
      << " to "
      << "(" << m.endRow << "," << m.endCol << ")";
   return os;
}

/*
 * Allows Moves to be put into data structures
 * which require sorting like a Map.
 */
bool operator <(const Move& m1, const Move& m2) {
    return stanfordcpplib::collections::compareTo4(
                m1.startRow, m2.startRow,
                m1.startCol, m2.startCol,
                m1.endRow, m2.endRow,
                m1.endCol, m2.endCol) < 0;
}

bool operator <=(const Move& m1, const Move& m2) {
    return m1 < m2 || m1 == m2;
}

bool operator >(const Move& m1, const Move& m2) {
    return m2 < m1;
}

bool operator >=(const Move& m1, const Move& m2) {
    return m1 > m2 || m1 == m2;
}

bool operator ==(const Move& m1, const Move& m2) {
    return stanfordcpplib::collections::compareTo4(
                m1.startRow, m2.startRow,
                m1.startCol, m2.startCol,
                m1.endRow, m2.endRow,
                m1.endCol, m2.endCol) == 0;
}

bool operator !=(const Move& m1, const Move& m2) {
    return !(m1 == m2);
}
