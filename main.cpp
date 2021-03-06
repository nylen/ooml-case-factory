#include <fstream>
#include <ooml/core/IndentWriter.h>

#include "casefactory.h"
#include "board.h"

// Small helper function which writes the model to a file in SCAD format.
void write(std::string fileName, const Component & model)
{
    std::cout << "Writing file " << fileName << " ... ";

    IndentWriter writer;
    writer << model;

    std::ofstream outFile;
    outFile.open(fileName);
    outFile << writer;

    std::cout << "done" << std::endl;
}


int main()
{
    BoardDescription board = makeNamedBoard();

    // Create a factory to build a case for this board.
    CaseFactory factory(board);

    // Now we can fine-tune some dimension parameters for the case model. See the class CaseFactory for more options, such as wall thickness, screw hole radius etc.
    factory.smallerBottomHeight = .5; // We want the bottom part to be a bit less high (so the GPIO pin ends will be within the floor; this is just to demonstrate the power of the feature "forbidden areas")

    // You can set some print settings which will help the factory to optimize print results.
    factory.printLayerHeight = .2;
    factory.printSafeBridgeLayerCount = 3;


    // Generate the models
    Component bottom = factory.constructBottom();
    Component top = factory.constructTop();


    // Write these models to SCAD files. We generate 3 files.
    // 1) Only the bottom:
    write(board.name + "-case-bottom.scad", bottom);
    // 2) Only the top:
    write(board.name + "-case-top.scad", top);
    // 3) Both parts side by side:
    double distance = 5; // mm
    double offset = factory.outerDimensions().y + distance;
    write(board.name + "-case.scad", bottom + top.translatedCopy(0, offset, 0));

    return 0;
}


