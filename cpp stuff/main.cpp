#include <iostream>
#include <stdio.h>
#include <string>
#include "BMPGenerator.h"
#include "GraphReader.h"

using namespace std;

//#define MATRIX_SIZE 252
const int MATRIX_SIZE = 252;

static char* tspFile = (char*)"..\\test graphs\\concordeTest.tsp";
static char* tourFile = (char*)"..\\test graphs\\tour.cyc";

void prettyRainbow();
static void matrixImageBuilder(float (&matrix)[MATRIX_SIZE][MATRIX_SIZE], float maxDist, int offset = 0, bool flip = true);
void matrixPipeline(char* tspFile, char* tourFile);


int main ()
{
    prettyRainbow();
    matrixPipeline(tspFile, tourFile);
}

void matrixPipeline(char* tspFile, char* tourFile) {
    //read distances
    vector<vector<float>> coords = GraphReader::readTsp_EUC_2D(tspFile);
    
    //build distance matrix
    float distanceMatrix[MATRIX_SIZE][MATRIX_SIZE];
    float maxDist = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = i; j < MATRIX_SIZE; j++)
        {
            float dx = coords[i][0] - coords[j][0];
            float dy = coords[i][1] - coords[j][1];
            float dist = sqrt(dx*dx + dy*dy);

            distanceMatrix[i][j] = dist;
            distanceMatrix[j][i] = dist;

            if (dist > maxDist) maxDist = dist;
        }
    }

    //read the tour order    
    vector<int> order = GraphReader::readTour(tourFile);

    //build the ordered matrix
    float orderedDistanceMatrix[MATRIX_SIZE][MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            orderedDistanceMatrix[i][j] = distanceMatrix[order[i]][order[j]];
        }
    }

    matrixImageBuilder(orderedDistanceMatrix, maxDist, 4);
    printf("Matrix image generated!");
}

void matrixImageBuilder(float (&matrix)[MATRIX_SIZE][MATRIX_SIZE], float maxDist, int offset, bool flip) {
    const int BYTES_PER_PIXEL = 3; /// red, green, & blue
    const int WIDTH = MATRIX_SIZE;
    const int HEIGHT = MATRIX_SIZE;
    
    int height = HEIGHT;
    int width = WIDTH;
    unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "matrix.bmp";

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            unsigned char color = ((int)(matrix[i][j] * 255 / maxDist));

            offset = (offset % MATRIX_SIZE + MATRIX_SIZE) % MATRIX_SIZE;
            int imageI = (i + offset) % MATRIX_SIZE;
            imageI = flip ? height - imageI - 1 : imageI;
            int imageJ = (j + offset) % MATRIX_SIZE;
            
            image[imageI][imageJ][2] = color; //r
            image[imageI][imageJ][1] = color; //g
            image[imageI][imageJ][0] = color; //b
        }
    }

    BMPGenerator::generateBitmapImage((unsigned char*) image, height, width, imageFileName);
}

void prettyRainbow() {
    const int BYTES_PER_PIXEL = 3; /// red, green, & blue
    const int WIDTH = 600;
    const int HEIGHT = 450;

    int height = HEIGHT;
    int width = WIDTH;
    unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "prettyRainbow.bmp";

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            image[i][j][2] = (unsigned char) ( i * 255 / height );             ///red
            image[i][j][1] = (unsigned char) ( j * 255 / width );              ///green
            image[i][j][0] = (unsigned char) ( (i+j) * 255 / (height+width) ); ///blue
        }
    }

    BMPGenerator::generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    printf("Image generated!!");
}