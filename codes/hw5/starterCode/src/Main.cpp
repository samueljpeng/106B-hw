#include "BrowserMain.h"
#include "TestingMain.h"

int main() {
    bool shouldRunTests = true;

    if (shouldRunTests) {
        testingMain();
    } else {
        browserMain();
    }
}
