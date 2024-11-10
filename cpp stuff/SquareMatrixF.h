#ifndef SQUARE_MATRIX_F
#define SQUARE_MATRIX_F

#include "Image.h"
#include "ColorScheme.h"
#include <vector>
#include <string>

class SquareMatrixF
{
public:
    SquareMatrixF(int size);
    SquareMatrixF(SquareMatrixF &source);
    ~SquareMatrixF();

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

    void order(int *order);

    Image toImage(double maxDist, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
    Image toImage(double maxDist, ColorScheme *colorScheme, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
    Image toDetailedImage(double maxDist, ColorScheme* colorScheme, int offset = 0, bool flipVertical = false, bool flipHorizontal = false);

    void toTspFullMatrix(char *fileNameWithPath, char *tspName = "tsplib from matrix", char *comment = "");
    void toNeosInput(char *fileNameWithPath, char *tspName = "tsplib from matrix", char *comment = "");
    void toInFullMatrix(char *fileNameWithPath);

    void swap(int i, int j);
    double moransI();
    double getStoredMoransI();
    double simAnnealingOrderMoransI(int iters, double startTemp);
    SquareMatrixF moransIDistanceMatrix();

private:
    void flipVertical();
    void flipHorizontal();
    void offset(int offset);

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