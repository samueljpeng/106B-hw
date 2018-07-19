/*
 * CS 106B/X Marble Solitaire
 * This instructor-provided file defines the members of a class representing the
 * overall graphical user interface (GUI) for this part of the program.
 * See marblegraphics.h for the class and member declarations and documentation.
 *
 * This file also contains the 'main' function to run the program,
 * which just creates a GUI object and waits for it to receive events.
 *
 * You don't need to modify this file.
 * Your recursive function should work with an unmodified version of this file.
 *
 * @author Cynthia Lee and Marty Stepp
 * @version 2017/10/25
 * - refactor / bug fix for intermittent "pure virtual function called" crash on Windows
 * @version 2017/10/22
 * - bug fix for 1-marble (already solved) case
 * - disable/enable buttons during solvePuzzle search
 * @version 2017/10/21
 * - small bug fixes; icons on buttons
 * @version 2017/10/19
 * - initial version for 17au CS 106B/X
 */

#include "marblegraphics.h"
#include "filelib.h"
#include "gfilechooser.h"
#include "goptionpane.h"
#include "random.h"
#include "simpio.h"
#include "timer.h"
#include "compression.h"

// this is your function!
extern bool solvePuzzle(Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory);

/*
 * Runs the overall program.
 */
int main() {
    MarbleGraphics mg;
    mg.eventLoop();
    return 0;
}

// define constants
const int MarbleGraphics::MARBLE_DIMENSION = 100;
const int MarbleGraphics::MARBLE_EMPTY_INSET = 4;
const int MarbleGraphics::MARBLE_HIGHLIGHT_RADIUS = 5;
const int MarbleGraphics::MARBLE_SPACING = 10;
const int MarbleGraphics::PAUSE_DURATION_MS = 200;
const double MarbleGraphics::CANVAS_SIZE =
        ((MarbleBoard::BOARD_DIMENSION + 1) * MARBLE_SPACING)
        + (MarbleBoard::BOARD_DIMENSION * MARBLE_DIMENSION);
const std::string MarbleGraphics::FONT = "SansSerif-18-Bold";
const std::string MarbleGraphics::RESOURCE_FOLDER = "";
const std::string MarbleGraphics::WINDOW_TITLE = "CS 106B/X Marble Solitaire";

/*
 * Helper function to clear a grid of pointers.
 */
template <typename ValueType>
static void clearGrid(Grid<ValueType*>& grid) {
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            grid[r][c] = nullptr;
        }
    }
}

MarbleGraphics::MarbleGraphics() {
    // initialize grids
    _board.addObserver(this);
    _isHumanTurn = true;

    // set up the graphical window
    _window = new GWindow(/* visible */ false);
    _window->setWindowTitle(WINDOW_TITLE);
    _window->setSize(CANVAS_SIZE, CANVAS_SIZE);
    _window->center();
    _window->setResizable(true);
    _window->setExitOnClose(true);

    // north buttons and components
    _loadButton = new GButton("Load");
    _loadButton->setIcon("icon-load.png");
    _defaultButton = new GButton("Default");
    _defaultButton->setIcon("icon-default.png");
    _solveButton = new GButton("Solve!");
    _solveButton->setIcon("icon-solve.png");
    _workingLabel = new GTextLabel;
    _workingLabel->setIcon("progress.gif");
    _workingLabel->setVisible(false);
    _window->addToRegion(_loadButton, GWindow::REGION_NORTH);
    _window->addToRegion(_defaultButton, GWindow::REGION_NORTH);
    _window->addToRegion(_solveButton, GWindow::REGION_NORTH);
    _window->addToRegion(_workingLabel, GWindow::REGION_NORTH);
    _window->setCanvasSize(CANVAS_SIZE, CANVAS_SIZE);
    _window->center();

    _statusLabel = new GTextLabel("<html>&nbsp;</html>");
    _statusLabel->setFont(FONT);

    // status label for printing messages in south region
    _statusLabel = new GTextLabel("<html>&nbsp;</html>");
    _statusLabel->setFont(FONT);
    _window->addToRegion(_statusLabel, GWindow::REGION_SOUTH);
    _window->setCanvasSize(CANVAS_SIZE, CANVAS_SIZE);
    _window->center();

    // wood board background
    _boardBackground = new GImage(RESOURCE_FOLDER + "wood-background.jpg");
    _boardBackground->scale((CANVAS_SIZE + 10) / _boardBackground->getWidth(),
                           (CANVAS_SIZE + 10) / _boardBackground->getHeight());
    _window->add(_boardBackground, 0, 0);

    _marbleCountLabel = new GLabel;
    _marbleCountLabel->setFont(FONT);
    _marbleCountLabel->setLocation(0, 20);
    _window->add(_marbleCountLabel);

    // a "flash" to display quickly when user makes an error (don't show yet)
    _flash = new GRect(CANVAS_SIZE + 10, CANVAS_SIZE + 10);
    _flash->setColor("YELLOW");
    _flash->setFilled(true);

    _highlight = new GOval(MARBLE_DIMENSION + 2 * MARBLE_HIGHLIGHT_RADIUS,
                           MARBLE_DIMENSION + 2 * MARBLE_HIGHLIGHT_RADIUS);
    _highlight->setColor("YELLOW");
    _highlight->setFilled(true);
    highlightOff();

    // draw the initial marbles and stuff
    drawBoard();

    // size and position the window
    _window->setCanvasSize(CANVAS_SIZE, CANVAS_SIZE);
    _window->center();
    _window->setVisible(true);
    _window->setCanvasSize(CANVAS_SIZE, CANVAS_SIZE);
    _window->center();

    // set up the initial board
    _board.resetToDefault();

    _watcher = new MarbleCompressWatcher(this);
    listenToCompressEvents(_watcher);
}

MarbleGraphics::~MarbleGraphics() {
//    for (GObject* obj : _marbleImages) {
//        if (obj) {
//            delete obj;
//        }
//    }
//    for (GObject* obj : _marbleSpaces) {
//        if (obj) {
//            delete obj;
//        }
//    }
    delete _defaultButton;
    delete _loadButton;
    delete _solveButton;
    delete _statusLabel;
    delete _workingLabel;
    delete _marbleCountLabel;
    delete _boardBackground;
    delete _highlight;
    delete _flash;
    delete _watcher;
    delete _window;
}

void MarbleGraphics::checkSolved() {
    if (_board.isSolved()) {
        if (_isHumanTurn) {
            setStatus("Congrats! You have won! :-)");
        } else {
            setStatus("My mighty CPU brain has solved the board.");
        }
    }
}

void MarbleGraphics::clearBoard() {
    _window->clearCanvas();
    _isHumanTurn = true;
}

/*
 * Performs computer play which will call the recursive exhaustive
 * search function in order to try to find a valid path. If a
 * valid path is found, it will allow the user to step through
 * the solution.
 */
void MarbleGraphics::computerPlay() {
    _isHumanTurn = false;
    _loadButton->setEnabled(false);
    _defaultButton->setEnabled(false);
    _solveButton->setEnabled(false);
    highlightOff();
    _workingLabel->setVisible(true);
    resetCompressCount();
    setStatus("Starting computer solver ...");
    Vector<Move> pathToWin;
    Grid<Marble> boardCopy = _board.grid();
    Timer tim;
    tim.start();
    bool won = solvePuzzle(boardCopy, _board.marbleCount(), pathToWin);
    tim.stop();
    if (won) {
        if (pathToWin.isEmpty()) {
            checkSolved();
        } else {
            for (const Move& move : pathToWin) {
                pause(PAUSE_DURATION_MS);
                if (_board.isValidMove(move)) {
                    setStatus("Making a move: " + move.toString());
                    _board.makeMove(move);
                } else {
                    setStatus("Invalid move: " + move.toString());
                    std::cout << "You returned the following invalid path:" << std::endl;
                    std::cout << pathToWin << std::endl;
                    break;
                }
            }
        }
    } else {
        setStatus("Sorry, no solution was found.", /* isError */ true);
    }

    _statusLabel->setText(_statusLabel->getText() + " (" + integerToString(tim.elapsed()) + "ms)");

    _isHumanTurn = false;
    _loadButton->setEnabled(true);
    _defaultButton->setEnabled(true);
    _solveButton->setEnabled(true);
    _workingLabel->setVisible(false);
}

void MarbleGraphics::drawBoard() {
    // delay repainting after each single marble is added, for speed
    _window->setRepaintImmediately(false);

    // wipe the board clean
    clearBoard();
    _window->add(_boardBackground, 0, 0);
    _window->add(_marbleCountLabel);

    // add highlight just after board background
    highlightOff();
    _window->add(_highlight);

    // first pass: draw empty spaces as outlined ovals
    for (int row = 0; row < _board.dimension(); row++) {
        for (int col = 0; col < _board.dimension(); col++) {
            Marble marb = _board.get(row, col);
            if (marb == MARBLE_EMPTY || marb == MARBLE_OCCUPIED) {
                drawEmptySpace(row, col);
            }
        }
    }

    // second pass: draw occupied spaces as marble images
    // (we do this second so that the marbles will be "on top"
    for (int row = 0; row < _board.dimension(); row++) {
        for (int col = 0; col < _board.dimension(); col++) {
            if (_board.get(row, col) == MARBLE_OCCUPIED) {
                drawMarble(row, col);
            }
        }
    }

    _window->setRepaintImmediately(true);
    _window->repaint();
}

void MarbleGraphics::drawEmptySpace(int row, int col) {
    if (!_board.inBounds(row, col)) {
        return;
    }
    GOval* emptySpace = new GOval(MARBLE_DIMENSION - 2 * MARBLE_EMPTY_INSET,
                                  MARBLE_DIMENSION - 2 * MARBLE_EMPTY_INSET);
    int x, y;
    rowColToXY(row, col, x, y);
    _window->add(emptySpace, x + MARBLE_EMPTY_INSET, y + MARBLE_EMPTY_INSET);
}

void MarbleGraphics::drawMarble(int row, int col) {
    if (!_board.inBounds(row, col)) {
        return;
    }
    GImage* marble = new GImage(pickRandomImage());
    int x, y;
    rowColToXY(row, col, x, y);
    _window->add(marble, x, y);
}


void MarbleGraphics::errorNotification() {
    // empty
}

void MarbleGraphics::eventLoop() {
    setStatus("Welcome to CS 106B/X Marble Solitaire!");

    bool isStartClick = true;   // alternates for each human click
    Move move;

    while (true) {
        GEvent event = waitForEvent(ACTION_EVENT | MOUSE_EVENT | WINDOW_EVENT);
        if (event.getEventClass() == ACTION_EVENT) {
            GActionEvent actionEvent(event);
            if (actionEvent.getSource() == _loadButton) {
                // load board from a file
                std::string filename = GFileChooser::showOpenDialog("", "*.txt");
                if (filename.empty()) {
                    continue;
                }
                setStatus("Reading board from " + getTail(filename) + " ...");
                if (_board.readBoardFromFile(filename)) {
                    setStatus("Reading board ... Complete.");
                    _isHumanTurn = true;
                } else {
                    setStatus("Reading board failed.");
                }
                isStartClick = true;
                highlightOff();
            } else if (actionEvent.getSource() == _defaultButton) {
                _window->setRepaintImmediately(false);
                _board.resetToDefault();
                _isHumanTurn = true;
                setStatus("Reset to default board.");
                isStartClick = true;
                highlightOff();
                _window->setRepaintImmediately(true);
                _window->repaint();
            } else if (actionEvent.getSource() == _solveButton) {
                computerPlay();
            }
        } else if (event.getEventClass() == MOUSE_EVENT) {
            GMouseEvent mouseEvent(event);
            if (event.getEventType() == MOUSE_PRESSED
                    && _isHumanTurn
                    && !_board.isSolved()) {
                // human player's turn
                if (humanClick(mouseEvent, isStartClick, move)) {
                    if (_board.isValidMove(move)) {
                        // valid move; make the move
                        isStartClick = true;
                        highlightOff();
                        setStatus("Making a move: " + move.toString());
                        _board.makeMove(move);
                    } else {
                        // invalid move; show error message
                        setStatus("Invalid move: " + move.toString(), /* isError */ true);
                        errorNotification();
                    }
                }
            }
        } else if (event.getEventClass() == WINDOW_EVENT) {
            if (event.getEventType() == WINDOW_CLOSING
                    || event.getEventType() == WINDOW_CLOSED) {
                break;
            }
        }
    }
}

GObject* MarbleGraphics::getGObjectAtRowCol(int row, int col) {
    int x, y;
    rowColToXY(row, col, x, y);
    return _window->getGObjectAt(x, y);
}

void MarbleGraphics::highlight(int row, int col) {
    int x, y;
    rowColToXY(row, col, x, y);
    _highlight->setLocation(x - MARBLE_HIGHLIGHT_RADIUS,
                            y - MARBLE_HIGHLIGHT_RADIUS);
    _highlight->setVisible(true);
}

void MarbleGraphics::highlightOff() {
    _highlight->setVisible(false);
}

bool MarbleGraphics::humanClick(GMouseEvent& mouseEvent, bool& isStartClick, Move& move) {
    GObject* obj = _window->getGObjectAt(mouseEvent.getX(), mouseEvent.getY());
    if (!obj) {
        isStartClick = true;
        highlightOff();
        errorNotification();
    } else if (obj->getType() == "GImage" && obj != _boardBackground) {
        // user clicked on a marble
        if (isStartClick) {
            // first click = starting location
            xyToRowCol(mouseEvent.getX(), mouseEvent.getY(), move.startRow, move.startCol);
            if (((GImage*) obj)->isVisible()) {
                GPoint pt = ((GImage*) obj)->getLocation();
                _highlight->setLocation(pt.getX() - MARBLE_HIGHLIGHT_RADIUS,
                                        pt.getY() - MARBLE_HIGHLIGHT_RADIUS);
                _highlight->setVisible(true);
            }
            isStartClick = false;
        } else {
            int endRow, endCol;
            xyToRowCol(mouseEvent.getX(), mouseEvent.getY(), endRow, endCol);
            if (move.startRow == endRow && move.startCol == endCol) {
                // abort move if user clicks on same marble twice
                isStartClick = true;
                highlightOff();
            }
        }
    } else if (obj->getType() == "GOval") {
        // user clicked in empty valid space on playing board
        if (!isStartClick) {
            // second click = ending location
            xyToRowCol(mouseEvent.getX(), mouseEvent.getY(), move.endRow, move.endCol);
            return true;
        }
    }
    return false;
}

std::string MarbleGraphics::pickRandomImage() {
    static const int IMAGE_COUNT = 17;
    int imageNum = randomInteger(1, IMAGE_COUNT);
    std::string image = RESOURCE_FOLDER + "m" + integerToString(imageNum) + ".png";
    if (!fileExists(image)) {
        error("Missing required image file: " + image);
    }
    return image;
}

void MarbleGraphics::rowColToXY(int row, int col, int& x, int& y) {
    // opposite of xyToRowCol formula
    x = col * (MARBLE_DIMENSION + MARBLE_SPACING) + MARBLE_SPACING;
    y = row * (MARBLE_DIMENSION + MARBLE_SPACING) + MARBLE_SPACING;
}

void MarbleGraphics::xyToRowCol(double x, double y, int& row, int& col) {
    // opposite of rowColToXY formula
    row = (y - MARBLE_SPACING) / (MARBLE_DIMENSION + MARBLE_SPACING);
    col = (x - MARBLE_SPACING) / (MARBLE_DIMENSION + MARBLE_SPACING);
}

void MarbleGraphics::setStatus(const std::string& message, bool isError) {
    _statusLabel->setText(message);
    if (isError) {
        _statusLabel->setColor(0x992222);
    } else {
        _statusLabel->setColor(0x222222);
    }

    // also output to (plain-text) console for debugging / output checking
    std::cout << message << std::endl;
}

void MarbleGraphics::update(Observable<BoardEvent>* /* obs */, const BoardEvent& arg) {
    _marbleCountLabel->setText(integerToString(_board.marbleCount()));
    switch (arg) {
    case BOARD_EVENT_CLEAR:
    case BOARD_EVENT_LOAD_DEFAULT:
    case BOARD_EVENT_LOAD_FROM_FILE:
        drawBoard();
        break;
    case BOARD_EVENT_MOVE_MADE:
        updateLastMove(_board.lastMove());
        checkSolved();
    }
}

void MarbleGraphics::updateLastMove(const Move& move) {
    GObject* movingMarbleObj = (GImage*) getGObjectAtRowCol(move.startRow, move.startCol);
    int jumpedRow = move.startRow + (move.endRow - move.startRow) / 2;
    int jumpedCol = move.startCol + (move.endCol - move.startCol) / 2;
    GObject* jumpedMarbleObj = (GImage*) getGObjectAtRowCol(jumpedRow, jumpedCol);
    if (!movingMarbleObj || !jumpedMarbleObj
            || movingMarbleObj->getType() != "GImage"
            || jumpedMarbleObj->getType() != "GImage") {
        return;
    }
    GImage* movingMarble = (GImage*) movingMarbleObj;
    GImage* jumpedMarble = (GImage*) jumpedMarbleObj;

    //Move image at start location to image at end location
    int endX, endY;
    rowColToXY(move.endRow, move.endCol, endX, endY);

    movingMarble->setLocation(endX, endY);

    //Delete marble for jumped space and create empty space there instead
    _window->remove(jumpedMarble);
//    std::cout << "move: " << move << std::endl;
//    std::cout << "moving marble: " << movingMarble << " " << movingMarble->toString() << std::endl;
//    std::cout << "jumped marble: " << jumpedMarble << " " << jumpedMarble->toString() << std::endl;
//    std::cout << std::endl;
    // delete jumpedMarble;
}

MarbleCompressWatcher::MarbleCompressWatcher(MarbleGraphics* mg)
    : _mg(mg)
{
    // empty
}

MarbleCompressWatcher::~MarbleCompressWatcher() {
    // empty
}

void MarbleCompressWatcher::update(Observable<int>* /* obs */, const int& arg) {
    _mg->setStatus("Boards evaluated: " + integerToString(arg));
}
