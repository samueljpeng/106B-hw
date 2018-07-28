#include "AddressPanel.h"
#include "Browser.h"
using namespace std;

/* Graphics constants. */
namespace {
    const string kSearchTitle = "Article: ";
    const int kSearchWidth = 30;
}

AddressPanel::AddressPanel(Browser* browser, GWindow& window, AutocompletePanel* autoPanel)
    : browser(browser), autocomplete(autoPanel) {

    searchBar = new GTextField(kSearchWidth);
    window.addToRegion(new GLabel(kSearchTitle), "NORTH");
    window.addToRegion(searchBar, "NORTH");
    searchBar->addChangeListener();
}

/* If an action is performed that was spawned by the search bar, go and change
 * the page to refer to that new article.
 */
void AddressPanel::actionPerformed(GObject* source) {
    if (source == searchBar) {
        browser->setPage(searchBar->getText());
    }
}

/* Keep the address bar text in sync with the current page. */
void AddressPanel::pageChanged(const string& title) {
    searchBar->setText(title);
    lastText = title;

    /* Disable autocomplete suggestions - we found what we're looking for! */
    autocomplete->disable();
}

/* If the search bar changes, let the autocomplete panel know about it. */
void AddressPanel::changeOccurredIn(GObject* source) {
    if (source == searchBar) {
        auto currText = searchBar->getText();
        if (currText != lastText) {
            autocomplete->setPrefix(searchBar->getText());
            lastText = currText;
        }
    }
}

string AddressPanel::text() const {
    return searchBar->getText();
}
