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

void tspToBmp(string fileNameNoExtension, double maxDist = 60000) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix((fileNameNoExtension + ".tsp").data());
    vector<int> order = GraphReader::readNeosTour((fileNameNoExtension + ".tour").data());
    distanceMatrix.order(order.data());
    
    //puts the dummy on the edge
    int offset = 0;
    for (int i = 0; i < order.size(); i++)
    {
        if (order[i] == order.size() - 1) {
            cout << i;
            offset = order.size() - i;
        }
    }
    
    distanceMatrix.toImage(maxDist, ColorScheme::spectral(), offset, false, true).printImageAsBMP((fileNameNoExtension + ".png").data());
}

double inToTsp(string fileNameNoExtension, int matrixSize) {
    FILE* file = fopen((fileNameNoExtension + ".in").data(), "r");

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
    distanceMatrix.toTspFullMatrix((fileNameNoExtension + ".tsp").data());
    return maxDist;
} 

int main ()
{
    // cout << inToTsp("..\\work_layer_0", 1305);
    tspToBmp("..\\work_layer_0", 120000); //maxDist is 117767

    // runAllTests();
}

