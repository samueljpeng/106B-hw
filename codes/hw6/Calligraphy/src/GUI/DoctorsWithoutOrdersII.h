#ifndef DoctorsWithoutOrdersII_Included
#define DoctorsWithoutOrdersII_Included

#include "ProblemHandler.h"
#include "BipartiteGraph.h"
#include "TemporaryComponent.h"
#include "ginteractors.h"
#include <istream>

/* Problem class for Doctors Without Orders II */
class DoctorsWithoutOrdersII: public ProblemHandler {
public:
    /* Construct handler, assuming the input stream contains the data to read. */
    DoctorsWithoutOrdersII(std::istream& input, GWindow& window);

    /* File extension. */
    static std::string fileExtension();

    /* Respond to action events. */
    void actionPerformed(GObject* source) override;

protected:
    /* Draw the current state of things. */
    void repaint(GWindow& window) override;

private:
    Temporary<GButton> solveButton;

    /* Underlying bipartite graph and matching. */
    BipartiteGraph graph;
    Matching matching;
};

#endif
