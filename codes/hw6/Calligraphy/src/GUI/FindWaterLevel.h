#ifndef FindWaterLevel_Included
#define FindWaterLevel_Included

#include "Floodplain.h"
#include "ProblemHandler.h"
#include "TemporaryComponent.h"
#include "gwindow.h"
#include "ginteractors.h"
#include <istream>

/* Problem class corresponding to the water level problem. */
class FindWaterLevel: public ProblemHandler {
public:
    /* Construct handler, assuming the input stream contains the data to read. */
    FindWaterLevel(std::istream& input, GWindow& window);

    /* File extension. */
    static std::string fileExtension();

    /* Respond to action events. */
    void actionPerformed(GObject* source) override;

protected:
    /* Draw the current state of things. */
    void repaint(GWindow& window) override;

private:
    Temporary<GLabel>     heightDesc;
    Temporary<GTextField> heightField;
    Temporary<GButton>    solveButton;
    Temporary<GLabel>     statusLine;

    /* The floodplain and what's currently under water. */
    Floodplain plain;
    Grid<bool> underwater;

    /* Runs a flood simulation. */
    void runFlood(double height);
};

#endif
