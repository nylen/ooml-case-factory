#include <fstream>
#include <ooml/core/IndentWriter.h>

#include "casefactory.h"
#include "cubieboard.h"



int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <out_file>" << std::endl;
        std::cerr << "   where <out_file> is the target filename of the OpenSCAD file." << std::endl;
        return 1;
    }


    // Here, you can change the board for which a case you want to construct.
    // Don't forget to change the #include statement above.
    BoardDescription board = cubieBoard();
    CaseFactory factory(board);

    // You can set some print settings which will help the factory to optimize print results.
    factory.printLevelHeight = .2;
    factory.printSafeBridgeLayerCount = 3;


    // Some different scene setups. Umcomment to enable.

    // Only the bottom
    /*
    Component scene = factory.constructBottom();
    */

    // Only the top
    /*
    Component scene = factory.constructTop();
    */

    // Both parts side by side
    double distance = 5; // mm
    Component scene = factory.constructBottom()
            + factory.constructTop()
            .translatedCopy(0, factory.outerDimensions().y + distance, 0);

    // Both parts as assembled
    /*
    double distance = 0.1; // mm
    Component scene = factory.constructBottom()
            + factory.constructTop()
            .rotatedCopy(180.0, 0, 0)
            .translatedCopy(0, board.size[1], factory.outerDimensions().z + distance);
    */



    // Write the results as an OpenSCAD file.
    IndentWriter writer;
    writer << scene;
    std::ofstream outFile;
    outFile.open(argv[1]);
    outFile << writer;

    return 0;
}


