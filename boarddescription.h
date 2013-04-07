#ifndef BOARDDESCRIPTION_H
#define BOARDDESCRIPTION_H

#include <vector>
#include "geom.h"


struct Port {
    enum Side {
        North, East, South, West
    };
    Side side;               // NESW: 0 = +Y, 1: +X, 2: -Y, 3: -X
    std::vector<Point> path; // Polygon with points (x,z) / (y,z) with z relative to board surface (bottom/top)
    double radius;           // of hulled cylinder
    double outset;           // at which distance from the board's border the diagonal-shaped corner starts (a cone is added to the cylinder)
};

struct ForbiddenArea {
    // all numbers positive, z relative to board surface (bottom/top)
    double x;
    double y;
    double sx;
    double sy;
    double sz;
};



struct BoardDescription
{
    // Board dimensions
    double size[2] = {0.0, 0.0};
    double thickness = 0.0;

    // Board holes
    std::vector<Point> holes;
    double holesRadius = 0.0;

    // Each area marks a cuboid in which no stuff should be placed (i.e., where some parts are on the board)
    std::vector<ForbiddenArea> bottomForbiddenAreas;
    std::vector<ForbiddenArea> topForbiddenAreas;

    // The board ports (connections).
    std::vector<Port> bottomPorts;
    std::vector<Port> topPorts;
};

#endif // BOARDDESCRIPTION_H
