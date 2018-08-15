#include "GUIMain.h"
#include "ProblemHandler.h"
#include "EmptyProblemHandler.h"
#include "DominoTiling.h"
#include "DoctorsWithoutOrdersII.h"
#include "FindWaterLevel.h"
#include "TaskOptimization.h"
#include "ginteractors.h"
#include "gwindow.h"
#include "gevents.h"
#include "hashmap.h"
#include "filelib.h"
#include "goptionpane.h"
#include <fstream>
#include <memory>
#include <functional>
#include "gfilechooser.h"
using namespace std;

namespace {
    /* Window graphics constants. */
    const double kWindowWidth  = 1000;
    const double kWindowHeight = 800;

    /* Default window title. */
    const string kWindowTitle = "Calligraphy";

    /* Buttons. */
    const string kLoadButtonText = "Choose Problem...";

    /* Type representing a function that builds a handler. */
    using Constructor = function<shared_ptr<ProblemHandler> (istream &, GWindow &)>;

    /* Type representing all state necessary to make the graphics work. */
    struct Graphics {
        GWindow window;                            // The window
        shared_ptr<ProblemHandler> handler;        // Current task handler
        GButton* loadButton;                       // Button to change task; never removed
        HashMap<string, Constructor> byExtension;  // Map from file extensions to handler constructors
    };

    /* Associates a given type with its extension. */
    template <typename Handler> void registerHandler(HashMap<string, Constructor>& result) {
        if (result.containsKey(Handler::fileExtension())) {
            error("Duplicate file extension: " + Handler::fileExtension());
        }

        /* Associate a constructor with the handler. */
        result[Handler::fileExtension()] = [] (istream& input, GWindow& window) {
            return make_shared<Handler>(input, window);
        };
    }

    /* Makes a map associating each extension with its problem handler. */
    HashMap<string, Constructor> extensionTable() {
        HashMap<string, Constructor> result;

        registerHandler<DominoTiling>(result);
        registerHandler<DoctorsWithoutOrdersII>(result);
        registerHandler<TaskOptimization>(result);
        registerHandler<FindWaterLevel>(result);

        return result;
    }

    /* Creates the graphics window and associated state. */
    shared_ptr<Graphics> makeGraphics() {
        auto result = make_shared<Graphics>();

        /* Basic window setup. */
        result->window.setTitle(kWindowTitle);
        result->window.setExitOnClose();

        /* Install buttons. */
        result->loadButton = new GButton(kLoadButtonText);
        result->window.addToRegion(result->loadButton, "NORTH");

        /* Basic graphics setup. */
        result->window.setRepaintImmediately(false);
        result->window.setCanvasSize(kWindowWidth, kWindowHeight);

        /* Default handler. */
        result->handler = make_shared<EmptyProblemHandler>();

        /* File extension handlers. */
        result->byExtension = extensionTable();
        return result;
    }

    /* Prompts the user to choose a problem, loading in the result. */
    void loadProblem(shared_ptr<Graphics> graphics) {
      /* Have the user tell us what file they want. */
      ifstream input;
      string filename = GFileChooser::showOpenDialog("");
      input.open(filename);
      /* If the user didn't choose a file, there's nothing to do. */
      if (!input.is_open() || !input || filename.empty()) return;

      /* See if we have a handler for this extension type. */
      auto extension = getExtension(filename);
      auto constructor = graphics->byExtension.get(extension);
      if (!constructor) {
          GOptionPane::showMessageDialog("Unknown file extension: " + extension);
          return;
      }

      /* We do have a handler. Dispose of whatever used to be here. */
      graphics->handler.reset();
      graphics->window.clearCanvas();

      /* Now, load whatever this problem is. */
      try {
          graphics->handler = constructor(input, graphics->window);
      } catch (const exception& e) {
          /* Oops! Something went wrong. Default to the empty handler. */
          GOptionPane::showMessageDialog("An error occurred: " + string(e.what()));
          graphics->handler = make_shared<EmptyProblemHandler>();
      }
    }
}

void guiMain() {
    auto graphics = makeGraphics();

    while (true) {
        /* Update the window (no-op if nothing needs to be redrawn.) */
        graphics->handler->draw(graphics->window);

        GEvent e = waitForEvent(ACTION_EVENT);
        if (e.getEventClass() == ACTION_EVENT) {
            auto source = GActionEvent(e).getSource();

            /* We are responsible for the load button. */
            if (source == graphics->loadButton) {
                loadProblem(graphics);
            }
            /* Any other event is the responsible of the problem handler. */
            else {
                graphics->handler->actionPerformed(source);
            }
        }
    }
}
