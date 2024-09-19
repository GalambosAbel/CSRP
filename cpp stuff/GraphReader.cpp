#include <iostream>
#include <string>
#include "GraphReader.h"

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

void GraphReader::readTour(char* tourFileNameWithPath, int* order, int length) {
    string tourString = fileToString(tourFileNameWithPath);
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

void GraphReader::readTsp(char* tspFileNameWithPath, float* xs, float* ys, int length) {
    string tspString = fileToString(tspFileNameWithPath);
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
