#ifndef Floodplain_Included
#define Floodplain_Included

#include "grid.h"
#include "point.h"
#include "vector.h"
#include <string>
#include <istream>

/* Type representing a floodplain. */
struct Floodplain {
    Grid<double> heights;              // Height of each point on the map, in meters.
    Vector<GridLocation> waterSources; // Which locations, if any, are water sources.
};

/* Reads a floodplain from the specified input stream. */
Floodplain loadFloodplain(std::istream& input);

/* Writes a Floodplain to a file. */
void saveFloodplain(const std::string& filename, const Floodplain& floodplain);

#endif
