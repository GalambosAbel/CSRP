#ifndef SQUARE_MATRIX_F
#define SQUARE_MATRIX_F

#include "Image.h"
#include "ColorScheme.h"
#include <vector>
#include <string>

class SquareMatrixF{
    public:
        SquareMatrixF(int size);
        SquareMatrixF(SquareMatrixF& source);
        ~SquareMatrixF();
        void setElement(int x, int y, double value);
        void setMaxValue(double value) {maxValue = value;};
        void setMinValue(double value) {minValue = value;};
        double getElement(int x, int y);
        double getMaxValue() {return maxValue;};
        double getMinValue() {return minValue;};
        int getSize();
        void order(int* order);
        Image toImage(double maxDist, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
        Image toImage(double maxDist, ColorScheme* colorScheme, int offset = 0, bool flipVertical = true, bool flipHorizontal = false);
        void toTspFullMatrix(char* fileNameWithPath, char* tspName = "tsplib from matrix", char* comment = "");
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
        double* matrixBuffer;
        double _morans_mean = -1;
        double _morans_variance = -1;
        double _moransI = -1;
};

#endif