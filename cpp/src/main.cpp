#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include "SquareMatrixF.h"
#include "GraphReader.h"
#include "Tests.h"

#include <cxxopts.hpp>

using namespace std;

void order(string inputFile, string outputFile, function<void(SquareMatrixF&)> orderFunction);
void visualize(string inputFile, string outputFile);
void score(string inputFile);

// int discretizeIn(char* tspFileName, char* discreteTspFileName, int matrixSize, int numBuckets) {
//     SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(tspFileName);
//
//     for (int i = 0; i < matrixSize; i++)
//     {
//         for (int j = 0; j < matrixSize; j++)
//         {
//             distanceMatrix.setElement(i, j, round((distanceMatrix.getElement(i, j)/(distanceMatrix.getMaxValue() - distanceMatrix.getMinValue())) * (numBuckets - 1)));
//         }
//     }
//
//     distanceMatrix.toTspFullMatrix(discreteTspFileName);
//     return 0;
// }

int main (int argc, char* argv[])
{
    cxxopts::Options options("csrp", "Some tools for ordering distance matrices");

    options.add_options()
        ("help", "produce help message")
        ("test", "run all tests")
        ("r,orderRaw", "order a distance matrix based on distances in it")
        ("m,orderMoransI", "order a distance matrix based on a metric that optimizes Moran's I")
        ("i,input", "input file", cxxopts::value<string>())
        ("v,visualize", "visualize file", cxxopts::value<string>())
        ("o,output", "output file", cxxopts::value<string>())
        ("s,score", "get a score of the ordering of the matrix")
    ;

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::exception &x) {
        std::cerr << "csrp: " << x.what() << '\n';
        std::cerr << "usage: csrp [options] <input_file> -s <matrix_size> ...\n";
        return EXIT_FAILURE;
    }

    if (result.count("help")) {
        cout << options.help() << endl;
        return EXIT_SUCCESS;
    }

    if (result.count("test")) {
        runAllTests();
        return EXIT_SUCCESS;
    }

    function<void(SquareMatrixF&)> orderFunction;
    if (result.count("orderRaw")) {
        orderFunction = [](SquareMatrixF& matrix) {
            matrix.orderTSPRaw();
        };
    } else if (result.count("orderMoransI")) {
        orderFunction = [](SquareMatrixF& matrix) {
            matrix.orderTSPMoransI();
        };
    }

    if (result.count("input")) {
        string inputFile = result["input"].as<string>();

        if (result.count("score")) {
            cout << "Input matrix score" << endl;
            score(inputFile);
        }

        if (orderFunction && result.count("visualize")) {
            string outputInFile = result.count("output") ? result["output"].as<string>() : "";
            order(inputFile, outputInFile, orderFunction);

            string outputPNGFile = result.count("visualize") ? result["visualize"].as<string>() : "";
            visualize(outputInFile, outputPNGFile);
            if (result.count("score")){
                cout << "Ordered matrix score" << endl;
                score(outputInFile);
            }
        } else {
            if(orderFunction){
                string outputInFile = result.count("output") ? result["output"].as<string>() : "";
                order(inputFile, outputInFile, orderFunction);

                if (result.count("score")) {
                    score(outputInFile);
                }
            }

            if (result.count("visualize")) {
                string outputFile = result.count("visualize") ? result["visualize"].as<string>() : "";
                visualize(inputFile, outputFile);
            }
        }
    }

    return EXIT_SUCCESS;
}

void order(string inputFile, string outputFile, function<void(SquareMatrixF&)> orderFunction) {
    SquareMatrixF distanceMatrix = GraphReader::loadDistanceMatrix(inputFile);
    int matrixSize = distanceMatrix.getSize();

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            distanceMatrix.setElement(i, j, distanceMatrix.getElement(i, j) * 100000);
        }
    }

    SquareMatrixF newDistanceMatrix(matrixSize + 1);
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            newDistanceMatrix.setElement(i + 1, j + 1, distanceMatrix.getElement(i, j));
        }

        newDistanceMatrix.setElement(i, 0, 0);
        newDistanceMatrix.setElement(0, i, 0);

        newDistanceMatrix.setMeaning(i, distanceMatrix.getMeaning(i));
        newDistanceMatrix.setPartOfSpeech(i, distanceMatrix.getPartOfSpeech(i));
    }

    orderFunction(newDistanceMatrix);

    if (!outputFile.empty()) {
        newDistanceMatrix.toInFullMatrix(const_cast<char*>(outputFile.c_str()));
    }
}

void visualize(string inputFile, string outputFile) {
    SquareMatrixF distanceMatrix = GraphReader::loadDistanceMatrix(inputFile);

    if (!outputFile.empty()) {
        distanceMatrix.toDetailedImage(60000, ColorScheme::spectral(), 0, false, false).printImageAsBMP(const_cast<char*>(outputFile.c_str()));
    }
}

void score(string inputFile) {
    SquareMatrixF distanceMatrix = GraphReader::loadDistanceMatrix(inputFile);
    cout << "The score of matrix \"" << inputFile << "\" is: " << distanceMatrix.score() << endl;
    cout << "The Moran's I of the matrix is: " << distanceMatrix.moransI() << endl;
}
