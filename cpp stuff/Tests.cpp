#include <iostream>
#include <stdio.h>
#include <string>
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
    vector<vector<float>> coords = GraphReader::readTsp_EUC_2D(tspFile);
    
    int matrixSize = coords.size();

    //build distance matrix
    SquareMatrixF distanceMatrix(matrixSize);
    float maxDist = 0;
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = i; j < matrixSize; j++)
        {
            float dx = coords[i][0] - coords[j][0];
            float dy = coords[i][1] - coords[j][1];
            float dist = sqrt(dx*dx + dy*dy);

            distanceMatrix.setElement(i,j, dist);
            distanceMatrix.setElement(j,i, dist);

            if (dist > maxDist) maxDist = dist;
        }
    }

    //read the tour order    
    vector<int> order = GraphReader::readTour(tourFile);

    //order the matrix
    distanceMatrix.order(order.data());

    //create the image
    char* imageFileName = (char*) "..\\testFiles\\testOutputs\\matrix.bmp";
    char* spectralImageFileName = (char*) "..\\testFiles\\testOutputs\\spectralMatrix.bmp";
    distanceMatrix.toImage(maxDist, 4).printImageAsBMP(imageFileName);
    distanceMatrix.toImage(maxDist, ColorScheme::spectral(), 4).printImageAsBMP(spectralImageFileName);
    cout << "MatrixPiplineTest finished!" << endl;
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