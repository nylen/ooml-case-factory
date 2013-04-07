#ifndef CUBIEBOARD_H
#define CUBIEBOARD_H

#include <vector>
#include "geom.h"

namespace CubieBoard {
namespace Measures {

struct ForbiddenArea {
    double x;
    double y;
    double sx;
    double sy;
    double sz;
};

struct Port {
    enum Side {
        North, East, South, West
    };
    Side side; // NESW: 0 = +Y, 1: +X, 2: -Y, 3: -X
    std::vector<Point> path; // Polygon with points (x,z) / (y,z) with z relative to board surface (bottom/top)
    double radius; // of hulled cylinder
    double outset; // at which distance from the board's border the diagonal-shaped corner starts (a cone is added to the cylinder)
};

// board size
double size[2] = {99.9, 60.0};
double thickness = 1.5;

// board holes
Point holes[] = {
    {3.3, 3.5}, {3.3, size[1] - 3.5},
    { 77, 3.5}, { 77, size[1] - 3.5}
};
double holesRadius = 1.5; // = M3

// Each area marks a cuboid in which no stuff should be placed (i.e., where some parts are on the board)
// encoded in x, y,  size_x, size_y, size_z  (all numbers positive, z relative to board surface (bottom/top))
std::vector<ForbiddenArea> bottomForbiddenAreas = {
    { 6.5,  1.1,    49.0,  5.0,  2.5}, // GPIO pins 1 - black part
    { 7.5,  1.5,    47.0,  4.2,  6.0}, // GPIO pins 1 - pins
    { 6.5, 54.2,    49.0,  5.0,  2.5}, // GPIO pins 2 - black part
    { 7.5, 54.6,    47.0,  4.2,  6.0}, // GPIO pins 2 - pins
    {77.0, 41.5,    21.5, 12.5,  1.4}, // Flash chip
    {57.5,  1.5,    15.5,  4.5,  2.0}, // Pins of the SATA port
    {81.0,  1.5,     5.5,  4.5,  1.5}, // Pins of the IR diode
    {  .0, 14.0,    13.0,  7.5,  5.5}, // Audio port  // FIXME
};
std::vector<ForbiddenArea> topForbiddenAreas = {
    {  -4, 36.5,    12.0, 16.5, 13.5}, // Ethernet port
    {56.0,  0.0,    18.0,  8.0, 11.0}, // SATA port
    {73.5,  7.0,    10.0,  7.0,  9.0}, // SATA power
    {81.0,  0.0,     6.0,  7.5, 11.0}, // IR diode
    {79.5, 43.5,    15.5, 18.0,  2.0}, // microSD slot incl. space for card
};

// The board ports (connections).
std::vector<Port> bottomPorts = {
    // Audio (Line-In)
    {
        Port::West, {
            {17.8, 2.6},
            {17.8,  -1} // avoid bridge
        },
        2.8,
        2.0
    }
};
std::vector<Port> topPorts = {
    // Audio (Line-Out)
    {
        Port::West, {
            {17.8, 2.6},
            {17.8,  -3} // avoid bridge
        },
        2.8,
        2.0
    },
    // USB Power
    {
        Port::West, {
            {26.5, 3.5},
            {33.5, 3.5},
            {33.5,  -3}, // avoid bridge
            {26.5,  -3}
        },
        1.0,
        2.0
    },
    // Ethernet (Including the housing, as it protrudes) (This should not be required since it's already covered by "forbidden areas", but for the sake of completeness...)
    {
        Port::West, {
            {36.5, 13.5},
            {53.0, 13.5},
            {53.0,   -3}, // avoid bridge
            {36.5,   -3}
        },
        0, // not rounded
        10 // full outset
    },
    // USB (2x)
    {
        Port::North, {
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
        Port::East, {
            {25.5,  5.5},
            {39.5,  5.5},
            {39.5,  2.5},
            {32.5,  -10}, // avoid bridge (approx 30 degree overhang)
            {25.5,  2.5}
        },
        1.5, // radius
        2.0  // outset
    },
    // Power
    {
        Port::East, {
            {6.5, 3.0},
            {6.5,  -3} // avoid bridge
        },
        2.8, // radius
        2.0  // outset
    }
};

}
}


#endif // CUBIEBOARD_H
