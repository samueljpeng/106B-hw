/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file declares the members of a class representing the
 * overall graphical user interface (GUI) for this part of the program.
 * See marblegraphics.cpp for the implementations of each member.
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/10/25
 * - refactor / bug fix for intermittent "pure virtual function called" crash on Windows
 * @version 2017/10/22
 * - bug fix for 1-marble (already solved) case
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#ifndef _marblegraphics_h
#define _marblegraphics_h

#include <fstream>
#include <iostream>
#include <string>
#include "map.h"
#include "gevents.h"
#include "gobjects.h"
#include "grid.h"
#include "gwindow.h"
#include "observable.h"
#include "set.h"
#include "vector.h"
#include "marbleboard.h"
#include "marbletypes.h"

class MarbleCompressWatcher;   // forward declaration

class MarbleGraphics : public Observer<BoardEvent> {
public:
    /*
     * Constructs the GUI and displays its window on the screen.
     */
    MarbleGraphics();

    /*
     * Frees any memory used by the GUI.
     */
    virtual ~MarbleGraphics();

    /*
     * Waits for events in the window and processes them.
     * This function does not return until the GUI window is closed.
     */
    void eventLoop();

    /*
     * Sets a status message to appear at the bottom of the window.
     * If isError is true, displays in a red "error" color.
     */
    void setStatus(const std::string& message, bool isError = false);

    /*
     * Required method of Observer interface.
     * Called by the MarbleBoard when its state changes.
     * Causes the GUI to repaint itself and update its display.
     */
    virtual void update(Observable<BoardEvent>* obs, const BoardEvent& arg);

private:
    /*
     * Checks whether the board is currently solved, and displays a message.
     */
    void checkSolved();

    /*
     * Removes all marbles from the window.
     */
    void clearBoard();

    /*
     * Begins the computer's turn, which will call the student's solvePuzzle function.
     * If the computer finds a solution, this function will also make all of the
     * moves of that solution with a slight delay to produce animation.
     */
    void computerPlay();

    /*
     * Draws graphics of all marbles placed on the game board.
     * Should be called every time the state of the board changes.
     */
    void drawBoard();

    // helper function; draws a single empty space as an outlined oval.
    void drawEmptySpace(int row, int col);

    // helper function; draws a single marble as an image.
    void drawMarble(int row, int col);

    /*
     * Makes a noise and flash to notify the user of user error.
     */
    void errorNotification();

    /*
     * Helper function to get the GObject for a given row/column.
     */
    GObject* getGObjectAtRowCol(int row, int col);

    /*
     * Turn on a highlight for the given row/column square.
     */
    void highlight(int row, int col);

    /*
     * Turn off any existing square highlight.
     */
    void highlightOff();

    /*
     * Called when the human player clicks the mouse on the game board.
     * Each pair of clicks can be used to make a move on a single marble.
     * The isStartClick parameter indicates whether this is the first click
     * (to select a marble) or the second (to jump it to an empty square).
     * Returns true if this was a second click and a move was made.
     * If a move was made, it will be stored in the given Move parameter.
     */
    bool humanClick(GMouseEvent& mouseEvent, bool& isStartClick, Move& move);

    // helper function; returns the filename of a randomly chosen image
    // that can be drawn on a marble.
    std::string pickRandomImage();

    // helper functions to compute the x/y pixel location for a given row/col, or vice versa
    void rowColToXY(int row, int col, int& x, int& y);
    void xyToRowCol(double x, double y, int& row, int& col);

    /*
     * Updates the onscreen state of the game board in response to the given
     * most recently made move by the player.
     * Removes the jumped-over marble, moves the jumper to its new location,
     * and so on.
     */
    void updateLastMove(const Move& move);

    // private member variables
    MarbleBoard _board;    // game state
    bool _isHumanTurn;     // true if it is the human's turn; false if computer's

    // graphical interactors
    GWindow* _window;
    GButton* _defaultButton;
    GButton* _loadButton;
    GButton* _solveButton;
    GTextLabel* _statusLabel;
    GTextLabel* _workingLabel;
    GLabel* _marbleCountLabel;

    // graphical drawn objects
    GImage* _boardBackground;
    GOval* _highlight;     // a yellow oval to highlight selected marbles
    GRect* _flash;         // a yellow rectangle to display over board to indicate an error

    // event objects
    MarbleCompressWatcher* _watcher;

    // constants for various window dimensions and attributes
    static const int MARBLE_DIMENSION;          // w/h of each marble image
    static const int MARBLE_EMPTY_INSET;        // px to shrink empty space size relative to marble size
    static const int MARBLE_HIGHLIGHT_RADIUS;   // px around marble to highlight in yellow
    static const int MARBLE_SPACING;            // px gap between squares
    static const int PAUSE_DURATION_MS;         // how many MS to pause for animation
    static const double CANVAS_SIZE;            // total window canvas size in px
    static const std::string FONT;              // font to use on widgets
    static const std::string RESOURCE_FOLDER;   // where images are located
    static const std::string WINDOW_TITLE;      // window title bar text
};

class MarbleCompressWatcher : public Observer<int> {
public:
    MarbleCompressWatcher(MarbleGraphics* mg);
    virtual ~MarbleCompressWatcher();

    /*
     * Required method of Observer interface.
     * Called by compressBoard when many boards have been compressed.
     * Causes the GUI to display a message about how many boards have been compressed.
     */
    virtual void update(Observable<int>* obs, const int& arg);

private:
    MarbleGraphics* _mg;
};

#endif // _marblegraphics_h
