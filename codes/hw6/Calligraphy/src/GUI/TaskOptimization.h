#ifndef TaskOptimization_Included
#define TaskOptimization_Included

#include "ProblemHandler.h"
#include "TemporaryComponent.h"
#include "TaskGroup.h"
#include "hashset.h"
#include "gevents.h"
#include <istream>

/* Problem class for task optimization */
class TaskOptimization: public ProblemHandler {
public:
    /* Construct handler, assuming the input stream contains the data to read. */
    TaskOptimization(std::istream& input, GWindow& window);

    /* Destructor frees the underlying process. */
    ~TaskOptimization();

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

    /* Underlying process and identified hot spots. */
    TaskGroup process;
    HashSet<Task*> hotSpots;
};

#endif
