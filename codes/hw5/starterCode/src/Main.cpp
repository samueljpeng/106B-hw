#include "BrowserMain.h"
#include "TestingMain.h"
#include <iostream>

int main() {
    bool shouldRunTests = true;
    std::cout << "Checkpoint 1\n";
    if (shouldRunTests) {
        std::cout << "Checkpoint 2\n";
        testingMain();
    } else {
        std::cout << "Checkpoint 2\n";
        browserMain();
    }
}
