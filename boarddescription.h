#ifndef BOARDDESCRIPTION_H
#define BOARDDESCRIPTION_H

#include <vector>
#include "geom.h"


enum Side {
    North,  // positive Y
    East,   // positive X
    South,  // negative Y
    West    // negative X
};
inline Vec sidePositiveTangentialVector(Side side) {
    Vec u = {0.0, 0.0, 0.0};
    if (side == North) { u.x = 1.0; }
    if (side == East)  { u.y = 1.0; }
    if (side == South) { u.x = 1.0; }
    if (side == West)  { u.y = 1.0; }
    return u;
}

// An instance of this class describes an external IO-port on a board.
// It is used by the case factory to cut a hole on the side of the case.
struct PortDescription {
    Side side;
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

struct WallSupportDescription {
    Side side;
    double pos;   // Same as the x-coorindate of a point of the path in a PortDescription
    double size;  // Entent in the same direction as pos.
    double inset; // The length of the support towards the inside of the case (0 = at the board's border). Forbidden areas have to be inserted properly.
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

    // Some wall supports where the walls would be too tiny otherwise
    // TODO: Maybe this can be calculated automatically?
    std::vector<WallSupportDescription> bottomWallSupports;
    std::vector<WallSupportDescription> topWallSupports;
};

#endif // BOARDDESCRIPTION_H
