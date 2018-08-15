#include "DoctorsWithoutOrdersII.h"
#include "Matchmaker.h"
#include "strlib.h"
#include "error.h"
#include "hashmap.h"
#include "gobjects.h"
#include <algorithm>
#include <sstream>
using namespace std;

namespace {
    /* "Solve" button text. */
    const string kSolveButtonText = "Solve!";

    const string kBackgroundColor = "White";

    /* Text box information. */
    const double kBoxWidth       = 250;
    const double kBoxHeight      = 30;

    /* Global top/bottom padding. */
    const double kVerticalPadding = kBoxHeight / 2.0;

    /* Spacing between columns. */
    const double kColumnSpacing = 200;

    /* Different colors for matched and unmatched boxes. */
    const string kMatchedBoxColor       = "#F3E5AB"; // Vanilla
    const string kMatchedBoxBorderColor = "Black";
    const string kMatchedBoxFontColor   = "Black";
    const string kMatchedBoxFont        = "Sans Serif-BOLD-16";

    const string kUnusedBoxColor       = "White";
    const string kUnusedBoxBorderColor = "Gray";
    const string kUnusedBoxFontColor   = "Gray";
    const string kUnusedBoxFont        = "Sans Serif-16";

    /* Line information. */
    const string kUnusedLineColor      = "#C0C0C0";
    const string kMatchedLineColor     = "Blue";
    const double kUnusedLineThickness  = 3;
    const double kMatchedLineThickness = 5;

    /* Extracts a single logical token from a stream, consuming all characters up to
     * and including the delimiter.
     */
    string extractToken(istream& stream, char delimiter) {
        string result;
        if (!getline(stream, result, delimiter)) error("Unterminated token.");

        return trim(result);
    }

    /* Given a line from the file of the form [Header] [Name]: [Attributes], parses
     * that line and fills in the appropriate entry in the table.
     */
    void parseLine(const string& line, const string& header,
                   HashMap<string, HashSet<string>>& table) {
        /* Skip the header. */
        istringstream input(line.substr(header.size()));

        /* Extract the name. */
        string name = header + extractToken(input, ':');
        if (table.containsKey(name)) error("Duplicate " + toLowerCase(trim(header)) + ": " + name);

        /* Record that we've seen this person, since they may have no attributes and the
         * autoinsertion feature below therefore might not add them.
         */
        table.put(name, {});

        /* Extract their attributes. It's possible that they have no attributes, which
         * is not an error.
         */
        string attributes;
        getline(input, attributes);

        for (auto attribute: stringSplit(attributes, ",")) {
            attribute = trim(attribute);
            if (attribute.empty()) error("Empty attribute?");

            /* We're okay with duplicate attributes. */
            table[name] += attribute;
        }
    }

    /* Lays out a single column. The X coordinate of the left side of the boxes is given
     * as input, along with the total window height.
     */
    void layOutColumn(const HashSet<string>& nodes, HashMap<string, GRectangle>& result,
                      double leftX, double height) {
        /* Edge case: If there are no nodes, don't do anything. */
        if (nodes.size() == 0) return;

        /* Edge case: If there is one node, center it vertically. */
        if (nodes.size() == 1) {
            result[nodes.first()] = { leftX, (height - kBoxHeight) / 2.0, kBoxWidth, kBoxHeight };
            return;
        }

        /* Adjust for the top/bottom margins. */
        double columnHeight = height - 2 * kVerticalPadding;

        /* Compute the spacing between boxes. */
        double boxSpacing = (columnHeight - kBoxHeight * nodes.size()) / (nodes.size() - 1);

        /* The base Y coordinate is the quantity that centers all these boxes. */
        double topY = (height - columnHeight) / 2.0;

        /* Lay out all the nodes. */
        int index = 0;
        for (const auto& node: nodes) {
            result[node] = { leftX, topY + (kBoxHeight + boxSpacing) * index, kBoxWidth, kBoxHeight };
            index++;
        }
    }

    /* Given a graph, determines the location of each node. */
    HashMap<string, GRectangle> layOutNodes(GWindow& window, const BipartiteGraph& graph) {
        /* Determine the locations of the left and right columns. The entire
         * arrangement will be centered in the window with the appropriate
         * buffer spacing between.
         */
        double centerX = window.getCanvasWidth() / 2.0;
        double height  = window.getCanvasHeight();

        HashMap<string, GRectangle> result;
        layOutColumn(graph.lhs, result, centerX - kColumnSpacing / 2.0 - kBoxWidth, height);
        layOutColumn(graph.rhs, result, centerX + kColumnSpacing / 2.0, height);

        return result;
    }

    /* Given a pair, extracts the doctor from that pair. Since the doctor names are always
     * prefixed with "Doctor", which lexicographically precedes "Patient," this is always
     * the lower of the two pair elements.
     */
    string doctorIn(const UnorderedPair& pair) {
        return min(pair.first, pair.second);
    }
    string patientIn(const UnorderedPair& pair) {
        return max(pair.first, pair.second);
    }

    /* Draw edges with the specified graphics properties. */
    void drawEdges(GWindow& window, const HashSet<UnorderedPair>& edges,
                   const HashMap<string, GRectangle>& nodeLocations,
                   const string& color, double thickness) {
        GLine line(0, 0, 0, 0);
        line.setLineWidth(thickness);
        line.setColor(color);
        for (const auto& edge: edges) {
            /* Line originates on the right-hand side of the doctor node and is vertically
             * centered.
             */
            auto doctorPos = nodeLocations[doctorIn(edge)];
            double x0 = doctorPos.getX() + doctorPos.getWidth();
            double y0 = doctorPos.getY() + doctorPos.getHeight() / 2.0;

            /* Line ends on the left-hand side of the patient node and is vertically
             * centered.
             */
            auto patientPos = nodeLocations[patientIn(edge)];
            double x1 = patientPos.getX();
            double y1 = patientPos.getY() + patientPos.getHeight() / 2.0;

            line.setLocation(x0, y0);
            line.setEndPoint(x1, y1);
            window.draw(&line);
        }
    }

    /* Draws each node in a nice pretty box. */
    void drawNodes(GWindow& window, const HashSet<string>& nodes,
                   const HashMap<string, GRectangle>& nodeLocations,
                   const string& boxColor, const string& borderColor,
                   const string& font, const string& fontColor) {
        /* A single GLabel, to minimize round-trip costs. */
        GLabel label;
        label.setFont(font);
        label.setColor(fontColor);

        for (const auto& node: nodes) {
            auto location = nodeLocations[node];

            /* Draw the background. */
            window.setColor(boxColor);
            window.fillRect(location);

            /* Draw the border. */
            window.setColor(borderColor);
            window.drawRect(location);

            /* Draw the text. */
            label.setLabel(node);

            double x = location.getX() + (location.getWidth() - label.getWidth()) / 2.0;
            double y = location.getY() + (location.getHeight() + label.getFontAscent()) / 2.0;

            window.draw(&label, x, y);
        }
    }

    /* Returns a new stream that strips out all blank lines and comments from the original. */
    istringstream sanitize(istream& input) {
        ostringstream builder;

        for (string line; getline(input, line); ) {
            /* Strip comments, if any. */
            size_t commentStart = line.find('#');
            if (commentStart != string::npos) {
                line = line.substr(0, commentStart);
            }

            /* Trim whitespace. */
            line = trim(line);

            if (!line.empty()) builder << line << endl;
        }

        return istringstream(builder.str());
    }
}

/* We use the .hospital suffix. */
string DoctorsWithoutOrdersII::fileExtension() {
    return ".hospital";
}

DoctorsWithoutOrdersII::DoctorsWithoutOrdersII(istream& source, GWindow& window)
  : solveButton(new GButton(kSolveButtonText), window, "NORTH")
{
    auto input = sanitize(source);

    HashMap<string, HashSet<string>> doctors;  // Name --> Skills
    HashMap<string, HashSet<string>> patients; // Name --> Requirements

    for (string line; getline(input, line); ) {
        /* See what kind of line this is. */
        if (startsWith(line, "Doctor "))  parseLine(line, "Doctor ", doctors);
        else if (startsWith(line, "Patient ")) parseLine(line, "Patient ", patients);
        else error("Unknown declaration in file.");
    }

    /* Construct a graph associating each doctor with each patient for whom they have all
     * the necessary skills.
     */
    for (auto doctor: doctors) graph.lhs += doctor;
    for (auto patient: patients) graph.rhs += patient;

    for (auto doctor: doctors) {
        for (auto patient: patients) {
            if (doctors[doctor].containsAll(patients[patient])) {
                graph.edges += { doctor, patient };
            }
        }
    }
}

void DoctorsWithoutOrdersII::repaint(GWindow& window) {
    /* Clear display. */
    window.setColor(kBackgroundColor);
    window.fillRect(0, 0, window.getCanvasWidth(), window.getCanvasHeight());

    /* Map each node to its bounding box. */
    auto nodeLocations = layOutNodes(window, graph);

    /* Determine which nodes are matched. */
    HashSet<string> matchedNodes;
    for (const auto& edge: matching) {
        matchedNodes += edge.first;
        matchedNodes += edge.second;
    }

    /* First draw the unmatched edges, then draw the matched edges, to improve
     * visibility.
     */
    drawEdges(window, graph.edges - matching, nodeLocations, kUnusedLineColor, kUnusedLineThickness);
    drawEdges(window, matching, nodeLocations, kMatchedLineColor, kMatchedLineThickness);

    /* Draw matched and unmatched nodes separately for efficiency (minimize graphics calls). */
    drawNodes(window, (graph.lhs + graph.rhs) - matchedNodes, nodeLocations,
              kUnusedBoxColor, kUnusedBoxBorderColor, kUnusedBoxFont, kUnusedBoxFontColor);
    drawNodes(window, matchedNodes, nodeLocations,
              kMatchedBoxColor, kMatchedBoxBorderColor, kMatchedBoxFont, kMatchedBoxFontColor);

    window.repaint();
}

/* React to the "Solve" button being pressed. */
void DoctorsWithoutOrdersII::actionPerformed(GObject* source) {
    if (source == solveButton.get()) {
        matching = matchmake(graph);
        requestRepaint();
    }
}
