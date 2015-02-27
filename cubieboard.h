#ifndef CUBIEBOARD_H
#define CUBIEBOARD_H

#include "boarddescription.h"


// The Cubieboard (http://linux-sunxi.org/Cubieboard)

BoardDescription makeBoard()
{
    BoardDescription b;

    b.name = "cubieboard";

    // Board size
    b.size[0] = 99.9;
    b.size[1] = 60.0;
    b.thickness = 1.6; // Add about +.2 to the actual board size

    // Board holes
    b.holes = {
        {3.3, 3.5}, {3.3, b.size[1] - 3.5},
        { 77, 3.5}, { 77, b.size[1] - 3.5}
    };
    b.holesRadius = 1.5; // = M3

    // Each area marks a cuboid in which no stuff should be placed (i.e., where some parts are on the board)
    b.bottomForbiddenAreas = {
        { 6.5,  1.1,    49.0,  5.0,  2.5}, // GPIO pins 1 - black part
        { 7.5,  1.5,    47.0,  4.2,  6.0}, // GPIO pins 1 - pins
        { 6.5, 54.2,    49.0,  5.0,  2.5}, // GPIO pins 2 - black part
        { 7.5, 54.6,    47.0,  4.2,  6.0}, // GPIO pins 2 - pins
        {77.0, 41.5,    21.5, 12.5,  1.4}, // Flash chip
        {57.5,  1.5,    15.5,  4.5,  2.0}, // Pins of the SATA port
        {81.0,  1.5,     5.5,  4.5,  1.5}, // Pins of the IR diode
        { 0.0, 14.0,    13.0,  7.5,  5.5}, // Audio port
        {-2.0, 27.0,     5.5,  7.5,  4.5}, // Switch
    };
    b.topForbiddenAreas = {
        { 6.5,  1.1,    49.0,  5.0,  2.5}, // GPIO pins 1 - pins with solder
        { 6.5, 54.2,    49.0,  5.0,  2.5}, // GPIO pins 2 - pins with solder
        { 1.0,  7.0,    49.0,  9.0,  1.0}, // Some resistors
        {   0, 36.0,    12.0, 17.5, 14.5}, // Ethernet port
        {56.0,  0.0,    18.0,  8.0, 11.0}, // SATA port
        {73.5,  7.0,    10.0,  7.0,  9.0}, // SATA power
        {81.0,  0.0,     6.0,  7.5, 11.0}, // IR diode
        {79.5, 43.5,    17.5, 18.5,  2.0}, // microSD slot incl. space for card
        {88.0, 25.0,    12.6, 15.5,  7.0}, // HDMI port
        {87.0,  0.5,    14.6, 10.0,  6.5}, // Power port
        {96.0, 18.0,     5.5,  4.5,  4.5}, // Switch
        { 0.2, 21.0,    10.0, 16.0,  1.0}  // Solder of Audio and MiniUSB ports
    };

    // The board ports (connections).
    b.bottomPorts = {
        // Audio (Line-In)
        {
            West, {
                {18.0, 2.6},
                {18.0,  -3} // avoid bridge
            },
            2.9,
            1.0
        }
    };
    b.topPorts = {
        // Audio (Line-Out)
        {
            West, {
                {18.0, 2.6},
                {18.0,  -3} // avoid bridge
            },
            2.9,
            1.0
        },
        // MiniUSB
        {
            West, {
                {27.0, 3.5},
                {33.0, 3.5},
                {33.0,  -3}, // avoid bridge
                {27.0,  -3}
            },
            1.0,
            2.0
        },
        // Ethernet (Including the housing, as it protrudes) (This should not be required since it's already covered by "forbidden areas", but for the sake of completeness...)
        {
            West, {
                {36.5, 14.0},
                {53.0, 14.0},
                {53.0,   -3}, // avoid bridge
                {36.5,   -3}
            },
            .5, // small radius
            10 // full outset
        },
        // USB (2x)
        {
            North, {
                {57.5,   -3}, // avoid bridge
                {57.5, 15.5},
                {71.0, 15.5},
                {71.0,   -3} // avoid bridge
            },
            1.0, // radius
            2.0  // outset
        },
        // TODO: SD-card slot. Could be a bit hard to get nice.
        // HDMI
        {
            East, {
                {27.0, 4.0},
                {38.5, 4.0},
                {38.5,  -3}, // avoid bridge
                {27.0,  -3}
            },
            2.0, // radius
            2.0  // outset
        },
        // Power
        {
            East, {
                {6.5, 3.0},
                {6.5,  -3} // avoid bridge
            },
            3.0, // radius
            1.0  // outset
        }
    };

    b.topWallSupports = {
        {West, 22.0, 4.0, 5.0},
        {West, 34.0, 19.0, 5.0},
        {East, 10.5, 5.5, 2.0},
        {East, 24, 1, 2.0}
    };

    return b;
}


#endif // CUBIEBOARD_H
