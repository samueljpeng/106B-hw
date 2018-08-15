#ifndef ProblemHandler_Included
#define ProblemHandler_Included

#include "gwindow.h"
#include "gobjects.h"

/* Interface representing a type that's capable of displaying a problem
 * graphically and responding to events.
 *
 * Each subtype that wants to be registered in the global registration system
 * must define these two functions:
 *
 *    static std::string fileExtension()
 *       Returns the file extension associated with this type.
 *    Constructor(std::istream& source, GWindow& window)
 *       Create a handler and install its interactors in the window.
 */
class ProblemHandler {
public:
    /* Polymorphic classes need virtual destructors. */
    virtual ~ProblemHandler() = default;

    /* Respond to action events. */
    virtual void actionPerformed(GObject* source) = 0;

    /* Redraws the window. */
    void draw(GWindow& window);

protected:
    /* Draw the current state of things. */
    virtual void repaint(GWindow& window) = 0;

    /* Marks the region as dirty. */
    void requestRepaint();

private:
    /* Dirty bit. We're initially dirty because nothing's been drawn yet. */
    bool isDirty = true;
};

#endif
