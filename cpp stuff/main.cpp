#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cmath>
#include "BMPGenerator.h"
#include "GraphReader.h"
#include "Image.h"
#include "SquareMatrixF.h"
#include "ColorScheme.h"
#include "Tests.h"

using namespace std;

int inToTsp(char* inFileName, char* tspFileName, int matrixSize) {
    GraphReader::loadDistanceMatrix(inFileName, matrixSize).toTspFullMatrix(tspFileName);
    return 0;
}

int tspToImg(char* tspFileName, char* imgFileName, double maxDist) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(tspFileName);
    
    distanceMatrix.toImage(maxDist, ColorScheme::spectral(), 0, false, true).printImageAsBMP(imgFileName);

    return 0;
}

int orderTsp(char* sourceTspFileName, char* tourFileName, char* targetTspFileName) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(sourceTspFileName);
    vector<int> order = GraphReader::readNeosTour(tourFileName);
    if (order.size() < distanceMatrix.getSize()) {return -2;}
    if (order.size() > distanceMatrix.getSize() + 1) {return -3;}
    if (order.size() == distanceMatrix.getSize() + 1) {
        for (int i = 0; i < distanceMatrix.getSize(); i++) //an extra node was added
        {
            order[i] = order[i+1] - 1;
        }        
    }
    distanceMatrix.order(order.data());
    
    distanceMatrix.toTspFullMatrix(targetTspFileName);

    return 0;
}

//TODO there is waaaay too muh code duplication here! (only line 1 differs in sa and sat)
int simAnneal(char* inFileName, char* outPath, int matrixSize, int iterations, double startTemp) {
    SquareMatrixF distanceMatrix = GraphReader::loadDistanceMatrix(inFileName, matrixSize);
    double best = distanceMatrix.simAnnealingOrderMoransI(iterations, startTemp);

    string noExtensionName(outPath);
    noExtensionName += to_string(best);
    std::ofstream output(noExtensionName + ".out");

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            output << distanceMatrix.getElement(i, j) << " ";
        }
        output << std::endl;
    }
        
    distanceMatrix.toImage(0.6, ColorScheme::spectral(), 4).printImageAsBMP(const_cast<char*>((noExtensionName + ".bmp").c_str()));

    output.close();
    return 0;
}

int simAnnealTsp(char* tspFileName, char* outPath, int iterations, double startTemp) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(tspFileName);
    double best = distanceMatrix.simAnnealingOrderMoransI(iterations, startTemp);

    string noExtensionName(outPath);
    noExtensionName += to_string(best);
    std::ofstream output(noExtensionName + ".out");

    for (int i = 0; i < distanceMatrix.getSize(); i++)
    {
        for (int j = 0; j < distanceMatrix.getSize(); j++)
        {
            output << distanceMatrix.getElement(i, j) << " ";
        }
        output << std::endl;
    }

    distanceMatrix.toImage(0.6, ColorScheme::spectral(), 4).printImageAsBMP(const_cast<char*>((noExtensionName + ".bmp").c_str()));

    output.close();

    return 0;
}

int printMoransI(char* tspFileName) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(tspFileName);
    cout << "The Moran's I in this oredring is: " << distanceMatrix.moransI() << "." << endl;
    return 0;
}

int moransIDistances(char* sourceTspFileName, char* targetTspFileName) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(sourceTspFileName);

    distanceMatrix.moransIDistanceMatrix().toTspFullMatrix(targetTspFileName);
    
    return 0;
}

int discretizeIn(char* tspFileName, char* discreteTspFileName, int matrixSize, int numBuckets) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(tspFileName);

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            distanceMatrix.setElement(i, j, round((distanceMatrix.getElement(i, j)/distanceMatrix.getMaxValue()) * (numBuckets - 1)));
        }
    }

    distanceMatrix.toTspFullMatrix(discreteTspFileName);
    return 0;
}

int main (int argc, char* argv[])
{
    if (argc == 1) {
        //Since test file paths are hardcoded, this needs to be invoked from the correct folder (same as source code)
        runAllTests();
        return 0;
    } else if (argc == 5 && (stricmp(argv[1], "inToTsp") == 0 || stricmp(argv[1], "itt") == 0)) {
        return inToTsp(argv[2], argv[3], stoi(argv[4]));
    } else if (argc == 5 && (stricmp(argv[1], "tspToImg") == 0 || stricmp(argv[1], "tti") == 0)) {
        return tspToImg(argv[2], argv[3], stod(argv[4]));
    } else if (argc == 5 && (stricmp(argv[1], "orderTsp") == 0 || stricmp(argv[1], "ot") == 0)) {
        return orderTsp(argv[2], argv[3], argv[4]);
    }else if (argc == 7 && (stricmp(argv[1], "simAnneal") == 0 || stricmp(argv[1], "sa") == 0)) {
        return simAnneal(argv[2], argv[3], stoi(argv[4]), stoi(argv[5]), stod(argv[6]));
    } else if (argc == 6 && (stricmp(argv[1], "simAnnealTsp") == 0 || stricmp(argv[1], "sat") == 0)) {
        return simAnnealTsp(argv[2], argv[3], stoi(argv[4]), stod(argv[5]));
    } else if (argc == 3 && (stricmp(argv[1], "printMoransI") == 0 || stricmp(argv[1], "pmi") == 0)) {
        return printMoransI(argv[2]);
    } else if (argc == 4 && (stricmp(argv[1], "moransIDistances") == 0 || stricmp(argv[1], "mid") == 0)) {
        return moransIDistances(argv[2], argv[3]);
    } else if (argc == 6 && (stricmp(argv[1], "discretize") == 0 || stricmp(argv[1], "dis") == 0)) {
        return discretizeIn(argv[2], argv[3], stoi(argv[4]), stoi(argv[5]));
    }
    return -1;
}


