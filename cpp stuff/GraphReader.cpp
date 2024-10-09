#include <iostream>
#include <fstream>
#include <string>
#include "GraphReader.h"
#include "SquareMatrixF.h"

using namespace std;

string GraphReader::fileToString(char* fileNameWithPath) {
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

vector<int> GraphReader::readTour(char* tourFileNameWithPath) {
    string tourString = fileToString(tourFileNameWithPath);
    string delimiter = "\n";

    vector<int> order = vector<int>();

    while (true)
    {
        //find end of the line
        int loc = tourString.find(delimiter);
        if(loc == string::npos) { //if not found, end of file, return
            return order;
        }

        //get and remove line
        string line = tourString.substr(0, loc);
        tourString.erase(0, tourString.find(delimiter) + delimiter.length());
    
        //get city, and store it
        int city = stoi(line);  
        order.push_back(city);
    }
    //should never happen
    return vector<int>();
}


vector<int> GraphReader::readNeosTour(char* tourFileNameWithPath) {
    string tourString = fileToString(tourFileNameWithPath);
    string lineEndDelimiter = "\n";
    string midLineDelimiter = " ";

    vector<int> order = vector<int>();

    //get length
    int loc = tourString.find(lineEndDelimiter);
    string line = tourString.substr(0, loc);
    tourString.erase(0, tourString.find(lineEndDelimiter) + lineEndDelimiter.length());
    int length = stoi(line.substr(0, line.find(midLineDelimiter)));

    for (int i = 0; i < length; i++)
    {
        //find end of the line
        loc = tourString.find(lineEndDelimiter);

        //get and remove line
        line = tourString.substr(0, loc);
        tourString.erase(0, tourString.find(lineEndDelimiter) + lineEndDelimiter.length());
    
        //get city, and store it
        int city = stoi(line.substr(0, line.find(midLineDelimiter)));  
        order.push_back(city);
    }

    return order;
}

vector<vector<double>> GraphReader::readTsp_EUC_2D(char* tspFileNameWithPath) {
    string tspString = fileToString(tspFileNameWithPath);
    string coordSectionStartString = "NODE_COORD_SECTION";
    string dimensionString = "DIMENSION: ";
    string endOfLine = "\n";
    string splitter = " ";
    
    vector<vector<double>> coords;
    
    //get dimension of graph
    int dimStart = tspString.find(dimensionString) + dimensionString.length();
    int dimEnd = tspString.find(endOfLine, dimStart);
    int length  = stoi(tspString.substr(dimStart, dimEnd));

    //get rid of headers
    tspString.erase(0, tspString.find(coordSectionStartString) + coordSectionStartString.length());
    tspString.erase(0, tspString.find(endOfLine) + endOfLine.length());

    //get all remaining lines
    for (int i = 0; i < length ; i++)
    {
        string line = tspString.substr(0, tspString.find(endOfLine));
        tspString.erase(0, tspString.find(endOfLine) + endOfLine.length());
    
        int index = stoi(line.substr(0, line.find(splitter))); //currently unused
        line.erase(0, line.find(splitter) + splitter.length());
        double x = stod(line.substr(0, line.find(splitter)));
        line.erase(0, line.find(splitter) + splitter.length());
        double y = stod(line);
    
        vector<double> coord;
        coord.push_back(x);
        coord.push_back(y);
        coords.push_back(coord);
    }

    return coords;
}

SquareMatrixF GraphReader::readTsp_Explicit_FullMatrix(char* tspFileNameWithPath) {
    string tspString = fileToString(tspFileNameWithPath);
    // string edgeWeightFormatString = "EDGE_WEIGHT_FORMAT: ";
    string edgeSectionStartString = "EDGE_WEIGHT_SECTION";
    string dimensionString = "DIMENSION: ";
    string endOfLine = "\n";
    string splitter = " ";
    
    
    //get dimension of graph
    int dimStart = tspString.find(dimensionString) + dimensionString.length();
    int dimEnd = tspString.find(endOfLine, dimStart);
    int length  = stoi(tspString.substr(dimStart, dimEnd));
    
    SquareMatrixF matrix = SquareMatrixF(length);

    //get rid of headers
    tspString.erase(0, tspString.find(edgeSectionStartString) + edgeSectionStartString.length());
    tspString.erase(0, tspString.find(endOfLine) + endOfLine.length());

    //get all remaining lines
    for (int i = 0; i < length ; i++)
    {
        string line = tspString.substr(0, tspString.find(endOfLine));
        tspString.erase(0, tspString.find(endOfLine) + endOfLine.length());
        for (int j = 0; j < length; j++)
        {
            double x = stod(line.substr(0, line.find(splitter)));
            line.erase(0, line.find(splitter) + splitter.length());

            matrix.setElement(j,i, x);
        }
    }

    return matrix;
}

SquareMatrixF GraphReader::load_distance_matrix(const string fileNameWithPath, int matrixSize) {
    std::ifstream file(fileNameWithPath);

    //build distance matrix
    SquareMatrixF distanceMatrix(matrixSize);
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            double dist;
            file >> dist;

            distanceMatrix.setElement(i,j, dist);
            distanceMatrix.setElement(j,i, dist);
        }
    }

    return distanceMatrix;
}