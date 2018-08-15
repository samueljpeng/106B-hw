#include "DominoTiling.h"
#include "Matchmaker.h"
#include "error.h"
#include "grid.h"
#include "strlib.h"
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

namespace {
    /* "Solve" button text. */
    const string kSolveButtonText = "Solve!";

    /* Text to display when solving. */
    const string kSolvingText = "Solving...";

    /* Text to display when rendering. */
    const string kRenderingText = "Rendering...";

    /* Window padding. */
    const double kVerticalPadding = 20;
    const double kHorizontalPadding = kVerticalPadding;

    /* Color palette. */
    const string kBackgroundColor   = "#400040";
    const string kOpenColor         = "White";
    const string kClosedColor       = "Black";
    const string kCellBorderColor   = "Black";
    const string kDominoColor       = "#80C0FF";
    const string kDominoBorderColor = "Black";

    /* Padding from the border of a cell to a domino contained in that cell. */
    const double kDominoPadding   = 0.1;

    /* Given a (row, column) pair, returns a string representation of that pair. */
    string toString(int row, int col) {
        return to_string(row) + " " + to_string(col);
    }

    /* Given a string representing a row/column pair, decodes that pair to a set of coordinates. */
    GridLocation fromString(const string& title) {
        istringstream extractor(title);

        /* Get the row and column. */
        int row, col;
        if (extractor >> row >> col, !extractor) error("Can't parse grid coordinate " + title);
        if (row < 0) error("Negative row?");
        if (col < 0) error("Negative column?");

        /* Confirm there's nothing left. */
        char leftover;
        if (extractor >> leftover) error("Unexpected character: " + to_string(leftover));

        return { row, col };
    }

    /* Given a grid of characters represented as a vector<string>, returns the BipartiteGraph it
     * corresponds to.
     */
    BipartiteGraph toGraph(const vector<string>& grid) {
        /* Edge case: if there are no lines, there's no grid. */
        if (grid.empty()) return {};

        const int rows = grid.size();
        const int cols = grid[0].size();
        BipartiteGraph result;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                /* If this is a free location, make it into a node. */
                if (grid[row][col] == '.') {
                    /* Which partition it goes into depends on the parity of the current
                     * square.
                     */
                    ((row + col) % 2 == 0? result.lhs : result.rhs).add(toString(row, col));

                    /* Look in all the cardinal directions for neighbors. */
                    for (int dr = -1; dr <= +1; dr++) {
                        for (int dc = -1; dc <= +1; dc++) {
                            if (((dr != 0) != (dc != 0)) // Orthogonal movement
                                && row + dr >= 0
                                && row + dr < rows       // in-bounds
                                && col + dc >= 0
                                && col + dc < cols
                                && grid[row + dr][col + dc] == '.') {
                                result.edges.add({ toString(row, col), toString(row + dr, col + dc) });
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    /* Given a bipartite graph representing a domino grid, returns a representation of that grid
     * as a Grid<bool> indicating which cells are empty (true) or filled (false).
     */
    Grid<bool> asGrid(const BipartiteGraph& graph) {
        /* Convert all nodes back into (row, col) pairs. */
        Vector<GridLocation> allPoints;
        for (auto textLoc: graph.lhs + graph.rhs) {
            allPoints += fromString(textLoc);
        }

        /* Find the largest row/column used; this will determine sizing. We use -1 as a sentinel
         * here so that if the graph is empty, we get back a 0x0 grid.
         */
        int maxRow = -1, maxCol = -1;
        for (auto loc: allPoints) {
            maxRow = max(maxRow, loc.row);
            maxCol = max(maxCol, loc.col);
        }

        /* Rebuild the grid. We use the fact that all grid entries default to false. */
        Grid<bool> result(maxRow + 1, maxCol + 1);
        for (auto loc: allPoints) {
            result[loc.row][loc.col] = true;
        }
        return result;
    }

    /* Draws a domino grid at the specified coordinates using the specified size. */
    void drawGrid(GWindow& window, const Grid<bool>& grid, double scale, double baseX, double baseY) {
        for (int row = 0; row < grid.numRows(); row++) {
            for (int col = 0; col < grid.numCols(); col++) {
                double x = baseX + col * scale;
                double y = baseY + row * scale;

                /* Fill the rectangle. */
                window.setColor(grid[row][col]? kOpenColor : kClosedColor);
                window.fillRect(x, y, scale, scale);

                /* Draw the border. */
                window.setColor(kCellBorderColor);
                window.drawRect(x, y, scale, scale);
            }
        }
    }

    void drawDominoes(GWindow& window, const Matching& matching, double scale, double baseX, double baseY) {
        for (auto pair: matching) {
            /* Decode the pair's components back to locations. */
            auto p1 = fromString(pair.first);
            auto p2 = fromString(pair.second);

            /* Get the bounding box in pixel space. The range is [lowX, highX) and
             * [lowY, highY) to account for the width and height of the rectangle.
             */
            double lowX  = baseX + min(p1.col, p2.col) * scale;
            double lowY  = baseY + min(p1.row, p2.row) * scale;
            double highX = baseX + (max(p1.col, p2.col) + 1) * scale;
            double highY = baseY + (max(p1.row, p2.row) + 1) * scale;

            /* Each domino will be placed so that it overlaps the bounding rectangle, with a slight
             * border placed around it, hence the offsets.
             */
            double x      = lowX + scale * kDominoPadding;
            double y      = lowY + scale * kDominoPadding;
            double width  = (highX - lowX) - 2 * scale * kDominoPadding;
            double height = (highY - lowY) - 2 * scale * kDominoPadding;

            window.setColor(kDominoColor);
            window.fillRect(x, y, width, height);

            window.setColor(kDominoBorderColor);
            window.drawRect(x, y, width, height);
        }
    }

    /* Returns a new stream that strips out all blank lines and comments from the original. */
    istringstream sanitize(istream& input) {
        ostringstream builder;

        for (string line; getline(input, line); ) {
            /* Strip comments, if any. */
            size_t commentStart = line.find('#');
            if (commentStart != string::npos) {
                line = line.substr(0, commentStart);
            }

            /* Trim whitespace. */
            line = trim(line);

            if (!line.empty()) builder << line << endl;
        }

        return istringstream(builder.str());
    }
}

/* We use the .grid extension. */
string DominoTiling::fileExtension() {
    return ".grid";
}

DominoTiling::DominoTiling(istream& source, GWindow& window)
  : solveButton(new GButton(kSolveButtonText), window, "NORTH"),
    statusLine (new GLabel(""),                window, "SOUTH")
{
    istringstream input = sanitize(source);

    /* Grab all the lines of the file. */
    vector<string> lines;
    for (string line; getline(input, line); ) {
        lines.push_back(line);
    }

    /* Confirm that all lines have the same length. */
    for (const auto& line: lines) {
        if (line.find_first_not_of(".X") != string::npos) error("Invalid characters in file.");
        if (line.size() != lines.back().size()) error("Grid does not have uniform dimensions.");
    }

    /* Stash the graph for later. */
    graph = toGraph(lines);
}

/* Draws the grid and matching, if any. */
void DominoTiling::repaint(GWindow& window) {
    statusLine->setText(kRenderingText);

    /* Clear the window. */
    window.setColor(kBackgroundColor);
    window.fillRect(0, 0, window.getCanvasWidth(), window.getCanvasHeight());

    /* Convert the graph back to a grid. */
    auto grid = asGrid(graph);

    /* If this grid is empty, there's nothing to draw. */
    if (grid.size() != 0) {
        double width  = window.getCanvasWidth()  - 2 * kHorizontalPadding;
        double height = window.getCanvasHeight() - 2 * kVerticalPadding;
        /* Determine the scaling factor used to display the grid. */
        double scale = min(width / grid.numCols(), height / grid.numRows());

        /* Determine the x and y offsets to center everything. */
        double baseX = kHorizontalPadding + (width  - grid.numCols() * scale) / 2.0;
        double baseY = kVerticalPadding   + (height - grid.numRows() * scale) / 2.0;

        drawGrid(window, grid, scale, baseX, baseY);
        drawDominoes(window, matching, scale, baseX, baseY);
    }

    window.repaint();
    statusLine->setText("");
}

/* Reacts to an action. If it's the solve button, run the student solver. */
void DominoTiling::actionPerformed(GObject* source) {
    if (source == solveButton.get()) {
        statusLine->setText(kSolvingText);

        matching = matchmake(graph);

        requestRepaint();
    }
}
