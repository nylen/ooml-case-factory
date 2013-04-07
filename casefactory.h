#ifndef CASEFACTORY_H
#define CASEFACTORY_H

#include <ooml/components.h>
#include <ooml/core/Hull.h>
#include <ooml/core/Intersection.h>
#include "geom.h"
#include "boarddescription.h"


// Small epsilon value for building differences where positive and negative parts have (partly) common faces.
// Should be small (<= .1mm) but not too small (>= .01mm).
// It does NOT affect the result unless set way too large (I suggest to not set it greater than 1mm).
static const double eps = .1;



struct CaseFactory
{
    CaseFactory(BoardDescription board);

    enum Side {
        BottomSide,
        TopSide
    };


    // Case dimension parameters:

    // case thickness
    double walls = 3.0;
    double floors = 2.0;

    // case holes at same position than the board holes, here for M3 screws
    double holesAddRadiusLoose = .4; // The screw needs to slide through the hole (but not its head)
    double holesAddRadiusTight = .0; // The screw needs to hold in the hole
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
    Component constructBottom();

    Component constructTop();

    // Calculate the final dimension of the assembled case
    Vec outerDimensions();


private:
    BoardDescription board;

    enum {
        ExtensionInside = 0,
        ExtensionOutside = 1
    };

    inline double bottomHeight() { return bottomInnerHeight + floors; }
    inline double topHeight() { return topInnerHeight + floors; }
    inline double extensionHeight() { return board.thickness; }
    inline double totalHeight() { return bottomHeight() + topHeight() + extensionHeight(); }

    // Returns the added size on each border in xy directions to the board's dimensions. (Case size = board size + 2 * outset)
    inline double outset() { return walls + space; }

    // Returns the case's size (see outset()).
    inline double outerWidth() { return board.size[0] + 2 * outset(); }
    inline double outerDepth() { return board.size[1] + 2 * outset(); }

    // Puts all things together required to build one part of the case (top / bottom)
    Component constructPart(Side whichSide);

    // Wall extension: 0 = on the inner half of the wall, 1 = on the outer half of the wall
    Component constructBase(double innerHeight, int extensionDirection);

    Component addWallSupport(const Component & component, double supportHeight, const WallSupportDescription & wallSupport);
    Component addHoleForScrew(const Component & component, double partOuterHeight, const Point & pos, double radius, bool screwHead);
    Component addHoleForPort(const Component & component, double partOuterHeight, const PortDescription & port);
};


#endif // CASEFACTORY_H
