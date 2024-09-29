#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include "SquareMatrixF.h"
#include "Image.h"
#include "ColorScheme.h"

using namespace std;

SquareMatrixF::SquareMatrixF(int size) {
    _size = size;
    matrixBuffer = (double*) malloc(sizeof(double) * _size * _size);
}

SquareMatrixF::SquareMatrixF(SquareMatrixF& source) {
    _size = source._size;
    matrixBuffer = (double*) malloc(sizeof(double) * _size * _size);
    memcpy(matrixBuffer, source.matrixBuffer, sizeof(double) * _size * _size);
}

void SquareMatrixF::setElement(int x, int y, double value) {
    matrixBuffer[x + y * _size] = value;
}

double SquareMatrixF::getElement(int x, int y) {
    return matrixBuffer[x + y * _size];
}

int SquareMatrixF::getSize() {
    return _size;
}

void SquareMatrixF::order(int* order) {
    SquareMatrixF copy(*this);
    
    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            setElement(x, y, copy.getElement(order[x],order[y]));
        }
    }
}

Image SquareMatrixF::toImage(double maxDist, int offset, bool flipVertical, bool flipHorizontal) {
    return toImage(maxDist, ColorScheme::greyscale(), offset, flipVertical, flipHorizontal);
}

Image SquareMatrixF::toImage(double maxDist, ColorScheme* colorScheme, int offset, bool flipVertical, bool flipHorizontal) {
    Image image(_size, _size, 3);
    offset = (offset % _size + _size) % _size;

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            unsigned char color[3];
            
            colorScheme->getColor(color, getElement(i,j), 0, maxDist);
            
            int imageI = (i + offset) % _size;
            int imageJ = (j + offset) % _size;
            imageI = flipVertical ? _size - imageI - 1 : imageI;
            imageJ = flipHorizontal ? _size - imageJ - 1 : imageJ;

            image.setPixel(imageI, imageJ, color);
        }
    }
    return image;
}

void SquareMatrixF::toTspFullMatrix(char* fileNameWithPath, char* tspName, char* comment) {
    string nameString = "NAME: ";
    string typeString = "TYPE: TSP";
    string commentString = "COMMENT: ";
    string dimensionString = "DIMENSION: ";
    string edgeWeightTypeString = "EDGE_WEIGHT_TYPE: EXPLICIT";
    string edgeWeightFormatString = "EDGE_WEIGHT_FORMAT: FULL_MATRIX";
    string dataStartString = "EDGE_WEIGHT_SECTION: ";

    ofstream output(fileNameWithPath);

    output << nameString << tspName << endl;
    output << typeString << endl;
    output << commentString << comment << endl;
    output << dimensionString << _size << endl;
    output << edgeWeightTypeString << endl;
    output << edgeWeightFormatString << endl;
    output << dataStartString << endl;

    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            output << getElement(j,i) << " ";
        }
        output << "" << endl;
    }
    output.close();
}

SquareMatrixF::~SquareMatrixF() {
    free(matrixBuffer);
}

void SquareMatrixF::flipVertical(){
    SquareMatrixF copy(*this);
    
    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            int imageX = _size - x - 1;
            setElement(imageX, y, copy.getElement(x,y));
        }
    }
}

void SquareMatrixF::flipHorizontal(){
    SquareMatrixF copy(*this);
    
    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            int imageY = _size - y - 1;
            setElement(x, imageY, copy.getElement(x,y));
        }
    }
}

void SquareMatrixF::offset(int offset){
    SquareMatrixF copy(*this);
    offset = (offset % _size + _size) % _size;
    
    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            int imageX = (x + offset) % _size;
            int imageY = (y + offset) % _size;
            setElement(imageX, imageY, copy.getElement(x,y));
        }
    }
}
