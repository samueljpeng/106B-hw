/*
 * CS106B Assignment 6-B
 * OptimizerTests.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/15/2018
 */
#include "OptimizerTests.h"
#include "Optimizer.h"
#include <string>
#include <sstream>
using namespace std;

ENABLE_TESTS();

namespace {
    /* Utility function that, given a string, converts that string into a process. */
    TaskGroup parse(const string& description) {
        istringstream source(description);
        return loadTaskGroup(source);
    }

    /* Utility function that, given a set of tasks, returns their names. You can use
     * this to check whether you got back a specific set of tasks from the optimizer.
     * For example:
     *
     *    expect(tasksMatch(optimizationCandidatesFor(proces), { "Task 1", "Task 2", ... }));
     */
    bool tasksMatch(const HashSet<Task *>& tasks, const HashSet<string>& names) {
        HashSet<string> result;
        for (auto task: tasks) {
            result += task->name;
        }
        return result == names;
    }
}

ADD_TEST("Every task in a simple chain is a candidate.") {
    auto process = parse(R"(
       Task: Step One
         Duration: 1d
         Depends On:

       Task: Step Two
         Duration: 1d
         Depends On: Step One

       Task: Step Three
         Duration: 1d
         Depends On: Step Two
    )");

    /* This task should have every step come back as an optimization candidate, since every
     * task is on the critical path.
     */
    expect(tasksMatch(optimizationCandidatesFor(process), { "Step One", "Step Two", "Step Three" }));

    freeProcess(process);
}

ADD_TEST("Only the longest task in a collection of disjoint tasks can be optimized.") {
    auto process = parse(R"(
       Task: Task A
         Duration: 1d
         Depends On:

       Task: Task B
         Duration: 1w
         Depends On:

       Task: Task C
         Duration: 1y
         Depends On:
    )");

    /* Only Task C should be an optimization candidate, since it takes much, much longer
     * than the others!
     */
    expect(tasksMatch(optimizationCandidatesFor(process), { "Task C" }));

    freeProcess(process);
}

ADD_TEST("If two or more tasks are tied for finishing last, no tasks are hot.") {
    auto process = parse(R"(
       Task: Task A
         Duration: 1d
         Depends On:

       Task: Task B
         Duration: 1d
         Depends On: Task A

       Task: Task C
         Duration: 2d
         Depends On:
    )");

    /* Tasks B and C complete at the same time. Therefore, no individual task can be
     * optimized in a way that improves the overall runtime.
     */
    expect(tasksMatch(optimizationCandidatesFor(process), { }));

    freeProcess(process);
}

ADD_TEST("If there's no task, no task.") {
    auto process = parse(R"(
      )");

    expect(tasksMatch(optimizationCandidatesFor(process), { }));

    freeProcess(process);
}

ADD_TEST("With branches.") {
    auto process = parse(R"(
       Task: Step One
         Duration: 1d
         Depends On:

       Task: Step Two
         Duration: 1d
         Depends On: Step One

       Task: Step Three
         Duration: 1d
         Depends On: Step Two

       Task: Step Four
         Duration: 2d
         Depends On: Step Two
    )");

    /* This task should have every step come back as an optimization candidate, since every
     * task is on the critical path.
     */
    expect(tasksMatch(optimizationCandidatesFor(process), { "Step One", "Step Two", "Step Four" }));

    freeProcess(process);
}
