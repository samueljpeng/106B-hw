#include "Floodplain.h"
#include "error.h"
#include <fstream>
using namespace std;

Floodplain loadFloodplain(istream& input) {
    int numRows, numCols;
    if (input >> numRows >> numCols, !input) error("Can't read rows/cols.");

    int numSources;
    if (input >> numSources, !input) error("Can't read number of sources.");

    /* Read the flooding sources. */
    Floodplain result;
    for (int i = 0; i < numSources; i++) {
        int row, col;
        if (input >> row >> col, !input) error("Couldn't read a flooding source.");

        result.waterSources.add({ row, col });
    }

    /* Read the height data. */
    result.heights.resize(numRows, numCols);
    for (int row = 0; row < result.heights.numRows(); row++) {
        for (int col = 0; col < result.heights.numCols(); col++) {
            if (input >> result.heights[row][col], !input) error("Can't read height data.");
        }
    }

    char leftover;
    if (input >> leftover) error("Unexpected contents found at end of file.");

    return result;
}


void saveFloodplain(const string& filename, const Floodplain& floodplain) {
    ofstream output(filename);
    output << floodplain.heights.numRows() << " " << floodplain.heights.numCols() << endl;
    output << floodplain.waterSources.size() << endl;

    for (auto source: floodplain.waterSources) {
        output << "  " << source.row << " " << source.col << endl;
    }

    for (int row = 0; row < floodplain.heights.numRows(); row++) {
        for (int col = 0; col < floodplain.heights.numCols(); col++) {
            output << floodplain.heights[row][col] << " ";
        }
        output << endl;
    }

    if (!output) error("File save error.");
}
