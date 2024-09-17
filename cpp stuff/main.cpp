#include <iostream>
#include <stdio.h>
#include <string>
#include "BMPGenerator.h"

using namespace std;

//#define MATRIX_SIZE 252
const int MATRIX_SIZE = 252;

static char* tspFile = (char*)"..\\test graphs\\concordeTest.tsp";
static char* tourFile = (char*)"..\\test graphs\\tour.cyc";

void prettyRainbow();
static void matrixImageBuilder(float (&matrix)[MATRIX_SIZE][MATRIX_SIZE], float maxDist, int offset = 0, bool flip = true);
string fileToString(char* fileNameWithPath);
void tspReader(string tspString, float* xs, float* ys, int length);
void tourReader(string tourString, int* order, int length);
void matrixPipeline(char* tspFile, char* tourFile);


int main ()
{
    prettyRainbow();
    matrixPipeline(tspFile, tourFile);
}

void matrixPipeline(char* tspFile, char* tourFile) {
    //read distances
    string resultString = fileToString(tspFile);
    float xs[MATRIX_SIZE];
    float ys[MATRIX_SIZE];
    tspReader(resultString, xs, ys, MATRIX_SIZE);
    
    //build distance matrix
    float distanceMatrix[MATRIX_SIZE][MATRIX_SIZE];
    float maxDist = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = i; j < MATRIX_SIZE; j++)
        {
            float dx = xs[i] - xs[j];
            float dy = ys[i] - ys[j];
            float dist = sqrt(dx*dx + dy*dy);

            distanceMatrix[i][j] = dist;
            distanceMatrix[j][i] = dist;

            if (dist > maxDist) maxDist = dist;
        }
    }

    //read the tour order    
    resultString = fileToString(tourFile);
    int order[MATRIX_SIZE];
    tourReader(resultString, order, MATRIX_SIZE);

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

void tourReader(string tourString, int* order, int length) {
    string match = "1 ";
    string delimiter = "\n";

    for (int i = 0; i < length; i++)
    {
        string line = tourString.substr(0, tourString.find(delimiter));
        tourString.erase(0, tourString.find(delimiter) + delimiter.length());
    
        int city = stoi(line);
            
        order[i] = city;
    }
}

void tspReader(string tspString, float* xs, float* ys, int length) {
    string match = "1 ";
    string delimiter = "\n";
    
    string line;
    int comp;

    //get rid of headers
    do {
        line = tspString.substr(0, tspString.find(delimiter));
        tspString.erase(0, tspString.find(delimiter) + delimiter.length());
        comp = match.compare(line.substr(0,2));
    } while (comp != 0);

    string splitter = " ";

    //first line we already got from previos section
    int index = stoi(line.substr(0, line.find(splitter)));
    line.erase(0, line.find(splitter) + splitter.length());
    float x = stof(line.substr(0, line.find(splitter)));
    line.erase(0, line.find(splitter) + splitter.length());
    float y = stof(line);

    xs[index - 1] = x;
    ys[index - 1] = y;

    //get all remaining lines
    while (tspString.length() > 0)
    {
        line = tspString.substr(0, tspString.find(delimiter));
        tspString.erase(0, tspString.find(delimiter) + delimiter.length());
    
        index = stoi(line.substr(0, line.find(splitter)));
        line.erase(0, line.find(splitter) + splitter.length());
        x = stof(line.substr(0, line.find(splitter)));
        line.erase(0, line.find(splitter) + splitter.length());
        y = stof(line);
            
        xs[index - 1] = x;
        ys[index - 1] = y;
    }
}

string fileToString(char* fileNameWithPath) {
    FILE* pFile;
    long lSize;
    char* buffer;
    size_t result;

    pFile = fopen ( fileNameWithPath, "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer, 1, lSize, pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    string resultString(buffer, 0, result);

    fclose (pFile);
    free (buffer);

    return resultString;
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