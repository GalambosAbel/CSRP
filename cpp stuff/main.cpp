#include <iostream>
#include <stdio.h>
#include <string>
#include "BMPGenerator.h"
#include "GraphReader.h"
#include "Image.h"
#include "SquareMatrixF.h"
#include "ColorScheme.h"
#include "Tests.h"

using namespace std;

void tspToBmp(char* tspFileName, char* tourFileName, char* imgFileName, double maxDist = 60000) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix(tspFileName);
    vector<int> order = GraphReader::readNeosTour(tourFileName);
    distanceMatrix.order(order.data());
    
    //puts the dummy on the edge
    int offset = 0;
    for (int i = 0; i < order.size(); i++)
    {
        if (order[i] == order.size() - 1) {
            // cout << i;
            offset = order.size() - i;
        }
    }
    
    distanceMatrix.toImage(maxDist, ColorScheme::spectral(), offset, false, true).printImageAsBMP(imgFileName);
}

double inToTsp(char* inFileName, char* tspFileName, int matrixSize) {
    FILE* file = fopen(inFileName, "r");

    //build distance matrix
    SquareMatrixF distanceMatrix(matrixSize + 1);
    double maxDist = 0;
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            double dist;
            fscanf(file, "%lf", &dist);
            
            dist *= 100000; //neos has rounding issues maybe?

            distanceMatrix.setElement(i,j, dist);
            distanceMatrix.setElement(j,i, dist);

            if (dist > maxDist) maxDist = dist;
        }
        //add dummy
        distanceMatrix.setElement(matrixSize, i, 0);
        distanceMatrix.setElement(i, matrixSize, 0);
    }
    distanceMatrix.toTspFullMatrix(tspFileName);
    return maxDist;
} 


int main (int argc, char* argv[])
{
    if (argc == 1) {
        //Sinec test file paths are hardcoded, this needs to be invoked from the correct folder (same as source code)
        runAllTests();
        return 0;
    }
    else if (argc >= 5 && (stricmp(argv[1], "inToTsp") == 0 || stricmp(argv[1], "itt") == 0)) {
        int matrixSize = stoi(argv[4]);
        int maxDist = inToTsp(argv[2], argv[3], matrixSize);
        if(argc == 6) {
            if(stricmp(argv[5], "print") == 0) {
                cout << maxDist;
            }
            if(stricmp(argv[5], "return") == 0) {
                return maxDist;
            }
        }
        return 0;
    }
    else if (argc >= 5 && (stricmp(argv[1], "tspToImg") == 0 || stricmp(argv[1], "tti") == 0)) {
        if(argc == 6) {
            int maxDist = stoi(argv[5]);
            tspToBmp(argv[2], argv[3], argv[4], maxDist);
        }
        else {
            tspToBmp(argv[2], argv[3], argv[4]);
        }
        return 0;
    }
    return -1;
}


