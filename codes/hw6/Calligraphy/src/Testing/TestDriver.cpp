#include "TestDriver.h"
#include <string>
using namespace std;

void doFail(const char* message, size_t line, const char* filename) {
    throw TestFailedException(message, line, filename);
}
void doExpect(bool condition, const char* expression, size_t line, const char* filename) {
    if (!condition) {
        doFail(expression, line, filename);
    }
}

/* TestFailedException implementation. */
TestFailedException::TestFailedException(const char* message, std::size_t line, const char*)
    : logic_error("Line " + to_string(line) + ": " + string(message)) {

}

/* TestCaseAdder implementation. */
TestCaseAdder::TestCaseAdder(TestGroup& group, const string& name, TestCase test) {
    if (group.containsKey(name)) error("Two or more tests have the same name \"" + name + ".\"");
    group.put(name, test);
}
