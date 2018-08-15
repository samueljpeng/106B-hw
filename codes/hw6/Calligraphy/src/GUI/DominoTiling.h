#ifndef Domino_Included
#define Domino_Included

#include "ProblemHandler.h"
#include "BipartiteGraph.h"
#include "TemporaryComponent.h"
#include "ginteractors.h"
#include <memory>
#include <istream>

/* Problem class corresponding to the maximum domino tiling problem. */
class DominoTiling: public ProblemHandler {
public:   
    /* Construct handler, assuming the input stream contains the data to read. */
    DominoTiling(std::istream& input, GWindow& window);

    /* File extension. */
    static std::string fileExtension();

    /* Respond to action events. */
    void actionPerformed(GObject* source) override;

protected:
    /* Draw the current state of things. */
    void repaint(GWindow& window) override;

private:
    Temporary<GButton> solveButton;
    Temporary<GLabel>  statusLine;

    /* Underlying bipartite graph and matching. */
    BipartiteGraph graph;
    Matching matching;
};

#endif
