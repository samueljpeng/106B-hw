/* Main driver program. This just glues everything together. */
#include "Browser.h"
#include "gevents.h"
using namespace std;

void browserMain() {
    Browser browser;
    browser.setPage("special:welcome");

    /* Run the main event loop. */
    while (true) {
        /* This won't actually do anything unless the browser has been asked to repaint itself. */
        browser.repaint();

        /* Forward all events to the browser. */
        GEvent e = waitForEvent(ACTION_EVENT | MOUSE_EVENT | CHANGE_EVENT);
        browser.handleEvent(e);
    }
}
