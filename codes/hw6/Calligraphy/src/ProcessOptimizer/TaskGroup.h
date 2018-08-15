#ifndef TaskGroup_Included
#define TaskGroup_Included

#include "Duration.h"
#include "hashset.h"
#include <string>
#include <ostream>
#include <istream>

/**
 * Type: Task
 *
 * A type representing a single task that needs to be performed.
 */
struct Task {
    std::string name;          // Name of the task
    Duration duration;         // How long it takes to complete
    HashSet<Task*> dependsOn;  // What other tasks we depend on
};

/**
 * Type: TaskGroup
 *
 * A type representing all the tasks that comprise a given process.
 */
using TaskGroup = HashSet<Task *>;

/**
 * Helper function to print tasks. You can use this function by writing something like
 *
 *    cout << myTask << endl;
 *
 * Remember that if you have a pointer to a task - that is, a Task* - you'd need to write
 *
 *    cout << *myTask << endl;
 *
 * since otherwise you'll just print out the value of the pointer itself, not what it points
 * at.
 */
std::ostream& operator<< (std::ostream& out, const Task& task);

/**
 * Reads a task group from a stream, reporting an error() if the file can't be parsed.
 */
TaskGroup loadTaskGroup(std::istream& input);

/**
 * Frees all dynamically-allocated memory associated with a given Process. This is used
 * internally; you shouldn't need to call this function.
 */
void freeProcess(TaskGroup& process);

#endif
