#include "ProblemHandler.h"
using namespace std;

/* Issues a redraw, if necessary. */
void ProblemHandler::draw(GWindow& window) {
    if (isDirty) {
        repaint(window);
        isDirty = false;
    }
}

/* Sets the dirty bit. */
void ProblemHandler::requestRepaint() {
    isDirty = true;
}
