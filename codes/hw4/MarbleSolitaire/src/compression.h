/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file declares a function for "compressing" a marble
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

#ifndef _compression_h
#define _compression_h

#include "grid.h"
#include "observable.h"
#include "marbletypes.h"

// alias the type uint32_t as "BoardEncoding" as a layer of thin abstraction for student code
typedef uint32_t BoardEncoding;

/*
 * Takes in a Marble Board and encodes it in a 32-bit unsigned
 * integer. This is done to save memory since a 7x7 Grid takes
 * up much more space than 32 bits. By encoding the boards,
 * we can store millions of boards in memory without slowing
 * down the program.
 *
 * Precondition: Board must have at most 32 valid positions.
 *
 * Returns a uint32_t variable which is an unsigned integer that
 * takes up exactly 32 bits. "unsigned" means that the integer doesn't
 * try to represent negative numbers so instead of being able to hold
 * numbers between -2^31 to 2^31 it can now hold numbers between
 * 0 to 2^32. This is a special integer type that is available from
 * the std namespace.
 */
BoardEncoding compressBoard(const Grid<Marble>& board);

/*
 * An event that helps our GUI watch calls to the compressBoard function.
 * Not to be used by students.
 */
void listenToCompressEvents(Observer<int>* obs);

/*
 * Forgets any counter of how many times compressBoard has been called.
 * Not to be used by students.
 */
void resetCompressCount();

#endif // _compression_h
