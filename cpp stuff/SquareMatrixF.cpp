#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include "SquareMatrixF.h"
#include "Image.h"
#include "ColorScheme.h"
#include <float.h>

SquareMatrixF::SquareMatrixF(int size) {
    _size = size;
    maxValue = 0;
    matrixBuffer = (double*) malloc(sizeof(double) * _size * _size);
}

SquareMatrixF::SquareMatrixF(SquareMatrixF& source) {
    _size = source._size;
    maxValue = source.maxValue;
    matrixBuffer = (double*) malloc(sizeof(double) * _size * _size);
    memcpy(matrixBuffer, source.matrixBuffer, sizeof(double) * _size * _size);
}

void SquareMatrixF::setElement(int x, int y, double value) {
    if (value > maxValue) setMaxValue(value);
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
    std::string nameString = "NAME: ";
    std::string typeString = "TYPE: TSP";
    std::string commentString = "COMMENT: ";
    std::string dimensionString = "DIMENSION: ";
    std::string edgeWeightTypeString = "EDGE_WEIGHT_TYPE: EXPLICIT";
    std::string edgeWeightFormatString = "EDGE_WEIGHT_FORMAT: FULL_MATRIX";
    std::string dataStartString = "EDGE_WEIGHT_SECTION: ";

    std::ofstream output(fileNameWithPath);

    output << nameString << tspName << std::endl;
    output << typeString << std::endl;
    output << commentString << comment << std::endl;
    output << dimensionString << _size << std::endl;
    output << edgeWeightTypeString << std::endl;
    output << edgeWeightFormatString << std::endl;
    output << dataStartString << std::endl;

    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            output << getElement(j,i) << " ";
        }
        output << "" << std::endl;
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

void SquareMatrixF::swap(int a, int b) {
    int i = std::min(a, b);
    int j = std::max(a, b);
    int d1[] = {0, 1, 0, -1};
    int d2[] = {1, 0, -1, 0};

    double moransI_modifier = _morans_variance * 4 * _size * (_size - 1);

    for (int x = 0; x < _size; x++)
    {
        for (int k = 0; k < 4; k++) {
            if (x + d1[k] < 0 || x + d1[k] >= _size) continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI -= (getElement(x,i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI -= 2 * (getElement(x,i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI -= (getElement(x,j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI -= 2 * (getElement(x,j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }

    for (int x = 0; x < _size; x++)
    {
        double temp = getElement(x,i);
        setElement(x,i, getElement(x,j));
        setElement(x,j, temp);
    }

    for (int x = 0; x < _size; x++)
    {
        for (int k = 0; k < 4; k++) {
            if (x + d1[k] < 0 || x + d1[k] >= _size) continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI += (getElement(x,i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI += 2 * (getElement(x,i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI += (getElement(x,j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI += 2 * (getElement(x,j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }

    for (int y = 0; y < _size; y++)
    {
        for (int k = 0; k < 4; k++) {
            if (y + d1[k] < 0 || y + d1[k] >= _size) continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI -= (getElement(i,y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI -= 2 * (getElement(i,y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI -= (getElement(j,y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI -= 2 * (getElement(j,y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }

    for (int y = 0; y < _size; y++)
    {
        double temp = getElement(i,y);
        setElement(i,y, getElement(j,y));
        setElement(j,y, temp);
    }

    for (int y = 0; y < _size; y++)
    {
        for (int k = 0; k < 4; k++) {
            if (y + d1[k] < 0 || y + d1[k] >= _size) continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI += (getElement(i,y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI += 2 * (getElement(i,y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size)) {
                if (k % 2 == 1) {
                    _moransI += (getElement(j,y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                } else {
                    _moransI += 2 * (getElement(j,y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }
}

SquareMatrixF SquareMatrixF::moransIDistanceMatrix(){
    moransI();
    SquareMatrixF newMatrix(_size + 1);

    //add extra node as node 0 so we can get an ME later.
    for (int i = 0; i < _size + 1; i++)
    {
        newMatrix.setElement(i, 0, 0);
        newMatrix.setElement(0, i, 0);
    }

    double min = 0; //since we know we have '0' elements already
    double max = 0;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            double sum = 0;
            for (int z = 0; z < _size; z++)
            {
                sum += (getElement(i,z) - _morans_mean) * (getElement(j,z) - _morans_mean); //here a_{xy} = getElement(x,y) - _morans_mean
            }
            //sum /= (_morans_variance * 4 * _size * (_size-1)); //with this it would be the MI contribution, and so the best ordering of this, is also the best MI (more proof needed?)
            sum = -sum;
            newMatrix.setElement(i+1,j+1, sum); //at this point we have an ME as defined by J. K. Lenstra et al. in  https://pubsonline.informs.org/doi/pdf/10.1287/opre.22.2.413 and so we know that tsp works.
            
            if(sum < min) {min = sum;}
            if(sum > max) {max = sum;}
        }
    }

    //normalize all the values, notice that we add/multiply all the values by the same constants, and therefore optimal tsp is not inhibited.
    //(note: multiplication is only done with positives)
    double range = max-min;
    for (int i = 0; i < _size + 1; i++)
    {
        for (int j = 0; j < _size + 1; j++)
        {
            double val = newMatrix.getElement(i,j);

            val -= min;
            val /= range; //is now in [0,1]
            val *= 10000; //so NEOS doesn't complain, could maybe be less/more precise, 4digits is arbitrary

            newMatrix.setElement(i,j, (int)val); 
        }
    }

    return newMatrix;
}

double SquareMatrixF::moransI() {
    // calculate mean and variance
    double sum = 0;
    double sumSquared = 0;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            sum += getElement(i,j);
            sumSquared += getElement(i,j) * getElement(i,j);
        }
    }
    _morans_mean = sum / (_size * _size);
    _morans_variance = sumSquared / (_size * _size) - _morans_mean * _morans_mean;
    
    // morans I compares the distance of each point to the mean with each of its neighbors' distances to the mean
    int d1[] = {0, 1, 0, -1};
    int d2[] = {1, 0, -1, 0};
    double out = 0;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            for (int k = 0; k < 4; k++) {
                if (i + d1[k] < 0 || i + d1[k] >= _size) continue;
                if (j + d2[k] < 0 || j + d2[k] >= _size) continue;
                out += (getElement(i,j) - _morans_mean) * (getElement(i + d1[k], j + d2[k]) - _morans_mean);
            }
        }
    }

    // and normalizes the result
    out /= _morans_variance;
    out /= 4 * _size * (_size - 1); // 4 neighbors for each point and n(n-1) pairs of points since we double count

    _moransI = out;

    return out;
}

double SquareMatrixF::simAnnealingOrderMoransI(int iters, double startTemp) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> distrib(0, _size - 1);

    SquareMatrixF* bestMatrix = new SquareMatrixF(*this);
    double currentMoransI = moransI();
    std:: cout << "Initial Moran's I: " << currentMoransI << std::endl;

    double bestMoransI = currentMoransI;

    double temp = startTemp;

    // calculate the end temperature based on an 
    // arbitrary minimum Moran's I cost difference of 1e-6
	// and a final accepting probability of 1e-9
    double endTemp = -1e-6 / log(1e-9);

    // apply geometric law for cooling
	double c = pow(endTemp / temp, 1.0 / iters);
    for (int iter = 0; iter < iters; iter++) {
        // swap two random elements
        int i = distrib(g);
        int j = distrib(g);
        swap(i, j);

        // double newMoransI = updateMoransISwap(i, j);
        double deltaMoransI = _moransI - currentMoransI;

        // apply Metropolis rule for acceptance of new order
        if (deltaMoransI > 0 || exp(deltaMoransI / temp) > (double) distrib(g) / _size) {
            currentMoransI = _moransI;
            if (currentMoransI > bestMoransI) {
                bestMoransI = currentMoransI;
                delete bestMatrix;
                bestMatrix = new SquareMatrixF(*this);
            }
        } else {
            swap(i, j);
        }

        temp *= c;

        // print progress
        if (iter % 100000 == 0) {
            std::cout << "Iter: " << iter << " Temp: " << temp << " Curr: " << _moransI << " Best: " << bestMoransI << std::endl;
        }
    }

    // copy the best order to the original matrix
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            setElement(i, j, bestMatrix->getElement(i, j));
        }
    }

    return bestMoransI;
}
