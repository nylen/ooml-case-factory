#include <fstream>
#include <random>
#include <ooml/components.h>
#include <ooml/core/Hull.h>
#include <ooml/core/Intersection.h>
#include <ooml/core/IndentWriter.h>

#include "geom.h"
#include "cubieboard.h"

// Which board to print? Redirect the namespace to the measures of the board for which a case you want to construct.
namespace board = CubieBoard::Measures;


// Small epsilon value for building differences where positive and negative parts have (partly) common faces.
// Should be small (<= .1mm) but not too small (>= .01mm).
// It does NOT affect the result unless set way too large (I suggest to not set it greater than 1mm).
static const double eps = .1;



struct CaseFactory
{
    enum Side {
        BottomSide,
        TopSide
    };



    // Case dimension parameters:

    // case thickness
    double walls = 2.7;
    double floors = 2.0;

    // case holes at same position than the board holes, here for M3 screws
    double holesRadiusLoose = board::holesRadius + .3; // The screw needs to slide through the hole (but not its head)
    double holesRadiusTight = board::holesRadius - .1; // The screw needs to hold in the hole
    double holesSize = 5.8; // size of the cuboid-shaped hole on the bottom case where the screw head fits in (>= screw head diameter)
    double holesWalls = 1.3; // ...and their wall thickness
    double holesFloors = 4.0; // ...and their floor thickness

    // space between board and case in xy dimensions (should be somewhere between 0 and 1)
    double space = .3;

    // rounded corners (only on the outside)
    double cornerRadius = 2;
    double cornerFaces = 20; // should be >= 16.

    // what to build on which side (the contrary part is on the other side)
    Side screwHeadsOnSide = BottomSide;
    Side outerExtensionOnSide = BottomSide;



    // Print parameters:
    double printLevelHeight = .2;
    double printSafeBridgeLayerCount = 1; // Number of layers for safe bridges to be printed (won't have "bridge holes" in this number of layers)




    // TODO: Derive these values from the board dimensions! .5mm less than the largest forbidden area / port dimension.
    // Case height (separately for bottom / top part)
    double bottomInnerHeight = 5.5;
    double topInnerHeight = 16.5;

    // Components factory:
    Component constructBottom()
    {
        return constructPart(BottomSide);
    }

    Component constructTop()
    {
        return constructPart(TopSide);
    }

    // Calculate the final dimension of the assembled case
    Vec outerDimensions() {
        return {outerWidth(), outerDepth(), totalHeight()};
    }


private:

    enum {
        ExtensionInside = 0,
        ExtensionOutside = 1
    };

    double bottomHeight() { return bottomInnerHeight + floors; }
    double topHeight() { return topInnerHeight + floors; }
    double extensionHeight() { return board::thickness; }
    double totalHeight() { return bottomHeight() + topHeight() + extensionHeight(); }

    // Returns the added size on each border in xy directions to the board's dimensions. (Case size = board size + 2 * outset)
    double outset() { return walls + space; }

    // Returns the case's size (see outset()).
    double outerWidth() { return board::size[0] + 2 * outset(); }
    double outerDepth() { return board::size[1] + 2 * outset(); }

    // Puts all things together required to build one part of the case (top / bottom)
    Component constructPart(Side whichSide)
    {
        // Select parameters depending on which part to build
        auto innerHeight     = (whichSide == BottomSide) ? bottomInnerHeight           : topInnerHeight;
        auto outerHeight     = (whichSide == BottomSide) ? bottomHeight()              : topHeight();
        auto forbiddenAreas  = (whichSide == BottomSide) ? board::bottomForbiddenAreas : board::topForbiddenAreas;
        auto ports           = (whichSide == BottomSide) ? board::bottomPorts          : board::topPorts;
        auto extension       = (whichSide == outerExtensionOnSide) ? ExtensionOutside  : ExtensionInside;
        auto screwHoleRadius = (whichSide == screwHeadsOnSide) ? holesRadiusLoose : holesRadiusTight;
        auto screwHeads      = (whichSide == screwHeadsOnSide);

        // We start with the base
        Component c = constructBase(innerHeight, extension);

        // Screw holes
        for (auto hole : board::holes) {
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
            c = c - Cube(area.sx, area.sy, area.sz + eps, false)
                    .translatedCopy(area.x, area.y, outerHeight - area.sz);
        }

        // If this is the top, we've just built it mirrored. So we mirror the y axis and move it so it matches the dimensions of the bottom part.
        if (whichSide == TopSide) {
            c.scale(1.0, -1.0, 1.0);
            c.translate(0.0, board::size[1], 0.0);
        }

        return c;
    }

    // Wall extension: 0 = on the inner half of the wall, 1 = on the outer half of the wall
    Component constructBase(double innerHeight, int extensionDirection)
    {
        // The walls and the floor are made by subtracting two cuboids.
        Component base = Cube(board::size[0] + 2*outset(), board::size[1] + 2*outset(), innerHeight + floors, false)
                .translatedCopy(-outset(), -outset(), 0);
        Component baseInner = Cube(board::size[0] + 2*space,  board::size[1] + 2*space,  innerHeight + eps, false)
                .translatedCopy(-space, -space, floors);
        base = base - baseInner;

        // Extension (half width wall goes a bit higher, either the inner or the outer half depending on extensionDirection)
        double off_ext_outer = walls * (extensionDirection ? 1.00 : 0.45) + space;
        double off_ext_inner = walls * (extensionDirection ? 0.55 : 0.00) + space;
        Component extension = Cube(board::size[0] + 2*off_ext_outer, board::size[1] + 2*off_ext_outer, extensionHeight() + eps, false)
                .translatedCopy(-off_ext_outer, -off_ext_outer, innerHeight + floors - eps);
        Component extensionInner = Cube(board::size[0] + 2*off_ext_inner, board::size[1] + 2*off_ext_inner, extensionHeight() + 3 * eps, false)
                .translatedCopy(-off_ext_inner, -off_ext_inner, innerHeight + floors - 2 * eps);
        extension = extension - extensionInner;

        // Combine both
        return base + extension;
    }

    Component addHoleForScrew(const Component & component, double partOuterHeight, const Point & pos, double radius, bool screwHead)
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

    Component addHoleForPort(const Component & component, double partOuterHeight, const board::Port & port)
    {
        double off_xy = walls + space;

        // u is the vector facing in the positive axis parallel to the side (orthogonal to both n and the z axis)
        // v is the vector which is used for the port's local point's y coordinate and faces away from the board surface.
        Vec u = {0, 0, 0}, v = {0, 0, -1};
        if (port.side == board::Port::North) { u.x = 1; }
        if (port.side == board::Port::East)  { u.y = 1; }
        if (port.side == board::Port::South) { u.x = 1; }
        if (port.side == board::Port::West)  {  u.y = 1; }

        // The base position of a port, if it has a local point of (0,0). This is the "origin" of the side.
        Vec base = {0, 0, partOuterHeight};
        if (port.side == board::Port::North) { base.y = board::size[1]; }
        if (port.side == board::Port::East)  { base.x = board::size[0]; }

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
};





int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;


    CaseFactory factory;
    factory.printLevelHeight = .2;
    factory.printSafeBridgeLayerCount = 3;

    // Only the bottom
    /*
    Component scene = factory.constructBottom();
    */

    // Only the top
    Component scene = factory.constructTop();

    // Both parts side by side
    /*
    double distance = 5; // mm
    Component scene = factory.constructBottom()
            + factory.constructTop()
            .translatedCopy(0, factory.outerDimensions().y + distance, 0);
    */

    // Both parts as assembled
    /*
    double distance = 0.1; // mm
    Component scene = factory.constructBottom()
            + factory.constructTop()
            .rotatedCopy(180.0, 0, 0)
            .translatedCopy(0, board::size[1], factory.outerDimensions().z + distance);
    */


    IndentWriter writer;
    writer << scene;
    std::ofstream outFile;
    outFile.open(argv[1]);
    outFile << writer;

    return 0;
}


