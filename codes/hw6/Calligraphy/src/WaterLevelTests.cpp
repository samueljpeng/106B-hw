/*
 * CS106B Assignment 6-A
 * WaterLevelTest.cpp
 *
 * Author: Samuel Peng & Tim Yi
 * Date: 08/15/2018
 *
 * Tests for WaterLevel.cpp
 */
#include "WaterLevelTests.h"
#include "WaterLevel.h"
using namespace std;

ENABLE_TESTS();

ADD_TEST("Entire terrain gets submerged if all points are below water height.") {
    Floodplain plain;
    plain.heights = {
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 }
    };
    plain.waterSources = {
        { 1, 1 }            // Start in the center
    };

    /* If we flood from a height of 1.0, everything should be underwater. */
    auto water = simulateFlood(plain, 1.0);
    Grid<bool> expected = {
        {  true,  true,  true },
        {  true,  true,  true },
        {  true,  true,  true }
    };

    expect(water == expected);
}

ADD_TEST("Entire terrain is dry if all points are below water height.") {
    Floodplain plain;
    plain.heights = {
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 }
    };
    plain.waterSources = {
        { 1, 1 }            // Start in the center
    };

    /* If we flood from a height of -1.0, nothing should be underwater. */
    auto water = simulateFlood(plain, -1.0);
    Grid<bool> expected = {
        { false, false, false },
        { false, false, false },
        { false, false, false }
    };

    expect(water == expected);
}

ADD_TEST("Water floods up to shoreline.") {
    Floodplain plain;
    plain.heights = {
        { -2.0, -1.0, +0.0, +1.0, +2.0 }
    };
    plain.waterSources = {
        { 0, 0 }            // Start from the far left side.
    };

    /* Let's flood the system up to a height of -0.5m. */
    auto water = simulateFlood(plain, -0.5);

    /* The left two entries should be underwater. The remaining entries
     * shouldn't be, because they're above the water line.
     */
    Grid<bool> expected = {
        { true, true, false, false, false }
    };

    expect(water == expected);
}

ADD_TEST("Water doesn't pass through barriers.") {
    Floodplain plain;
    plain.heights = {
        { -2.0, -1.0, +0.0, -1.0, -2.0 }
    };
    plain.waterSources = {
        { 0, 0 }            // Start from the far left side.
    };

    /* Let's flood the system up to a height of -0.5m. */
    auto water = simulateFlood(plain, -0.5);

    /* Although the far right side of the world is below the water line, there's
     * no path for the water to take to flow to those locations, so they should
     * stay high and dry.
     */
    Grid<bool> expected = {
        { true, true, false, false, false }
    };
    expect(water == expected);
}

ADD_TEST("Water flows from multiple sources.") {
    Floodplain plain;
    plain.heights = {
        { 0.0, 1.0, 0.0 },
        { 1.0, 1.0, 1.0 },
        { 0.0, 1.0, 0.0 }
    };
    plain.waterSources = {
        { 0, 0 },  // Water flows from all four corners.
        { 0, 2 },
        { 2, 0 },
        { 2, 2 }
    };

    /* If we flood from a height of 0.5, the corners should be under water */
    auto water = simulateFlood(plain, 0.5);
    Grid<bool> expected = {
        {  true, false,  true },
        { false, false, false },
        {  true, false,  true }
    };

    expect(water == expected);
}

ADD_TEST("Water doesn't flow diagonally.") {
    Floodplain plain;
    plain.heights = {
        { 0.0, 0.0, 0.0, 0.0, 1.0 },
        { 0.0, 0.0, 0.0, 1.0, 0.0 },
        { 0.0, 0.0, 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0, 0.0, 0.0 },
        { 1.0, 0.0, 0.0, 0.0, 0.0 },
    };
    plain.waterSources = {
        { 0, 0 },  // Top-left corner.
    };

    /* The water should not be able to cross the diagonal barrier through the
     * middle of the world.
     */
    auto water = simulateFlood(plain, 0.5);
    Grid<bool> expected = {
        {  true,  true,  true,  true, false },
        {  true,  true,  true, false, false },
        {  true,  true, false, false, false },
        {  true, false, false, false, false },
        { false, false, false, false, false },
    };

    expect(water == expected);
}

ADD_TEST("Given flood point is above sea lever") {
    Floodplain plain;
    plain.heights = {
        { 0.0, 1.0, 0.0 },
        { 1.0, 1.0, 1.0 },
        { 0.0, 1.0, 0.0 }
    };
    plain.waterSources = {
        { 1, 1 }
    };

    /* If we flood from a height of 0.5, the corners should be under water */
    auto water = simulateFlood(plain, 0.5);
    Grid<bool> expected = {
        { false, false, false },
        { false, false, false },
        { false, false, false }
    };

    expect(water == expected);
}

ADD_TEST("Downward Slope") {
    Floodplain plain;
    plain.heights = {
        { 4.0, 3.0, 2.0 },
        { 3.0, 2.0, 1.0 },
        { 2.0, 1.0, 0.0 }
    };
    plain.waterSources = {
        { 0, 0 }
    };

    /* If we flood from a height of 0.5, the corners should be under water */
    auto water = simulateFlood(plain, 4.0);
    Grid<bool> expected = {
        { true, true, true },
        { true, true, true },
        { true, true, true }
    };

    expect(water == expected);
}
