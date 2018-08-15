#include "Duration.h"
#include "TaskGroup.h"
#include "linkedhashset.h"
#include "linkedhashmap.h"
#include "error.h"
#include "strlib.h"
#include <fstream>
using namespace std;

namespace {
    /* Names of the different headers that appear in a file describing a task. */
    const string kNameHeader = "Task:";
    const string kDependencyHeader = "Depends On:";
    const string kDurationHeader = "Duration:";

    /* Given a string consisting of a name, possibly with leading or trailing spaces,
     * returns a canonical version of that name.
     */
    string extractNameFrom(const string& name) {
        string result = trim(name);
        if (result.empty() || result.find(',') != string::npos) error("Illegal name.");

        return result;
    }

    /* Given a name line, returns the name from that line. */
    string parseName(const string& nameLine) {
        if (!startsWith(nameLine, kNameHeader)) error("Malformed name.");

        return extractNameFrom(nameLine.substr(kNameHeader.length()));
    }

    /* Given a dependencies line, returns a list of dependencies from that line. */
    LinkedHashSet<string> parseDependencies(const string& dependenciesLine) {
        if (!startsWith(dependenciesLine, kDependencyHeader)) error("Malformed dependencies.");

        /* The rest of the line is a comma-separated list of dependencies. */
        auto rawNames = stringSplit(dependenciesLine.substr(kDependencyHeader.size()), ",");

        LinkedHashSet<string> result;
        for (string name: rawNames) {
            result += extractNameFrom(name);
        }

        return result;
    }

    /* Given a duration line, returns a number of seconds equal to that duration. */
    Duration parseDuration(const string& durationLine) {
        if (!startsWith(durationLine, kDurationHeader)) error("Malformed duration.");

        istringstream time(durationLine.substr(kDurationHeader.size()));
        Duration result;
        time >> result;
        return result;
    }

    /* A type representing a task, except that task links are referenced purely as
     * string names rather than hard pointers. This is used internally inside loadProcess
     * to build up the graph.
     */
    struct ProtoTask {
        string name;
        Duration duration;
        LinkedHashSet<string> dependsOn; // To maintain the relative order.
    };

    /* Reads a ProtoTask from an input stream, reporting an error if it's not possible to
     * do so.
     */
    istream& operator>> (istream& input, ProtoTask& pt) {
        istream::sentry sentry(input);
        if (sentry) {
            string name, duration, dependencies;

            /* If we can't even read the name line, we're done. */
            if (!getline(input, name)) return input;

            /* Otherwise, read the rest, which should succeed. */
            getline(input, duration);
            getline(input, dependencies);

            /* Strip whitespace. */
            name = trim(name);
            duration = trim(duration);
            dependencies = trim(dependencies);

            pt.name      = parseName(name);
            pt.duration  = parseDuration(duration);
            pt.dependsOn = parseDependencies(dependencies);
        }

        return input;
    }
}

/* Output a human-readable representation of a task. */
ostream& operator<< (ostream& out, const Task& task) {
    ostringstream result;
    result << "{ "
           << kNameHeader << " " << task.name << ", "
           << kDurationHeader << " " << task.duration << ", "
           << kDependencyHeader << " ";

    /* Linearize the tasks. */
    vector<Task*> orderedTasks(task.dependsOn.begin(), task.dependsOn.end());
    for (int i = 0; i < orderedTasks.size(); i++) {
        result << orderedTasks[i]->name;
        if (i + 1 != orderedTasks.size()) result << ", ";
    }
    result << " }";

    return out << result.str();
}

TaskGroup loadTaskGroup(istream& input) {
    /* Read a list of prototasks. */
    LinkedHashMap<string, ProtoTask> prototasks;
    for (ProtoTask task; input >> task; ) {
        if (prototasks.containsKey(task.name)) error("Duplicate name: " + task.name);
        prototasks.put(task.name, task);
    }

    /* Validate that all internal links are to existing tasks. */
    for (const auto& task: prototasks) {
        for (const auto& dependency: prototasks[task].dependsOn) {
            if (!prototasks.containsKey(dependency)) error("Task " + task + " depends on unknown task " + dependency);
        }
    }

    /* Convert everything into a proper set of tasks. */
    LinkedHashMap<string, Task *> tasks;
    TaskGroup result;
    for (const auto& task: prototasks) {
        tasks.put(task, new Task{ prototasks[task].name, prototasks[task].duration, {} });
    }
    for (const auto& task: prototasks) {
        for (const auto& dependency: prototasks[task].dependsOn) {
            tasks[task]->dependsOn += tasks[dependency];
        }
    }

    for (const auto& task: tasks) {
        result += tasks[task];
    }

    return result;
}

void freeProcess(TaskGroup& process) {
    for (auto task: process) {
        delete task;
    }
}
