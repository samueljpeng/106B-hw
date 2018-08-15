#include "GUIMain.h"
#include "TestingMain.h"

int main() {
    bool shouldRunTests = true;
    if (shouldRunTests) {
        testingMain();
    } else {
        guiMain();
    }
    return 0;
}
