#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iomanip>
#include "BMPGenerator.h"
#include "GraphReader.h"
#include "Image.h"
#include "SquareMatrixF.h"
#include "ColorScheme.h"
#include "Tests.h"

void runAllTests() {
    cout << "Tests started!" << endl;
    matrixPipelineTest("..\\testFiles\\testInputs\\test graphs\\concordeTest.tsp", "..\\testFiles\\testInputs\\test graphs\\tour.cyc");
    readWriteTspMatrixTest();
    prettyRainbowTest();
    adjMatrixToImageTest("..\\testFiles\\testInputs\\_layer_0.in", 1305);
    colorSchemeMatrixTest();
    moransITest();
    cout << "All tests completed!" << endl;
}

void readWriteTspMatrixTest() {
    cout << "ReadWriteTest started! ";
    SquareMatrixF mat = GraphReader::readTsp_Explicit_FullMatrix("..\\testFiles\\testInputs\\tspTest.tsp");
    mat.toImage(100, ColorScheme::spectral()).printImageAsBMP("..\\testFiles\\testOutputs\\tspTestMatrix.bmp");
    mat.toTspFullMatrix("..\\testFiles\\testOutputs\\writeTest.tsp");
    cout << "ReadWriteTest finished!" << endl;
}

void matrixPipelineTest(char* tspFile, char* tourFile) {
    cout << "MatrixPiplineTest started! ";
    //read distances
    vector<vector<double>> coords = GraphReader::readTsp_EUC_2D(tspFile);
    
    int matrixSize = coords.size();

    //build distance matrix
    SquareMatrixF distanceMatrix(matrixSize);
    double maxDist = 0;
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = i; j < matrixSize; j++)
        {
            double dx = coords[i][0] - coords[j][0];
            double dy = coords[i][1] - coords[j][1];
            double dist = sqrt(dx*dx + dy*dy);

            distanceMatrix.setElement(i,j, dist);
            distanceMatrix.setElement(j,i, dist);

            if (dist > maxDist) maxDist = dist;
        }
    }

    //read the tour order    
    vector<int> order = GraphReader::readTour(tourFile);

    //order the matrix
    distanceMatrix.order(order.data());

    cout << distanceMatrix.moransI() << endl;

    //create the image
    char* imageFileName = (char*) "..\\testFiles\\testOutputs\\matrix.bmp";
    char* spectralImageFileName = (char*) "..\\testFiles\\testOutputs\\spectralMatrix.bmp";
    distanceMatrix.toImage(maxDist, 4).printImageAsBMP(imageFileName);
    distanceMatrix.toImage(maxDist, ColorScheme::spectral(), 4).printImageAsBMP(spectralImageFileName);
    cout << "MatrixPiplineTest finished!" << endl;
}

void adjMatrixToImageTest(char* adjMatrixFile, int matrixSize) {
    cout << "adjMatrixToImageTest started! ";
    FILE* file = fopen(adjMatrixFile, "r");

    //build distance matrix
    SquareMatrixF distanceMatrix(matrixSize);
    double maxDist = 0;
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            double dist;
            fscanf(file, "%lf", &dist);

            distanceMatrix.setElement(i,j, dist);
            distanceMatrix.setElement(j,i, dist);

            if (dist > maxDist) maxDist = dist;
        }
    }

    //create the image
    char* imageFileName = (char*) "..\\testFiles\\testOutputs\\adjMatrix.bmp";
    char* spectralImageFileName = (char*) "..\\testFiles\\testOutputs\\spectralAdjMatrix.bmp";
    distanceMatrix.toImage(maxDist, 4).printImageAsBMP(imageFileName);
    distanceMatrix.toImage(maxDist, ColorScheme::spectral(), 4).printImageAsBMP(spectralImageFileName);
    cout << "adjMatrixToImageTest finished!" << endl;
}

void prettyRainbowTest() {
    cout << "PrettyRainbowTest sterted! ";
    const int BYTES_PER_PIXEL = 3; /// red, green, & blue
    const int WIDTH = 600;
    const int HEIGHT = 450;

    int height = HEIGHT;
    int width = WIDTH;
    Image image(WIDTH, HEIGHT, BYTES_PER_PIXEL);
    char* imageFileName = (char*) "..\\testFiles\\testOutputs\\prettyRainbow.bmp";

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            image.setPixelChannel(i, j, 2, (unsigned char) ( i * 255 / height )); ///red
            image.setPixelChannel(i, j, 1, (unsigned char) ( j * 255 / width )); ///green
            image.setPixelChannel(i, j, 0, (unsigned char) ( (i+j) * 255 / (height+width) )); ///blue
        }
    }

    image.printImageAsBMP(imageFileName);
    cout << "PrettyRainbowTest finished!" << endl;
}

void colorSchemeMatrixTest(){
    cout << "ColorSchemeTest started! ";
    SquareMatrixF colorMatrix(1000);

    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            colorMatrix.setElement(i, j, i - 150);
        }
    }
    colorMatrix.toImage(1000-300, ColorScheme::spectral()).printImageAsBMP("..\\testFiles\\testOutputs\\spectralGradient.bmp");
    cout << "ColorSchemeTest finished!" << endl;
}

void moransITest() {
    cout << "MoransITest started!" << endl;
    // 0 1 0 0 1 1
    // 1 0 1 0 1 0
    // 0 1 0 1 0 1
    // 0 0 1 0 1 0
    // 1 1 0 1 0 1
    // 1 0 1 0 1 0

    SquareMatrixF* mat = new SquareMatrixF(6);
    mat->setElement(0, 0, 0);
    mat->setElement(0, 1, 1);
    mat->setElement(0, 2, 0);
    mat->setElement(0, 3, 0);
    mat->setElement(0, 4, 1);
    mat->setElement(0, 5, 1);
    mat->setElement(1, 0, 1);
    mat->setElement(1, 1, 0);
    mat->setElement(1, 2, 1);
    mat->setElement(1, 3, 0);
    mat->setElement(1, 4, 1);
    mat->setElement(1, 5, 0);
    mat->setElement(2, 0, 0);
    mat->setElement(2, 1, 1);
    mat->setElement(2, 2, 0);
    mat->setElement(2, 3, 1);
    mat->setElement(2, 4, 0);
    mat->setElement(2, 5, 1);
    mat->setElement(3, 0, 0);
    mat->setElement(3, 1, 0);
    mat->setElement(3, 2, 1);
    mat->setElement(3, 3, 0);
    mat->setElement(3, 4, 1);
    mat->setElement(3, 5, 0);
    mat->setElement(4, 0, 1);
    mat->setElement(4, 1, 1);
    mat->setElement(4, 2, 0);
    mat->setElement(4, 3, 1);
    mat->setElement(4, 4, 0);
    mat->setElement(4, 5, 1);
    mat->setElement(5, 0, 1);
    mat->setElement(5, 1, 0);
    mat->setElement(5, 2, 1);
    mat->setElement(5, 3, 0);
    mat->setElement(5, 4, 1);
    mat->setElement(5, 5, 0);

    cout << "Expected Morans I: -0.73 ";
    cout << "Computed Morans I: " << fixed << setprecision(2) << mat->moransI() << endl;
    
    cout << "MoransITest finished!" << endl;
}