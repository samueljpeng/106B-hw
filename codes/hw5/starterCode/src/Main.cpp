#include "BrowserMain.h"
#include "TestingMain.h"
#include <iostream>

int main() {
    bool shouldRunTests = false;
    if (shouldRunTests) {
        testingMain();
    } else {
        browserMain();
    }
}
