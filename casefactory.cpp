#include "casefactory.h"

CaseFactory::CaseFactory(BoardDescription board) :
    board(board)
{
}


Component CaseFactory::constructBottom()
{
    return constructPart(BottomSide);
}


Component CaseFactory::constructTop()
{
    return constructPart(TopSide);
}


Vec CaseFactory::outerDimensions()
{
    return {outerWidth(), outerDepth(), totalHeight()};
}


Component CaseFactory::constructPart(Side whichSide)
{
    // Select parameters depending on which part to build
    auto innerHeight     = (whichSide == BottomSide) ? bottomInnerHeight           : topInnerHeight;
    auto outerHeight     = (whichSide == BottomSide) ? bottomHeight()              : topHeight();
    auto forbiddenAreas  = (whichSide == BottomSide) ? board.bottomForbiddenAreas : board.topForbiddenAreas;
    auto ports           = (whichSide == BottomSide) ? board.bottomPorts          : board.topPorts;
    auto extension       = (whichSide == outerExtensionOnSide) ? ExtensionOutside  : ExtensionInside;
    auto screwHoleRadius = (whichSide == screwHeadsOnSide) ? (board.holesRadius + holesAddRadiusLoose) : (board.holesRadius + holesAddRadiusTight);
    auto screwHeads      = (whichSide == screwHeadsOnSide);

    // We start with the base
    Component c = constructBase(innerHeight, extension);

    // Screw holes
    for (auto hole : board.holes) {
        c = addHoleForScrew(c, outerHeight, hole, screwHoleRadius, screwHeads);
    }

    // Apply rounded corners (if enabled)
    if (cornerRadius > 0.0) {
        // Class "RoundedCube" of ooml is buggy as it doesn't respect the "faces" parameter.
        // So we construct our own rounded cuboid by taking the convex hull of eight spheres.
        Vec min = {-outset(), -outset(), 0};
        Vec max = outerDimensions() + min;
        Vec cornerOffset = {cornerRadius, cornerRadius, cornerRadius};
        min += cornerOffset;
        max -= cornerOffset;
        CompositeComponent roundedCorners = Hull::create();
        for (int corner = 0; corner < 8; corner++) {
            double x = (corner & (1 << 0)) ? min.x : max.x;
            double y = (corner & (1 << 1)) ? min.y : max.y;
            double z = (corner & (1 << 2)) ? min.z : max.z;
            roundedCorners.addComponent(Sphere(cornerRadius, cornerFaces).translatedCopy(x, y, z));
        }

        // Intersect the current part with the rounded cuboid
        c = c * roundedCorners;
    }

    // Port holes
    for (auto port : ports) {
        c = addHoleForPort(c, outerHeight, port);
    }

    // "Forbidden areas" of the board
    for (auto area : forbiddenAreas) {
        c = c - Cube(area.sx, area.sy, area.sz + extensionHeight() + eps, false)
                .translatedCopy(area.x, area.y, outerHeight - area.sz);
    }

    // If this is the top, we've just built it mirrored. So we mirror the y axis and move it so it matches the dimensions of the bottom part.
    if (whichSide == TopSide) {
        c.scale(1.0, -1.0, 1.0);
        c.translate(0.0, board.size[1], 0.0);
    }

    return c;
}


Component CaseFactory::constructBase(double innerHeight, int extensionDirection)
{
    // The walls and the floor are made by subtracting two cuboids.
    Component base = Cube(board.size[0] + 2*outset(), board.size[1] + 2*outset(), innerHeight + floors, false)
            .translatedCopy(-outset(), -outset(), 0);
    Component baseInner = Cube(board.size[0] + 2*space,  board.size[1] + 2*space,  innerHeight + eps, false)
            .translatedCopy(-space, -space, floors);
    base = base - baseInner;

    // Extension (half width wall goes a bit higher, either the inner or the outer half depending on extensionDirection)
    double off_ext_outer = walls * (extensionDirection ? 1.00 : 0.45) + space;
    double off_ext_inner = walls * (extensionDirection ? 0.55 : 0.00) + space;
    Component extension = Cube(board.size[0] + 2*off_ext_outer, board.size[1] + 2*off_ext_outer, extensionHeight() + eps, false)
            .translatedCopy(-off_ext_outer, -off_ext_outer, innerHeight + floors - eps);
    Component extensionInner = Cube(board.size[0] + 2*off_ext_inner, board.size[1] + 2*off_ext_inner, extensionHeight() + 3 * eps, false)
            .translatedCopy(-off_ext_inner, -off_ext_inner, innerHeight + floors - 2 * eps);
    extension = extension - extensionInner;

    // Combine both
    return base + extension;
}


Component CaseFactory::addHoleForScrew(const Component & component, double partOuterHeight, const Point & pos, double radius, bool screwHead)
{
    // The "radius" of the cuboid. Maybe a cylindrical shape is nicer?
    double ri = holesSize / 2.0;
    double ro = holesSize / 2.0 + holesWalls;

    // Hole cuboid
    Component cuboid = Cube(ro*2, ro*2, partOuterHeight, false)
            .translatedCopy(pos.x - ro, pos.y - ro, 0);
    if (screwHead) {
        Component cuboidInner = Cube(ri*2, ri*2, partOuterHeight - holesFloors + eps, false)
                .translatedCopy(pos.x - ri, pos.y - ri, -eps);
        cuboid = cuboid - cuboidInner;
    }

    // Hole itself
    double holeStart;
    if (screwHead)
        holeStart = (partOuterHeight - holesFloors) + (printLevelHeight * printSafeBridgeLayerCount);
    else
        holeStart = floors;
    Component hole = Cylinder(radius, partOuterHeight - holeStart + eps, 32, false)
            .translatedCopy(pos.x, pos.y, holeStart);

    // Combine them on the existing component
    return component + cuboid - hole;
}


Component CaseFactory::addHoleForPort(const Component & component, double partOuterHeight, const PortDescription & port)
{
    double off_xy = walls + space;

    // u is the vector facing in the positive axis parallel to the side (orthogonal to both n and the z axis)
    // v is the vector which is used for the port's local point's y coordinate and faces away from the board surface.
    Vec u = {0, 0, 0}, v = {0, 0, -1};
    if (port.side == PortDescription::North) { u.x = 1; }
    if (port.side == PortDescription::East)  { u.y = 1; }
    if (port.side == PortDescription::South) { u.x = 1; }
    if (port.side == PortDescription::West)  {  u.y = 1; }

    // The base position of a port, if it has a local point of (0,0). This is the "origin" of the side.
    Vec base = {0, 0, partOuterHeight};
    if (port.side == PortDescription::North) { base.y = board.size[1]; }
    if (port.side == PortDescription::East)  { base.x = board.size[0]; }

    // The hole is a hull of translated cylinders, so the hole is a rounded shape with radius port.radius along port.path [If the radius is 0, we use a tiny cylinder with 4 faces]
    Component cyl = Cylinder(std::max(port.radius, .001), off_xy + 2 * eps, port.radius == 0 ? 4 : 32, false);
    cyl.translate(0, 0, -eps);
    cyl.rotateEulerZXZ(0.0, -90.0, -90.0 * port.side);

    // Add a cone for diagonal borders of the port hole
    double coneLength = off_xy - port.outset;
    Component cone = Cylinder(port.radius, port.radius + coneLength + 2 * eps, coneLength + 2 * eps, 32, false);
    cone.translate(0, 0, port.outset);
    cone.rotateEulerZXZ(0.0, -90.0, -90.0 * port.side);

    // Build convex hull of translated copies of this cylinder along the path of the port
    CompositeComponent cylHull = Hull::create();
    CompositeComponent coneHull = Hull::create();
    for (auto localPoint : port.path)
    {
        Vec p = base + localPoint.x*u + localPoint.y*v;
        Component thisCyl = cyl.translatedCopy(p.x, p.y, p.z);
        Component thisCone = cone.translatedCopy(p.x, p.y, p.z);
        cylHull.addComponent(thisCyl);
        coneHull.addComponent(thisCone);
    }

    return component - (cylHull + coneHull);
}
