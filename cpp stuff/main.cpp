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

void tspToBmp(double maxDist = 60000) {
    SquareMatrixF distanceMatrix = GraphReader::readTsp_Explicit_FullMatrix("..\\work_layer_0.tsp");
    vector<int> order = GraphReader::readNeosTour("..\\work_layer_0.tour");
    distanceMatrix.order(order.data());
    int offset = 0;
    for (int i = 0; i < order.size(); i++)
    {
        if (order[i] == order.size() - 1) {
            cout << i;
            offset = order.size() - i;
        }
    }
    
    distanceMatrix.toImage(maxDist, ColorScheme::spectral(), offset).printImageAsBMP("..\\work_layer_0.png");
}

double inToTsp() {
    int matrixSize = 1305;

    FILE* file = fopen("..\\testFiles\\testInputs\\_layer_0.in", "r");

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
    distanceMatrix.toTspFullMatrix("..\\work_layer_0.tsp");
    return maxDist;
} 

int main ()
{
    // cout << inToTsp();
    tspToBmp(); //maxDist is 43699
}

