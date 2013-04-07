#ifndef BOARDDESCRIPTION_H
#define BOARDDESCRIPTION_H

#include <vector>
#include "geom.h"

// An instance of this class describes an external IO-port on a board.
// It is used by the case factory to cut a hole on the side of the case.
struct PortDescription {
    enum Side {
        North, East, South, West
    };
    Side side;               // NESW: 0 = +Y, 1: +X, 2: -Y, 3: -X
    std::vector<Point> path; // Polygon with points (x,z) / (y,z) with z relative to board surface (bottom/top)
    double radius;           // of hulled cylinder
    double outset;           // at which distance from the board's border the diagonal-shaped corner starts (a cone is added to the cylinder)
};

// An instance of this class describes a cuboid-shaped area above or
// below the board's surface in which no case should be placed.
// These areas will be subtracted from the case model by the factory.
struct ForbiddenAreaDescription {
    // All numbers positive, z relative to board surface (bottom/top)
    double x;
    double y;
    double sx;
    double sy;
    double sz;
};


// An instance of this class describes an electronic PCB board with all
// informations required by the case factory to construct a case for it.
// These informations cover the PCB dimensions, holes for screws, external
// IO-ports as well as additional "forbidden areas". See also the other
// classes which are used by this class.
struct BoardDescription
{
    // PCB board dimensions
    double size[2] = {0.0, 0.0};
    double thickness = 0.0;

    // PCB board holes for screws
    std::vector<Point> holes;
    double holesRadius = 0.0;

    // Each area marks a cuboid in which no stuff should be placed (i.e., where some parts are on the board)
    std::vector<ForbiddenAreaDescription> bottomForbiddenAreas;
    std::vector<ForbiddenAreaDescription> topForbiddenAreas;

    // The board ports (connections).
    std::vector<PortDescription> bottomPorts;
    std::vector<PortDescription> topPorts;
};

#endif // BOARDDESCRIPTION_H
