#include "TaskOptimization.h"
#include "Optimizer.h"
#include "urlstream.h"
#include <iomanip>
#include <sstream>
#include <cstdint>
using namespace std;

namespace {
    /* Window display attributes. */
    const string kBackgroundColor = "White";

    /* Text to display when solving. */
    const string kSolvingString             = "Solving...";
    const string kDownloadingString         = "Downloading visualization...";
    const string kRemoteDownloadErrorString = "Oops! We weren't able to download a visualization for this process. (This isn't your fault. Try that operation again.)";
    const string kVisualizationErrorString  = "Oops! We weren't able to download a visualization for this process. (Check your network connection?)";

    /* Button labels. */
    const string kOptimizeButtonText = "Find Optimization Candidates";

    /* Constants for transferring data from the Google Charts API to the display. */
    static const string kAPIBase = "https://chart.googleapis.com/chart?cht=gv&chof=png&chl=";

    /* Type representing a downloading error. It just forwards the downloading error along. */
    class DownloadError {
    public:
        DownloadError(int errorCode) : theErrorCode(errorCode) {

        }

        int errorCode() const {
            return theErrorCode;
        }

    private:
        int theErrorCode;
    };

    /* Stable hash function to use to compute hash IDs. This hash function is used in
     * place of other hash functions because it computes consistent IDs across program
     * runs.
     *
     * This is the Jenkins 32-bit hash function. It's not the best hash function available,
     * but it's one of the easiest ones to code up. It's adapted from
     * https://en.wikipedia.org/wiki/Jenkins_hash_function.
     */
    uint32_t jenkinsHashOf(const string& str) {
        uint32_t result = 0;
        for (char ch: str) {
            result += static_cast<unsigned char>(ch);
            result += result << 10;
            result ^= result >> 6;
        }
        result += result << 3;
        result ^= result >> 11;
        result += result << 15;
        return result;
    }

    /* Given a set of tasks, sorts those tasks alphabetically. This is used to get a
     * canonical ordering of the tasks in a group.
     */
    vector<Task *> sortedVersionOf(const HashSet<Task *>& tasks) {
        vector<Task *> result(tasks.begin(), tasks.end());
        sort(result.begin(), result.end(), [](Task* lhs, Task* rhs) {
            return lhs->name < rhs->name;
        });
        return result;
    }

    /* Given a process, produces a string containing a GraphViz representation of that process. */
    string toGraphviz(const TaskGroup& process, const HashSet<Task*>& hotSpots) {
        /* We need to come up with a canonical ordering for processes to ensure that we generate
         * consistent GraphViz files for the same process. We'll do this by sorting the
         * processes alphabetically.
         */
        vector<Task *> orderedProcess = sortedVersionOf(process);

        /* Each task gets mapped to an integer ID equal to its position. This is the reverse
         * mapping.
         */
        HashMap<Task*, int> taskToID;
        for (int i = 0; i < orderedProcess.size(); i++) {
            taskToID[orderedProcess[i]] = i;
        }

        ostringstream output;
        output << "digraph G { " << endl;

        /* Define each of the nodes. */
        for (auto task: orderedProcess) {
            output << "  " << taskToID[task] << " [shape=box,penwidth=2,";
            if (hotSpots.contains(task)) {
                output << "fillcolor=gold,style=filled,";
            }
            output << "label=\"" << task->name << "\\n(Time: " << task->duration << ")\"];" << endl;
        }

        /* Define the edges. */
        for (auto task: orderedProcess) {
            for (auto dependency: sortedVersionOf(task->dependsOn)) {
                output << "  " << taskToID[dependency] << " -> " << taskToID[task] << " [style=bold]" << endl;
            }
        }

        output << "}";

        return output.str();
    }

    /* Converts a string that will be used as part of a URL into a series of escaped
     * characters.
     */
    string urlEscape(const string& str) {
        ostringstream result;
        for (char ch: str) {
            if (isalnum(ch)) {
                result << ch;
            } else {
                result << '%' << setw(2) << setfill('0') << hex << int((unsigned char)ch);
            }
        }
        return result.str();
    }

    /* Returns whether we have a cached version of the given dot string in the indicated
     * base file.
     */
    bool cachedVersionExistsFor(const string& dotString, const string& baseFile) {
        /* See if the .dot and .png files exist. */
        ifstream dotFile(baseFile + ".dot");
        ifstream pngFile(baseFile + ".png");

        /* If the file doesn't exist or can't be opened, then we don't have anything
         * to draw from.
         */
        if (!dotFile || !pngFile) return false;

        /* We'll assume the .dot file is valid for us if its contents match the dot string. */
        ostringstream contents;
        contents << dotFile.rdbuf();
        return contents.str() == dotString;
    }

    /* Attempts really, really hard to get an image for the given process. The Google servers
     * are, unfortunately, kinda flaky, so we'll ping them multiple times in the hope that they
     * succeed at least once. :-(
     */
    const size_t kMaxTries = 10;
    string downloadPNGFor(const string& text) {
        int lastErrorCode = 0;
        for (size_t i = 0; i < kMaxTries; i++) {
            iurlstream downloader;
            downloader.setHeader("User-Agent", "A programming assignment for Stanford's CS106B course. Contact: htiek@cs.stanford.edu");
            downloader.open(kAPIBase + urlEscape(text));

            if (!downloader) {
                lastErrorCode = downloader.getErrorCode();
                continue;
            }

            ostringstream result;
            result << downloader.rdbuf();
            return result.str();
        }

        throw DownloadError(lastErrorCode);
    }

    /* Returns the name of a file containing a visualization of the given process.
     * We use a caching system to avoid repetitively downloading the same file. Specifically,
     * we store pairs of files of the form
     *
     *      [ID].png
     *      [ID].dot
     *
     * where ID is formed by hashing the GraphViz string. If there's an existing .dot file that
     * matches the given GraphViz string, we use it. Otherwise, we query the Google Charts API
     * to create the image.
     */
    string imageFor(const TaskGroup& process,
                    const HashSet<Task *>& hotSpots,
                    GLabel* statusLine) {
        auto text = toGraphviz(process, hotSpots);
        auto baseFile = "processes/images/" + to_string(jenkinsHashOf(text));

        /* If a cached version doesn't exist, go make one! */
        if (!cachedVersionExistsFor(text, baseFile)) {
            statusLine->setText(kDownloadingString);

            /* Download the image file to use. This step may fail if, for example,
             * there's no network connection or the remote servers are having
             * a bad day.
             */
            string pngBytes = downloadPNGFor(text);

            /* Dump everything to disk. */
            ofstream result(baseFile + ".png", ios::binary);
            result << pngBytes;

            /* Write the .dot file for future reference. */
            ofstream dotFile(baseFile + ".dot");
            dotFile << text;

            if (!result || !dotFile) error("Error saving result to disk.");
        }
        return baseFile + ".png";
    }
}

/* File extension is .process. */
string TaskOptimization::fileExtension() {
    return ".process";
}

TaskOptimization::TaskOptimization(istream& input, GWindow& window)
  : solveButton(new GButton(kOptimizeButtonText), window, "NORTH"),
    statusLine (new GLabel(" "),                  window, "SOUTH")
{
    process = loadTaskGroup(input);
}

TaskOptimization::~TaskOptimization() {
    freeProcess(process);
}

void TaskOptimization::actionPerformed(GObject* source) {
    if (source == solveButton.get()) {
        statusLine->setText(kSolvingString);
        hotSpots = optimizationCandidatesFor(process);
        requestRepaint();
    }
}

void TaskOptimization::repaint(GWindow& window) {
    /* Clear the display. */
    window.setColor(kBackgroundColor);
    window.fillRect(0, 0, window.getCanvasWidth(), window.getCanvasHeight());

    /* If there is no active process, there's nothing to do. */
    if (!process.isEmpty()) {
        try {
            GImage image(imageFor(process, hotSpots, statusLine.get()));

            /* Scale the image so that we fit, but don't distort things. */
            const double scale = min(1.0, min(window.getCanvasWidth() / image.getWidth(), window.getCanvasHeight() / image.getHeight()));
            image.scale(scale);

            /* Center the image in the window. */
            double x = (window.getCanvasWidth()  - image.getWidth())  / 2.0;
            double y = (window.getCanvasHeight() - image.getHeight()) / 2.0;

            window.draw(&image, x, y);
            statusLine->setText(" ");
        } catch (const DownloadError& e) {
            if (e.errorCode() < 0) {
                statusLine->setText(kVisualizationErrorString);
            } else {
                statusLine->setText(kRemoteDownloadErrorString);
            }
        }
    }
    window.repaint();
}
