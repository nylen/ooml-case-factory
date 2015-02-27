#ifndef BBATXRABOARD_H
#define BBATXRABOARD_H

#include "boarddescription.h"


// The Cytron BB-ATXRA ATX Breakout Board Case (http://www.thingiverse.com/thing:156795)
// By: Anthony W. Rainer <pristine.source@gmail.com>
// All measured as if the board were turned so the switch is at the bottom and the ATX connector is to the right
// All metric nut sizes from: http://www.fairburyfastener.com/xdims_metric_nuts.htm

BoardDescription makeBoard()
{
    BoardDescription b;

    float bd = 0.5; // buffer distance to give it some breathing room around the board
    // Board size
    b.size[0] = 43.6 + (bd*2); // x
    b.size[1] = 76.65 + (bd*2); // y
    b.thickness = 1.6 + 0.2; // z, Add about +.2 to the actual board size

    // Board holes
    b.holes = {
        //x, y
        { 5.0+bd, 5.0+bd },    // bottom-left
        { 5.0+bd, 71.0+bd },    // top-left
        { 38.5+bd, 5.0+bd },    // bottom-right
        { 38.5+bd, 71.0+bd }    // top-right
    };
    b.holesRadius = 1.6; // 1.5 = M3, +0.1 to allow screw to move

    b.holeNuts = {
    // index, width, thinkness, distance, insert_side
        {0, 5.5, 2.5, 7.0, East},
    {1, 5.5, 2.5, 7.0, East},
    {2, 5.5, 2.5, 7.0, North},
    {3, 5.5, 2.5, 7.0, South}
    };

    // Each area marks a cuboid in which no stuff should be placed (i.e., where some parts are on the board)
    b.bottomForbiddenAreas = {
    // x, y, sizex, sizey, sizez
        { 13.5+bd, 66.4+bd, 2.5, 5.2, 1.5 },     // Power LED Pins
        { 24.4+bd, 66.4+bd, 2.5, 5.2, 1.5 },     // Standby LED Pins
        { 5.0+bd, 13.25+bd, 13.75, 6.4, 2.0 },     // Power Switch Pins
        { 21.4+bd, 13.4+bd, 8.75, 49.1, 2.5 },     // ATX Connector Pins
        { 29.6+bd, 34.6+bd, 12.6, 2.5, 1.5 },     // Top Resistor Pins
        { 29.6+bd, 9.1+bd, 12.6, 2.5, 1.5 },     // Bottom Resitor Pins
    { 4.5+bd, 12.6+bd, 5.0, 51.0, 3.0 },     // Breakout Connector Pins
    };
    b.topForbiddenAreas = {
        // x, y, sizex, sizey, sizez
        { 12.0+bd, 66.0+bd, 5.1, 5.1, 11.7 }, // Power LED
        { 22.6+bd, 66.0+bd, 5.1, 5.1, 11.7 }, // Standby LED
        { 12.9+bd, 3.9+bd, 13.9, 8.25, 11.7 }, // Power Switch
        { 21.3+bd, 12.3+bd, 22.3, 52, 11.5 }, // ATX Connector
        //{ 32.5+bd, 32.7+bd, 15.2, 10.0, 15.5 }, // ATX Connector Clip (this causes the over all height to increase when we can just cut it out of the top)
        { 29.6+bd, 64.3+bd, 14.0, 3.3, 2.4 }, // Top Resistor
        { 29.6+bd, 9.0+bd, 14.0, 3.3, 2.4 }, // Bottom Resitor
    { (-3.0)+bd, 11.6+bd, 11.0, 53.1, 11.7 }, // Breakout Connector
    { (-3.0)+bd, 12.85+bd, 23.3, 50.6, 11.7 } // Breakout Connector Labels
    };

    // The board ports (connections).
    b.topPorts = {
        // ATX Connector ( +0.5 )
        {
            East, {
        // y, z
                { 11.8+bd, 12.0 },
                { 64.8+bd, 12.0 },
                { 64.8+bd, -3.0 },
                { 11.8+bd, -3.0 }
            },
            0.5,
            2.5
        },
        // ATX Clip Top ( +0.5 )
        {
            Flat, {
        // x, y
                { 49.6+bd, 32.2+bd }, // we add 6 so it will not have the bevel at the top corners
                { 31.8+bd, 32.2+bd },
                { 31.8+bd, 43.2+bd },
                { 49.6+bd, 43.2+bd }  // we add 6 so it will not have the bevel at the top corners
            },
            0.5,
            2.5
        },
        // Breakout Connector ( +0.5 )
        {
            West, {
        // y, z
                { 12.6+bd, -3.0 },
                { 12.6+bd, 12.0 },
                { 63.7+bd, 12.0 },
                { 63.7+bd, -3.0 }
            },
            1.0,
            2.5
        },
    // Breakout Connector Top ( +0.5 )
        {
            Flat, {
        // x, y
                { -3.0, 12.6+bd },
                { 7.3+bd, 12.6+bd },
                { 7.3+bd, 63.7+bd },
                { -3.0, 63.7+bd }
            },
            1.0,
            2.5
        },
    // Breakout Connector Top 2 ( +0.5 )
        {
            Flat, {
        // x, y
                { -3.0, 12.85+(1.0)+bd },
                { 19.3+bd, 12.86+(1.0)+bd },
                { 19.3+bd, 63.45-(1.0)+bd },
                { -3.0, 63.45-(1.0)+bd }
            },
            1.0,
            2.5
        },
    // Power LED
    {
        Flat, {
            // x, y
            { 14.55+bd, 68.55+bd }
        },
        2.5,
        2.5
    },
    // Standby LED
    {
        Flat, {
            // x, y
            { 25.15+bd, 68.55+bd }
        },
        2.5,
        2.5
    },
    // Power Switch
    {
        Flat, {
            // x, y
            { 19.85+bd, 8.025+bd }
        },
        3.15,
        10
    }
    };

    b.topWallSupports = {
        {South, 20.3+bd, 1.0, 76.65 + (bd*2)},
    {West, 10.1+bd, 5.7, 20.4+bd},
    {West, (65.9-5.7)+bd, 5.7, 20.4+bd}
    };

    return b;
}


#endif // BBATXRABOARD_H
