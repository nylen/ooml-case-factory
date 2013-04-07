#include <fstream>
#include <ooml/core/IndentWriter.h>

#include "casefactory.h"
#include "cubieboard.h"


void write(std::string fileName, const Component & model)
{
    IndentWriter writer;
    writer << model;

    std::ofstream outFile;
    outFile.open(fileName);
    outFile << writer;
}


int main(int argc, char* argv[])
{
    // Here, you can change the board for which a case you want to construct.
    // Don't forget to change the #include statement above.
    BoardDescription board = cubieBoard();
    CaseFactory factory(board);
    std::string name = "cubieboard"; // File name prefix

    // You can set some print settings which will help the factory to optimize print results.
    factory.printLevelHeight = .2;
    factory.printSafeBridgeLayerCount = 3;


    // Generate the models
    Component bottom = factory.constructBottom();
    Component top = factory.constructTop();


    // Only the bottom
    write(name + "-case-bottom.scad", bottom);

    // Only the top
    write(name + "-case-top.scad", top);

    // Both parts side by side
    double distance = 5; // mm
    double offset = factory.outerDimensions().y + distance;
    write(name + "-case.scad", bottom + top.translatedCopy(0, offset, 0));

    return 0;
}


