#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cstring>
#include "SquareMatrix.h"
#include <cfloat>
#include "Image.h"
#include "ColorScheme.h"
#include "NEOSJob.h"

using namespace std;

SquareMatrix::SquareMatrix(int size)
{
    _size = size;
    maxValue = DBL_MIN;
    minValue = DBL_MAX;
    wordMeanings = (int *)malloc(sizeof(int) * _size);
    partOfSpeech = (int *)malloc(sizeof(int) * _size);
    matrixBuffer = (double *)malloc(sizeof(double) * _size * _size);
}

SquareMatrix::SquareMatrix(SquareMatrix &source)
{
    _size = source._size;
    maxValue = source.maxValue;
    wordMeanings = (int *)malloc(sizeof(int) * _size);
    partOfSpeech = (int *)malloc(sizeof(int) * _size);
    matrixBuffer = (double *)malloc(sizeof(double) * _size * _size);
    memcpy(wordMeanings, source.wordMeanings, sizeof(int) * _size);
    memcpy(partOfSpeech, source.partOfSpeech, sizeof(int) * _size);
    memcpy(matrixBuffer, source.matrixBuffer, sizeof(double) * _size * _size);
}

void SquareMatrix::setElement(int x, int y, double value)
{
    if (value > maxValue)
        setMaxValue(value);
    if (value < minValue)
        setMinValue(value);
    matrixBuffer[x + y * _size] = value;
}

void SquareMatrix::order(vector<int> order)
{
    SquareMatrix copy(*this);

    // order meanings
    for (int i = 0; i < _size; i++) {
        setMeaning(i, copy.getMeaning(order[i]));
    }

    // order parts of speech
    for (int i = 0; i < _size; i++) {
        setPartOfSpeech(i, copy.getPartOfSpeech(order[i]));
    }

    // order the distance matrix
    for (int x = 0; x < _size; x++) {
        for (int y = 0; y < _size; y++) {
            setElement(x, y, copy.getElement(order[x], order[y]));
        }
    }
}

Image SquareMatrix::toImage(double maxDist, int offset, bool flipVertical, bool flipHorizontal)
{
    return toImage(maxDist, ColorScheme::greyscale(), offset, flipVertical, flipHorizontal);
}

Image SquareMatrix::toImage(double maxDist, ColorScheme *colorScheme, int offset, bool flipVertical, bool flipHorizontal)
{
    Image image(_size, _size, 3);
    offset = (offset % _size + _size) % _size;

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            unsigned char color[3];

            colorScheme->getColor(color, getElement(i, j), 0, maxDist);

            int imageI = (i + offset) % _size;
            int imageJ = (j + offset) % _size;
            imageI = flipVertical ? _size - imageI - 1 : imageI;
            imageJ = flipHorizontal ? _size - imageJ - 1 : imageJ;

            image.setPixel(imageI, imageJ, color);
        }
    }

    return image;
}

Image SquareMatrix::toDetailedImage(double maxDist, ColorScheme *colorScheme, int offset, bool flipVertical, bool flipHorizontal)
{
    // allocate (legend + matrix)^2 for the image
    int legendSize = 100;
    Image image(_size + legendSize, _size + legendSize, 3);

    for (int i = 0; i < image.getHeight(); i++)
    {
        for (int j = 0; j < image.getWidth(); j++)
        {
            // default pixels are white
            unsigned char color[3] = {255, 255, 255};

            int matrixI = (i + offset - legendSize) % _size;
            int matrixJ = (j + offset - legendSize) % _size;

            matrixI = flipVertical ? _size - matrixI - 1 : matrixI;
            matrixJ = flipHorizontal ? _size - matrixJ - 1 : matrixJ;

            if (i >= legendSize && j < legendSize - 10) // Left bars
            {
                if (j < legendSize / 2 - 10) // left meanings color bar
                    ColorScheme::meaningLegend(getMeaning(matrixI), color);
                else if (j >= legendSize / 2) // left parts of speech color bar
                    ColorScheme::speechLegend(getPartOfSpeech(matrixI), color);
            }
            else if (i < legendSize - 10 && j >= legendSize) // Top bars
            {
                if (i < legendSize / 2 - 10) // top meanings color bar
                    ColorScheme::meaningLegend(getMeaning(matrixJ), color);
                else if (i >= legendSize / 2) // top parts of speech color bar
                    ColorScheme::speechLegend(getPartOfSpeech(matrixJ), color);
            }
            else if (i >= legendSize && j >= legendSize)
                colorScheme->getColor(color, getElement(matrixI, matrixJ), 0, maxDist);

            image.setPixel(_size + legendSize - 1 - i, j, color);
        }
    }

    return image;
}

string SquareMatrix::toTspFullMatrix(string tspName, string comment)
{
    string output = "";

    output += "NAME: " + tspName + "\n";
    output += "TYPE: TSP \n";
    output += "COMMENT: " + comment + "\n";
    output += "DIMENSION: " + to_string(_size) + "\n";
    output += "EDGE_WEIGHT_TYPE: EXPLICIT \n";
    output += "EDGE_WEIGHT_FORMAT: FULL_MATRIX \n";
    output += "EDGE_WEIGHT_SECTION: \n";

    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            output += to_string((int)getElement(i,j)) + " ";
        }
        output += "\n";
    }

    return output;
}

SquareMatrix SquareMatrix::toNeosInput()
{
    SquareMatrix newDistanceMatrix(getSize() + 1);
    for (int i = 0; i < getSize(); i++)
    {
        for (int j = 0; j < getSize(); j++)
        {
            newDistanceMatrix.setElement(i + 1, j + 1, 10000 * getElement(i, j));
        }

        newDistanceMatrix.setElement(i, 0, 0);
        newDistanceMatrix.setElement(0, i, 0);

        newDistanceMatrix.setMeaning(i+1, getMeaning(i));
        newDistanceMatrix.setPartOfSpeech(i+1, getPartOfSpeech(i));
    }
    newDistanceMatrix.setMeaning(0, 0);
    newDistanceMatrix.setPartOfSpeech(0, 0);

    return newDistanceMatrix;
}

void SquareMatrix::toInFullMatrix(char *fileNameWithPath)
{
    std::ofstream output(fileNameWithPath);
    output << _size << std::endl;

    // Write word meanings
    for (int i = 0; i < _size; i++)
    {
        output << getMeaning(i) << " ";
    }
    output << "" << std::endl;

    // Write part of speech
    for (int i = 0; i < _size; i++)
    {
        output << getPartOfSpeech(i) << " ";
    }
    output << "" << std::endl;

    // Write distances
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            output << getElement(j, i) << " ";
        }
        output << "" << std::endl;
    }
    output.close();
}

SquareMatrix::~SquareMatrix()
{
    free(wordMeanings);
    free(partOfSpeech);
    free(matrixBuffer);
}

void SquareMatrix::flipVertical()
{
    SquareMatrix copy(*this);

    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            int imageX = _size - x - 1;
            setElement(imageX, y, copy.getElement(x, y));
        }
    }
}

void SquareMatrix::flipHorizontal()
{
    SquareMatrix copy(*this);

    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            int imageY = _size - y - 1;
            setElement(x, imageY, copy.getElement(x, y));
        }
    }
}

void SquareMatrix::offset(int offset)
{
    SquareMatrix copy(*this);
    offset = (offset % _size + _size) % _size;

    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            int imageX = (x + offset) % _size;
            int imageY = (y + offset) % _size;
            setElement(imageX, imageY, copy.getElement(x, y));
        }
    }
}

void SquareMatrix::swap(int a, int b)
{
    int i = std::min(a, b);
    int j = std::max(a, b);
    int d1[] = {0, 1, 0, -1};
    int d2[] = {1, 0, -1, 0};

    double moransI_modifier = _morans_variance * 4 * _size * (_size - 1);

    for (int x = 0; x < _size; x++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (x + d1[k] < 0 || x + d1[k] >= _size)
                continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI -= (getElement(x, i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI -= 2 * (getElement(x, i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI -= (getElement(x, j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI -= 2 * (getElement(x, j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }

    for (int x = 0; x < _size; x++)
    {
        double temp = getElement(x, i);
        setElement(x, i, getElement(x, j));
        setElement(x, j, temp);
    }

    for (int x = 0; x < _size; x++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (x + d1[k] < 0 || x + d1[k] >= _size)
                continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI += (getElement(x, i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI += 2 * (getElement(x, i) - _morans_mean) * (getElement(x + d1[k], i + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI += (getElement(x, j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI += 2 * (getElement(x, j) - _morans_mean) * (getElement(x + d1[k], j + d2[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }

    for (int y = 0; y < _size; y++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (y + d1[k] < 0 || y + d1[k] >= _size)
                continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI -= (getElement(i, y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI -= 2 * (getElement(i, y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI -= (getElement(j, y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI -= 2 * (getElement(j, y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }

    for (int y = 0; y < _size; y++)
    {
        double temp = getElement(i, y);
        setElement(i, y, getElement(j, y));
        setElement(j, y, temp);
    }

    for (int y = 0; y < _size; y++)
    {
        for (int k = 0; k < 4; k++)
        {
            if (y + d1[k] < 0 || y + d1[k] >= _size)
                continue;

            if (!(i + d2[k] < 0 || i + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI += (getElement(i, y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI += 2 * (getElement(i, y) - _morans_mean) * (getElement(i + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }

            if (!(j + d2[k] < 0 || j + d2[k] >= _size))
            {
                if (k % 2 == 1)
                {
                    _moransI += (getElement(j, y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
                else
                {
                    _moransI += 2 * (getElement(j, y) - _morans_mean) * (getElement(j + d2[k], y + d1[k]) - _morans_mean) / moransI_modifier;
                }
            }
        }
    }
}

SquareMatrix SquareMatrix::KevinsDistanceMatrix(){
    SquareMatrix newMatrix(_size);

    //first normalize
    double minimum = getMinValue();
    double maximum = getMaxValue();
    double range = maximum - minimum;
    //now normal_val = (val-min)/range
    for (int i = 0; i < _size; i++) 
    {
        for (int j = 0; j < _size; j++)
        {
            double sum = 0;
            for (int z = 0; z < _size; z++)
            {
                double x = (getElement(i, z) - minimum) / range;
                double y = (getElement(j, z) - minimum) / range;
                double a = 0.1; //should maybe be a parameter
                sum += (x + y)*a + (1-a)*(x-y)*(x-y);
            }
            
            newMatrix.setElement(i, j, sum/_size);
        }

        if (i % 100 == 1) {
            cout << "\rBuilding Kevins distance matrix: " << (int)(((double)i / _size) * 100) << "\% complete" << flush;
        }
    }
    cout << "\rBuilding Kevins distance matrix: " << 100 << "\% complete!" << endl;
    
    return newMatrix;
}

SquareMatrix SquareMatrix::moransIDistanceMatrix()
{
    moransI();
    SquareMatrix newMatrix(_size);

    double min = DBL_MAX; // since we know we have '0' elements already
    double max = DBL_MIN;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            double sum = 0;
            for (int z = 0; z < _size; z++)
            {
                sum += (getElement(i, z) - _morans_mean) * (getElement(j, z) - _morans_mean); // here a_{xy} = getElement(x,y) - _morans_mean
            }
            // sum /= (_morans_variance * 4 * _size * (_size-1)); //with this it would be the MI contribution, and so the best ordering of this, is also the best MI (more proof needed?)
            sum = -sum;
            newMatrix.setElement(i, j, sum); // at this point we have an ME as defined by J. K. Lenstra et al. in  https://pubsonline.informs.org/doi/pdf/10.1287/opre.22.2.413 and so we know that tsp works.

            if(sum < min) { min = sum; }
            if(sum > max) { max = sum; }
        }

        if (i % 100 == 1) {
            cout << "\rBuilding MoransI distance matrix: " << (int)(((double)i / _size) * 100) << "\% complete" << flush;
        }
    }
    cout << "\rBuilding MoransI distance matrix: " << 100 << "\% complete!" << endl;

    // normalize all the values, notice that we add/multiply all the values by the same constants, and therefore optimal tsp is not inhibited.
    //(note: multiplication is only done with positives)
    double range = max - min;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            double val = newMatrix.getElement(i, j);

            val -= min;
            val /= range; // is now in [0,1]

            newMatrix.setElement(i, j, val);
        }
    }

    return newMatrix;
}

double SquareMatrix::moransI()
{
    // calculate mean and variance
    double sum = 0;
    double sumSquared = 0;
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            sum += getElement(i, j);
            sumSquared += getElement(i, j) * getElement(i, j);
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
            for (int k = 0; k < 4; k++)
            {
                if (i + d1[k] < 0 || i + d1[k] >= _size)
                    continue;
                if (j + d2[k] < 0 || j + d2[k] >= _size)
                    continue;
                out += (getElement(i, j) - _morans_mean) * (getElement(i + d1[k], j + d2[k]) - _morans_mean);
            }
        }
    }

    // and normalizes the result
    out /= _morans_variance;
    out /= 4 * _size * (_size - 1); // 4 neighbors for each point and n(n-1) pairs of points since we double count

    _moransI = out;

    return out;
}

double SquareMatrix::entropyScore() {
    double entropy = 0;
    double seqLength = 1;

    for (int i = 1; i < getSize(); i++) {
        if (getMeaning(i - 1) == getMeaning(i)) seqLength++;
        else {
            double seqProbability = (double)seqLength / _size;
            entropy -= log(seqProbability) * seqProbability;
            seqLength = 1;
        }
    }
    double seqProbability = (double)seqLength / _size;
    entropy -= log(seqProbability) * seqProbability; //for the final segment

    return entropy;
}

double SquareMatrix::switchingScore() {
    double score = 0;

    for (int i = 1; i < getSize(); i++) {
        if (getMeaning(i - 1) != getMeaning(i)) score++;
    }

    return score;
}

double SquareMatrix::simAnnealingOrderMoransI(int iters, double startTemp)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> distrib(0, _size - 1);

    SquareMatrix *bestMatrix = new SquareMatrix(*this);
    double currentMoransI = moransI();
    std::cout << "Initial Moran's I: " << currentMoransI << std::endl;

    double bestMoransI = currentMoransI;

    double temp = startTemp;

    // calculate the end temperature based on an
    // arbitrary minimum Moran's I cost difference of 1e-6
    // and a final accepting probability of 1e-9
    double endTemp = -1e-6 / log(1e-9);

    // apply geometric law for cooling
    double c = pow(endTemp / temp, 1.0 / iters);
    for (int iter = 0; iter < iters; iter++)
    {
        // swap two random elements
        int i = distrib(g);
        int j = distrib(g);
        swap(i, j);

        // double newMoransI = updateMoransISwap(i, j);
        double deltaMoransI = _moransI - currentMoransI;

        // apply Metropolis rule for acceptance of new order
        if (deltaMoransI > 0 || exp(deltaMoransI / temp) > (double)distrib(g) / _size)
        {
            currentMoransI = _moransI;
            if (currentMoransI > bestMoransI)
            {
                bestMoransI = currentMoransI;
                delete bestMatrix;
                bestMatrix = new SquareMatrix(*this);
            }
        }
        else
        {
            swap(i, j);
        }

        temp *= c;

        // print progress
        if (iter % 100000 == 0)
        {
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

vector<int> SquareMatrix::getTSPOrder() {
    NEOSJob job = NEOSJob();
    job.setCategory("co");
    job.setSolver("concorde");
    job.setInputMethod("TSP");
    job.setEmail(NEOSJob::defaultEmail);
    job.setTsp(toNeosInput().toTspFullMatrix());
    job.setAlgType("con");
    job.setRDType("fixed");
    job.setPLType("no");

    return job.submit();
}

void SquareMatrix::orderTSPRaw() {
    // SquareMatrix distanceMatrix = *this;
    // vector<int> o = distanceMatrix.getTSPOrder();

    vector<int> extraOrder = getTSPOrder();
    vector<int> normalOrder = extraNodeOrderToNormal(extraOrder);
    order(normalOrder);
}

void SquareMatrix::orderTSPMoransI() {
    //only this line is different for different orderings
    SquareMatrix distanceMatrix = moransIDistanceMatrix();

    vector<int> extraOrder = distanceMatrix.getTSPOrder();
    vector<int> normalOrder = extraNodeOrderToNormal(extraOrder);
    order(normalOrder);
}

void SquareMatrix::orderTSPKevin() {
    //only this line is different for different orderings
    SquareMatrix distanceMatrix = KevinsDistanceMatrix();
    
    vector<int> extraOrder = distanceMatrix.getTSPOrder();
    vector<int> normalOrder = extraNodeOrderToNormal(extraOrder);
    order(normalOrder);
}

vector<int> SquareMatrix::extraNodeOrderToNormal(vector<int> extraOrder){
    vector<int> normalOrder;
    for (int i = 1; i < extraOrder.size(); i++) {
       normalOrder.push_back(extraOrder[i] - 1);
    }
    return normalOrder;
}