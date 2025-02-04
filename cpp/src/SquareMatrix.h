#ifndef SQUARE_MATRIX
#define SQUARE_MATRIX

#include "Image.h"
#include "ColorScheme.h"
#include <vector>
#include <string>

using namespace std;

class SquareMatrix
{
public:
    SquareMatrix(int size);
    SquareMatrix(SquareMatrix &source);
    ~SquareMatrix();

    void setElement(int x, int y, double value);
    void setMaxValue(double value) { maxValue = value; };
    void setMinValue(double value) { minValue = value; };
    void setMeaning(int x, int value) { wordMeanings[x] = value; };
    void setPartOfSpeech(int x, int value) { partOfSpeech[x] = value; };

    double getElement(int x, int y) { return matrixBuffer[x + y * _size]; };
    double getMaxValue() { return maxValue; };
    double getMinValue() { return minValue; };
    int getSize() { return _size; };
    int getMeaning(int x) { return wordMeanings[x]; };
    int getPartOfSpeech(int x) { return partOfSpeech[x]; };

    void order(vector<int> order);

    Image toImage(double maxDist, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
    Image toImage(double maxDist, ColorScheme *colorScheme, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
    Image toDetailedImage(double maxDist, ColorScheme* colorScheme, int offset = 0, bool flipVertical = false, bool flipHorizontal = false);

    string toTspFullMatrix(string tspName = "tsplib from matrix", string comment = "");
    SquareMatrix toNeosInput();
    void toInFullMatrix(char *fileNameWithPath);

    void swap(int i, int j);
    double moransI();
    double entropyScore();
    double switchingScore();
    double simAnnealingOrderMoransI(int iters, double startTemp);
    SquareMatrix moransIDistanceMatrix();

    SquareMatrix KevinsDistanceMatrix();

    void orderTSPRaw();
    void orderTSPMoransI();
    void orderTSPKevin();
private:
    vector<int> getTSPOrder();

    void flipVertical();
    void flipHorizontal();
    void offset(int offset);

    vector<int> extraNodeOrderToNormal(vector<int> extraOrder);

    int _size;
    double maxValue;
    double minValue;

    int *wordMeanings; // Keeps information for the visualization
    int *partOfSpeech; //

    double *matrixBuffer;

    double _morans_mean = -1;
    double _morans_variance = -1;
    double _moransI = -1;
};

#endif